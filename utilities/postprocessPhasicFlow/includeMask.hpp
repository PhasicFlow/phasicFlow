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

#ifndef __includeMask_hpp__
#define __includeMask_hpp__

#include "virtualConstructor.hpp"
#include "readFromTimeFolder.hpp"
#include "dictionary.hpp"


namespace pFlow
{


class includeMask
{
protected:
	word 	fieldName_;

	word 	fieldType_;

	word 	operatorType_;	

	readFromTimeFolder& timeFolder_;

	static
	bool getFieldType(const dictionary& dict, readFromTimeFolder& timeFolder, word& fName, word& fType);

public:

	TypeInfo("includeMask");

	includeMask(const dictionary& dict, const word& opType, readFromTimeFolder& timeFolder);

	virtual ~includeMask() = default;

	create_vCtor(
		includeMask,
		dictionary,
		(
			const dictionary& dict,
			const word& opType,
			readFromTimeFolder& timeFolder
		),
		(dict, opType, timeFolder)		
		);

	word fieldName()const
	{
		return   fieldName_;
	}

	word fieldType()const
	{
		return fieldType_;
	}

	word operatorType()const
	{
		return operatorType_;
	}

	auto& timeFolder()
	{
		return timeFolder_;
	}

	virtual bool isIncluded(int32 n) const = 0;

	bool operator()(int32 n) const 
	{
		return isIncluded(n);
	}

	static 
	uniquePtr<includeMask> create(
		const dictionary& dict,
		const word& opType,
		readFromTimeFolder& timeFolder);
	
};



} // pFlow

#endif //__IncludeMask_hpp__


