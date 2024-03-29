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


#ifndef __pointFieldToVTK_hpp__
#define __pointFieldToVTK_hpp__

#include <regex>

#include "vtkFile.hpp"
#include "systemControl.hpp"
#include "pointStructure.hpp"
#include "pointFields.hpp"


namespace pFlow::PFtoVTK
{

/*template<typename IntType, typename IncludeMaskType>
bool addIntPointField(
	iOstream& 	os,
	word 		fieldName,
	int32 		numActivePoints,
	IntType* 		field,
	IncludeMaskType includeMask );

template<typename IncludeMaskType>
bool addRealPointField(
	iOstream& 	os,
	word 		fieldName,
	int32 		numActivePoints,
	real* 		field,
	IncludeMaskType includeMask );

template<typename IncludeMaskType>
bool addRealx3PointField(
	iOstream& 	os,
	word 		fieldName,
	int32 		numActivePoints,
	realx3* 	field,
	IncludeMaskType includeMask );*/

bool regexCheck(word TYPENAME, word fieldType)
{
	std::regex match("pointField\\<([A-Za-z1-9_]*)\\,([A-Za-z1-9_]*)\\>");
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
	return regexCheck(pointField<Type>::TYPENAME(), objectType);
}

/*template<typename T>
bool convertIntPointField
(
	iOstream& os,
	const IOfileHeader& header,
	const pointStructure& pStruct
)
{

	using PointFieldType = pointField<VectorSingle, T, HostSpace>;

	word objectType = header.objectType();

	if(!checkFieldType<T>(objectType))
	{
		return false;
	}

	auto objField = IOobject::make<PointFieldType>
	(
		header,
		pStruct,
		static_cast<T>(0)
	);

	auto& Field = objField().template getObject<PointFieldType>();

	T* data = Field.deviceVectorAll().data();
	
	REPORT(2)<<"writing "<< greenColor <<header.objectName()<<defaultColor<<" field to vtk.\n";

	return addIntPointField(
		os,
		header.objectName(),
		pStruct.numActive(),
		data,
		pStruct.activePointsMaskH() );
}*/


/*bool convertRealTypePointField(
	iOstream& os,
	const IOfileHeader& header,
	const pointStructure& pStruct)
{
	word objectType = header.objectType();

	if(!checkFieldType<real>(objectType))return false;

	auto objField = IOobject::make<realPointField_H>
	(
		header,
		pStruct,
		static_cast<real>(0)
	);

	auto& Field = objField().getObject<realPointField_H>();

	real* data = Field.hostVectorAll().data();
	
	REPORT(2)<<"writing "<< greenColor <<header.objectName()<<defaultColor<<" field to vtk."<<endREPORT;

	return addRealPointField(
		os,
		header.objectName(),
		pStruct.numActive(),
		data,
		pStruct.activePointsMaskH() );
}*/

/*bool convertRealx3TypePointField(
	iOstream& os,
	const IOfileHeader& header,
	const pointStructure& pStruct)
{
	word objectType = header.objectType();

	if(!checkFieldType<realx3>(objectType))return false;

	auto objField = IOobject::make<realx3PointField_H>
	(
		header,
		pStruct,
		static_cast<real>(0)
	);

	auto& Field = objField().getObject<realx3PointField_H>();

	realx3* data = Field.hostVectorAll().data();
	
	REPORT(2)<<"writing "<< greenColor <<header.objectName()<<defaultColor<<" field to vtk."<<endREPORT;

	return addRealx3PointField(
		os,
		header.objectName(),
		pStruct.numActive(),
		data,
		pStruct.activePointsMaskH() );
}*/


bool addUndstrcuturedGridField(
	iOstream& 	os,
	realx3* 	position,
	uint32 		numPoints)
{

	os<< "DATASET UNSTRUCTURED_GRID\n";
	os<< "POINTS "<< numPoints << " float\n"; 
	
	if(numPoints==0) return true;
	
	for(uint32 i=0; i<numPoints; i++)
	{
		os<< position[i].x()<<' '<< position[i].y()<<' '<<position[i].z()<<'\n';
	}

	os<<"CELLS "<< numPoints<<' '<< 2*numPoints<<'\n';
	for(uint32 i=0; i<numPoints; i++)
	{
		os<< 1 <<' '<< i<<'\n';
	}

	os<<"CELL_TYPES "<< numPoints<<'\n';

	for(int32 i=0; i<numPoints; i++)
	{
		os<< 1 <<'\n';
	}
	
	os << "POINT_DATA " << numPoints << endl;

	return true;
}


/*template<typename IntType, typename IncludeMaskType>
bool addIntPointField(
	iOstream& 	os,
	word 		fieldName,
	int32 		numActivePoints,
	IntType* 		field,
	IncludeMaskType includeMask )
{
	if(numActivePoints==0) return true;

	auto [iFirst, iLast] = includeMask.activeRange();

	os << "FIELD FieldData 1\n"<<
	fieldName << " 1 " << numActivePoints << " int\n";
	for(int32 i=iFirst; i<iLast; ++i)
	{
		if(includeMask(i))
			os<< field[i] <<'\n';
	}

	return true;
}*/

/*template<typename IncludeMaskType>
bool addRealPointField(
	iOstream& 	os,
	word 		fieldName,
	int32 		numActivePoints,
	real* 		field,
	IncludeMaskType includeMask )
{
	if(numActivePoints==0) return true;

	auto [iFirst, iLast] = includeMask.activeRange();

	os << "FIELD FieldData 1\n"<<
	fieldName << " 1 " << numActivePoints << " float\n";
	for(int32 i=iFirst; i<iLast; ++i)
	{
		if(includeMask(i))
			os<< field[i] <<'\n';
	}
	return true;
}*/

/*template<typename IncludeMaskType>
bool addRealx3PointField(
	iOstream& 	os,
	word 		fieldName,
	int32 		numActivePoints,
	realx3* 	field,
	IncludeMaskType includeMask )
{
	if(numActivePoints==0) return true;

	auto [iFirst, iLast] = includeMask.activeRange();

	os << "FIELD FieldData 1\n"<<
	fieldName << " 3 " << numActivePoints << " float\n";
	for(int32 i=iFirst; i<iLast; ++i)
	{
		if(includeMask(i))
			os<< field[i].x()<<' '<< field[i].y()<<' '<<field[i].z()<<'\n';
	}

	return true;
}*/


bool convertTimeFolderPointFields( 
	systemControl& control,
	fileSystem destPath,
	word bName)
{

	fileSystem timeFolder = control.time().path();
	// check if pointStructure exist in this folder 
	IOfileHeader pStructHeader(
		objectFile(
			pointStructureFile__,
			timeFolder,
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS)
		);

	if( !pStructHeader.headerOk(true) )
	{
		output<<yellowColor<<"Time folder "<< control.time().path() <<
		" does not contain any pStructure data file. Skipping this folder . . ."
		<<defaultColor<<nl;
		return true;
	}

	vtkFile vtk(destPath, bName, control.time().currentTime());

	if(!vtk) return false;

	
	auto pStruct = pointStructure(control);

	// get a list of files in this timeFolder;

	auto posVec = pStruct.pointPositionHost();
	auto* pos = posVec.data();

	REPORT(1)<<"Writing pointStructure to vtk file with "<< Yellow_Text(pStruct.numActive())
					 <<" active particles"<<END_REPORT;
	addUndstrcuturedGridField(
		vtk(),
		pos,
		pStruct.numActive());

	//auto fileList = containingFiles(timeFolder);


	/*for(auto& file:fileList)
	{
		IOfileHeader fieldHeader(
			objectFile(
			file.wordPath(),
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS) );

		if( fieldHeader.headerOk(true) )
		{
			convertIntPointField<int32>(vtk(), fieldHeader, pStruct);
			convertIntPointField<int64>(vtk(), fieldHeader, pStruct);
			convertIntPointField<int8>(vtk(), fieldHeader, pStruct);
			convertRealTypePointField(vtk(), fieldHeader, pStruct);
			convertRealx3TypePointField(vtk(), fieldHeader, pStruct);
		}
	}*/

	return true;
}



bool convertTimeFolderPointFieldsSelected( 
	fileSystem 			timeFolder,
	real 				time,
	fileSystem 			destPath,
	word 				bName,
	wordVector 			fieldsName,
	bool 				mustExist)
{

	// check if pointStructure exist in this folder 
	/*IOfileHeader pStructHeader(
		objectFile(
			pointStructureFile__,
			timeFolder,
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS)
		);

	if( !pStructHeader.headerOk(true) )
	{
		output<<yellowColor<<"Time folder "<< timeFolder <<
		" does not contain any pStructure data file. Skipping this folder . . ."
		<<defaultColor<<nl;
		return true;
	}

	vtkFile vtk(destPath, bName, time);

	if(!vtk) return false;

	auto pStructObjPtr = IOobject::make<pointStructure>(pStructHeader);
	auto& pStruct = pStructObjPtr().getObject<pointStructure>();

	// get a list of files in this timeFolder;

	auto posVec = std::as_const(pStruct).pointPosition().hostVectorAll();
	auto* pos = posVec.data();

	REPORT(1)<<"Writing pointStructure to vtk file with "<< yellowText(pStruct.numActive())
					 <<" active particles"<<endREPORT;
	addUndstrcuturedGridField(
		vtk(),
		pStruct.numActive(),
		pos,
		pStruct.activePointsMaskH());

	auto fileList = containingFiles(timeFolder);


	for(auto& fname:fieldsName)
	{
		fileSystem fieldAddress = timeFolder+fname;
		
		IOfileHeader fieldHeader(
			objectFile(
			fieldAddress.wordPath(),
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS) );

		if( fieldHeader.headerOk(true) )
		{
			convertIntPointField<int32>(vtk(), fieldHeader, pStruct);
			convertIntPointField<int64>(vtk(), fieldHeader, pStruct);
			convertIntPointField<int8>(vtk(), fieldHeader, pStruct);
			convertRealTypePointField(vtk(), fieldHeader, pStruct);
			convertRealx3TypePointField(vtk(), fieldHeader, pStruct);
		}
		else
		{
			if(mustExist)
			{
				fatalErrorInFunction<<"Field " << fieldAddress << 
				" does not exist."<<endl;
				return false;
			}
			else
			{
				REPORT(1)<<"Could not find "<<yellowText(fieldAddress) <<" skipping . . ."<<endREPORT;
			}
		}
	}*/

	return true;
}

 
}



#endif 
