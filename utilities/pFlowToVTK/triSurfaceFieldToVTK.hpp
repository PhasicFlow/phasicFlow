/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/


#ifndef __triSurfaceFieldToVTK_hpp__
#define __triSurfaceFieldToVTK_hpp__

#include <regex>

#include "vtkFile.hpp"
#include "triSurface.hpp"
#include "multiTriSurface.hpp"
#include "triSurfaceFields.hpp"
#include "IOobject.hpp"

namespace pFlow::TSFtoVTK
{

bool regexCheck(word TYPENAME, word fieldType)
{
	std::regex match("triSurfaceField\\<([A-Za-z1-9_]*)\\,([A-Za-z1-9_]*)\\>");
	std::smatch search1, search2;
	if(!std::regex_match(fieldType, search1, match))return false;
	if(!std::regex_match(TYPENAME, search2, match))return false;
	if(search1.size()!=3)return false;
	if(search1.size()!=search2.size())return false;
	return search1[1] == search2[1];
}

template<typename Type>
bool checkFieldType(word objectType)
{
	//if( pointField<VectorSingle,Type>::TYPENAME() == objectType )return true;
	//if( pointField<VectorSingle,Type, HostSpace>::TYPENAME() == objectType ) return true;
	//if( pointField<VectorDual, Type>::TYPENAME() == objectType )return true;
	return regexCheck(triSurfaceField<VectorSingle,Type>::TYPENAME(), objectType);
	
}

template<typename Type>
bool triDataToVTK(iOstream& os, const Type& dataEntity)
{
	fatalErrorInFunction<<
	"not implemented function!";
	fatalExit;
	return false;
}

template<>
bool triDataToVTK(iOstream& os, const triSurface& surface )
{
	auto nP = surface.numPoints();
	auto hPoints = surface.points().hostVector();

	os << "DATASET POLYDATA" << endl;
	os << "POINTS " << nP << " float" << endl;


	for ( auto i=0; i<nP; i++ )
	{
		os << hPoints[i].x()  << " " << hPoints[i].y() << " " << hPoints[i].z() << endl;
	}

	auto nV = surface.numTriangles();
	auto hVertices = surface.vertices().hostVector();

	os << "POLYGONS " << nV << "  " << 4*nV << endl;

	for(auto i=0; i<nV; i++)
	{
		os<< 3 <<" "<< hVertices[i].x() << " " << hVertices[i].y() <<" "<<hVertices[i].z()<<endl;
	}

	return true;
}

template<>
bool triDataToVTK(iOstream& os, const multiTriSurface& surface )
{
	auto nP = surface.numPoints();
	auto hPoints = surface.points().hostVector();

	os << "DATASET UNSTRUCTURED_GRID" << endl;
	os << "POINTS " << nP << " float" << endl;


	for ( auto i=0; i<nP; i++ )
	{
		os << hPoints[i].x()  << " " << hPoints[i].y() << " " << hPoints[i].z() << endl;
	}

	auto nV = surface.numTriangles();
	auto hVertices = surface.vertices().hostVector();

	os<<"CELLS "<< nV<<' '<< 4*nV<<'\n';
	//os << "POLYGONS " << nV << "  " << 4*nV << endl;

	for(auto i=0; i<nV; i++)
	{
		os<< 3 <<" "<< hVertices[i].x() << " " << hVertices[i].y() <<" "<<hVertices[i].z()<<endl;
	}

	os<<"CELL_TYPES "<< nV<<'\n';

	for(int32 i=0; i<nV; i++)
	{
		os<< 5 <<'\n';
	}
	
	os << "CELL_DATA " << nV << endl;

	return true;
}

bool addRealx3TriSurfaceField(
	iOstream& 	os,
	word 		fieldName,
	int32 		size,
	realx3* 	field )
{
	if(size==0) return true;


	os << "FIELD FieldData 1\n"<<
	fieldName << " 3 " << size << " float\n";
	for(int32 i=0; i<size; ++i)
	{
			os<< field[i].x()<<' '<< field[i].y()<<' '<<field[i].z()<<'\n';
	}

	return true;
}

bool convertRealx3TypetriSurfaceField(
	iOstream& os,
	const IOfileHeader& header,
	const multiTriSurface& tSurface)
{
	word objectType = header.objectType();

	if(!checkFieldType<realx3>(objectType))return false;

	auto objField = IOobject::make<realx3TriSurfaceField_H>
	(
		header,
		tSurface,
		static_cast<real>(0)
	);

	auto& Field = objField().getObject<realx3TriSurfaceField_H>();

	realx3* data = Field.hostVectorAll().data();
	
	REPORT(2)<<"writing "<< greenColor <<header.objectName()<<defaultColor<<" field to vtk."<<endREPORT;

	return addRealx3TriSurfaceField(
		os,
		header.objectName(),
		tSurface.size(),
		data );
}

bool convertTimeFolderTriSurfaceFields( 
	fileSystem timeFolder,
	real time,
	fileSystem destPath,
	word bName)
{

	// check if pointStructure exist in this folder 
	IOfileHeader triSurfaeHeader(
		objectFile(
			triSurfaceFile__,
			timeFolder,
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS)
		);

	if( !triSurfaeHeader.headerOk(true) )
	{
		output<<yellowText("Time folder "<< timeFolder <<
		" does not contain any triSurface data file. Skipping this folder . . ."
		)<<nl;
		return true;
	}

	vtkFile vtk(destPath, bName, time);

	if(!vtk) return false;

	auto triSurfaceObjPtr = IOobject::make<multiTriSurface>(triSurfaeHeader);
	auto& tSurface = triSurfaceObjPtr().getObject<multiTriSurface>();

	// get a list of files in this timeFolder;
	REPORT(1)<<"Wrting triSurface mesh/Geometry to vtk file."<<endREPORT;
	if(!triDataToVTK(vtk(), tSurface))
	{
		fatalErrorInFunction<<
		"error in writing triSurface data to vtk file "<< vtk.fileName()<<endl;
		return false;
	}


	auto fileList = containingFiles(timeFolder);


	for(auto& file:fileList)
	{
		IOfileHeader fieldHeader(
			objectFile(
			file.wordPath(),
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS) );

		if( fieldHeader.headerOk(true) )
		{
			//output<<"object file type is "<<fieldHeader.objectType()<<endl;
			convertRealx3TypetriSurfaceField(vtk(), fieldHeader, tSurface);
		}
	}

	return true;
}

}

#endif 
