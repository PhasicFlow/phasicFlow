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
#ifndef __readFromTimeFolder_H__
#define __readFromTimeFolder_H__


#include "repository.H"
#include "pointFields.H"
#include "utilityFunctions.H"

namespace pFlow
{

class readFromTimeFolder
{
protected:

	repository& 		repository_;

	bool checkForPointStructure()const
	{
		return repository_.lookupObjectName(pointStructureFile__);
	}

public:


	readFromTimeFolder(repository& rep);

	auto path()const
	{
		return repository_.path();
	}

	auto& db()
	{
		return repository_;
	}

	std::pair<bool, IOfileHeader> 
		fieldExists(word fieldName)const;
	


	bool pointFieldFileGetType(word fieldName, word& typeName) const;
	
	template<typename T>
	bool pointFieldGetType(word& typeName)const
	{
		word fieldTYPENAME = pointField_H<T>::TYPENAME();
		word fldType{}, space{};
		
		if( !pFlow::utilities::pointFieldGetType(
			fieldTYPENAME, fldType, space) )
		{
			fatalErrorInFunction<<
			"error in extracting type from "<<fieldTYPENAME<<endl;
			return false;
		}
	
		typeName = fldType;
		return true;
		
	}

	template<typename T>
	bool pointFieldGetCheckType(word fieldName, word& typeName) const
	{
		
		word fieldTYPENAME = pointField_H<T>::TYPENAME();
		word flType{},fldType{};

		if(!pointFieldFileGetType( fieldName, flType))
		{
			fatalExit;
			return false;
		}
		
		if( !pointFieldGetType<T>(fldType) )
		{
			fatalExit;
			return false;
		}

		
		if( flType == fldType )
		{
			typeName = flType;
			return true;
		}else
		{	
			typeName.clear();
			return false;
		}
	}

	template<typename T>
	pointField_H<T>&  createUniformPointField_H(word name, T value)
	{
		if( !checkForPointStructure() )
		{
			fatalErrorInFunction<<
			"cannot find " << pointStructureFile__ << " in repository "<< repository_.name()<<endl;
			fatalExit;
		}
		auto& pStruct = repository_.lookupObject<pointStructure>(pointStructureFile__);

		word fType;
		pointFieldGetType<T>(fType);
		word newName = name + fType;

		auto& field = repository_.emplaceReplaceObject<pointField_H<T>>(
			objectFile(
				name,
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_NEVER
				),
			pStruct,
			value
			);

		return field;

	}

	template<typename T>
	pointField_H<T>& readPointField_H(word name) 
	{
		if( !checkForPointStructure() )
		{
			fatalErrorInFunction<<
			"cannot find " << pointStructureFile__ << " in repository "<< repository_.name()<<endl;
			fatalExit;
		}
		auto& pStruct = repository_.lookupObject<pointStructure>(pointStructureFile__);

		auto& field = repository_.emplaceObjectOrGet<pointField_H<T>>(
			objectFile(
				name,
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS
				),
			pStruct,
			T{}
			);

		return field;

	}

	template<typename T>
	pointField_D<T>& readPointField_D(word name) 
	{
		if( !checkForPointStructure() )
		{
			fatalErrorInFunction<<
			"cannot find " << pointStructureFile__ << " in repository "<< repository_.name()<<endl;
			fatalExit;
		}
		auto& pStruct = repository_.lookupObject<pointStructure>(pointStructureFile__);

		auto& field = repository_.emplaceObjectOrGet<pointField_H<T>>(
			objectFile(
				name,
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS
				),
			pStruct,
			T{}
			);

		return field;
	}

};

} //pFlow



#endif //__readFromTimeFolder_H__ 
