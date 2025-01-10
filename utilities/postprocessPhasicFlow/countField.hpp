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
#ifndef __countField_hpp__
#define __countField_hpp__


#include "virtualConstructor.hpp"
#include "dictionary.hpp"
#include "readFromTimeFolder.hpp"
#include "includeMask.hpp"


namespace pFlow
{


class repository;

class countField
{
protected:

	dictionary 	dict_;
	
	mutable readFromTimeFolder 	timeFolder_;

	uniquePtr<includeMask>  	includeMask_ = nullptr;
	
	bool static getFieldType(
			const dictionary& dict,
			readFromTimeFolder& timeFolder,
			word& fieldName,
			word& fieldType);

public:

	TypeInfo("countField");

	countField(const dictionary& dict, repository& rep);

	
	auto& dict()
	{
		return dict_;
	}

	const auto& dict()const
	{
		return dict_;
	}

	auto& timeFolderRepository()
	{
		return timeFolder_.db();
	}
	
	
	auto& timeFolder()
	{
		return timeFolder_;
	}

	word variableName()const
	{
		return dict_.name();
	}

	// requires a class to read pointField from timefolder 
	bool process(uint32& countedValue);



	//virtual bool writeToFile(iOstream& is) const = 0;

	/*static 
	uniquePtr<countField> create(
		const dictionary& dict,
		repository& rep);*/
};


}


#endif //__countField_hpp__