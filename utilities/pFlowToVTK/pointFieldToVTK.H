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


#ifndef __pointFieldToVTK_H__
#define __pointFieldToVTK_H__

#include <regex>

#include "vtkFile.H"
#include "pointFields.H"
#include "IOobject.H"

namespace pFlow::PFtoVTK
{

template<typename IncludeMaskType>
bool addInt64PointField(
	iOstream& 	os,
	word 		fieldName,
	int32 		numActivePoints,
	int64* 		field,
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
	IncludeMaskType includeMask );

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
	//if( pointField<VectorSingle,Type>::TYPENAME() == objectType )return true;
	//if( pointField<VectorSingle,Type, HostSpace>::TYPENAME() == objectType ) return true;
	//if( pointField<VectorDual, Type>::TYPENAME() == objectType )return true;
	return regexCheck(pointField<VectorSingle,Type>::TYPENAME(), objectType);
	
}

bool convertIntTypesPointField(
	iOstream& os,
	const IOfileHeader& header,
	const pointStructure& pStruct )
{
	word objectType = header.objectType();

	if( !(checkFieldType<int8>(objectType)  ||
		checkFieldType<int16>(objectType) 	||
		checkFieldType<int32>(objectType) 	||
		checkFieldType<int64>(objectType) 	||
		checkFieldType<uint32>(objectType) 	||
		checkFieldType<label>(objectType))
		)
	{
		return false;
	}

	auto objField = IOobject::make<int64PointField_H>
	(
		header,
		pStruct,
		static_cast<int64>(0)
	);

	auto& Field = objField().getObject<int64PointField_H>();

	int64* data = Field.hostVectorAll().data();
	
	Report(2)<<"writing "<< greenColor <<header.objectName()<<defaultColor<<" field to vtk.\n";

	return addInt64PointField(
		os,
		header.objectName(),
		pStruct.numActive(),
		data,
		pStruct.activePointsMaskH() );

}

bool convertRealTypePointField(
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
	
	Report(2)<<"writing "<< greenColor <<header.objectName()<<defaultColor<<" field to vtk."<<endReport;

	return addRealPointField(
		os,
		header.objectName(),
		pStruct.numActive(),
		data,
		pStruct.activePointsMaskH() );
}

bool convertRealx3TypePointField(
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
	
	Report(2)<<"writing "<< greenColor <<header.objectName()<<defaultColor<<" field to vtk."<<endReport;

	return addRealx3PointField(
		os,
		header.objectName(),
		pStruct.numActive(),
		data,
		pStruct.activePointsMaskH() );
}


template<typename IncludeMaskType>
bool addUndstrcuturedGridField(
	iOstream& 	os,
	int32 		numActivePoints,
	realx3* 	position,
	IncludeMaskType includeMask)
{
	
	

	auto [iFirst, iLast] = includeMask.activeRange();

	os<< "DATASET UNSTRUCTURED_GRID\n";
	os<< "POINTS "<< numActivePoints << " float\n"; 
	
	if(numActivePoints==0) return true;
	
	for(int32 i=iFirst; i<iLast; ++i)
	{
		if(includeMask(i))
			os<< position[i].x()<<' '<< position[i].y()<<' '<<position[i].z()<<'\n';
	}

	os<<"CELLS "<< numActivePoints<<' '<< 2*numActivePoints<<'\n';
	for(int32 i=0; i<numActivePoints; i++)
	{
		os<< 1 <<' '<< i<<'\n';
	}

	os<<"CELL_TYPES "<< numActivePoints<<'\n';

	for(int32 i=0; i<numActivePoints; i++)
	{
		os<< 1 <<'\n';
	}
	
	os << "POINT_DATA " << numActivePoints << endl;

	return true;
}


template<typename IncludeMaskType>
bool addInt64PointField(
	iOstream& 	os,
	word 		fieldName,
	int32 		numActivePoints,
	int64* 		field,
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
}

template<typename IncludeMaskType>
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
}

template<typename IncludeMaskType>
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
}


bool convertTimeFolderPointFields( 
	fileSystem timeFolder,
	real time,
	fileSystem destPath,
	word bName)
{

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

	Report(1)<<"Writing pointStructure to vtk file with "<< yellowText(pStruct.numActive())
					 <<" active particles"<<endReport;
	addUndstrcuturedGridField(
		vtk(),
		pStruct.numActive(),
		pos,
		pStruct.activePointsMaskH());

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
			convertIntTypesPointField(vtk(), fieldHeader, pStruct);
			convertRealTypePointField(vtk(), fieldHeader, pStruct);
			convertRealx3TypePointField(vtk(), fieldHeader, pStruct);
		}
	}

	return true;
}
 
}



#endif 
