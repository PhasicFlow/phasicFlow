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

#ifndef __processField_H__
#define __processField_H__


#include "virtualConstructor.H"
#include "dictionary.H"
#include "readFromTimeFolder.H"
#include "includeMask.H"
#include "pointRectCell.H"

namespace pFlow
{


class repository;

class processField
{
protected:

	dictionary 					dict_;

	pointRectCell& 				pointToCell_;
	
	mutable readFromTimeFolder 	timeFolder_;

	word 	processedFieldName_;

	word  	fieldName_;

	word  	fieldType_;

	word 	action_;

	word  	includeMaskType_;

	int32 	threshold_ = 1;

	uniquePtr<includeMask> includeMask_ = nullptr;

	bool static getFieldType(
			const dictionary& dict,
			readFromTimeFolder& timeFolder,
			word& fieldName,
			word& fieldType);

public:

	TypeName("processField");

	processField(const dictionary& dict, pointRectCell& pToCell, repository& rep);


	create_vCtor(
			processField,
			dictionary,
			(const dictionary& dict,
			 pointRectCell& pToCell,
			 repository& rep),
			(dict, pToCell, rep) );


	const auto& mesh()const
	{
		return pointToCell_.mesh();
	}

	const auto& pointToCell()const
	{
		return pointToCell_;
	}

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

	auto& processedRepository()
	{
		return pointToCell_.processedRepository();
	}

	const word& fieldType()const
	{
		return fieldType_;
	}
	
	const word& fieldName()const
	{
		return fieldName_;
	}
	
	bool isUniform()const
	{
		return fieldName_ == "uniformField";
	}

	const word& action()const
	{
		return action_;
	}

	auto& timeFolder()
	{
		return timeFolder_;
	}

	const word& includeMaskType()const
	{
		return includeMaskType_;
	}

	auto threshold()const
	{
		return threshold_;
	}
	
	const word& processedFieldName()const
	{
		return processedFieldName_;
	}

	// requires a class to read pointField from timefolder 
	virtual bool process() = 0;

	virtual bool writeToVTK(iOstream& is) const = 0;

	static 
	uniquePtr<processField> create(
		const dictionary& dict,
		pointRectCell& pToCell,
		repository& rep);
};


}


#endif //__processField_H__