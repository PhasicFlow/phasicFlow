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


#include "insertionRegion.H"
#include "dictionary.H"

bool pFlow::insertionRegion::readInsertionRegion
(
	const dictionary& dict
)
{

	name_ = dict.name();
	type_ = dict.getVal<word>("type");
	
	pRegion_  = peakableRegion::create(type_, dict.subDict(type_+"Info"));
	
	mixture_ = makeUnique<shapeMixture>(dict.subDict("mixture"));

	addToNumInserted(mixture_().totalInserted());

	if( !dict.containsDictionay("setFields"))
	{
		output<<"\n insertion region "<< name_ << " does not contain setFields dictionary."
		" An empty dictoiinary is created for it. \n";
		setFields_ = makeUnique<setFieldList>( dictionary("setFields") );
	}
	else
	{
		setFields_ = makeUnique<setFieldList>( dict.subDict("setFields") );
	}

	for(auto& sfEntry:setFields_())
	{
		if(!sfEntry.checkForTypeAndValueAll())
		{
			fatalErrorInFunction<<
			"   error in setFields dictionary "<< dict.globalName()<<endl;
			return false;
		}
	}
	
	return true;
}

bool pFlow::insertionRegion::writeInsertionRegion
(
	dictionary& dict
) const
{
	
	if(!dict.add("type", type_)) return false;
	

	if(pRegion_)
	{
		auto& prDict = dict.subDictOrCreate(type_+"Info");
		if(!pRegion_().write(prDict)) return false;
	}

	if(mixture_)
	{
		auto& mixDict = dict.subDictOrCreate("mixture");
		if(!mixture_().write(mixDict)) return false;
	}

	if(setFields_)
	{
		auto& sfDict = dict.subDictOrCreate("setFields");
		setFields_().write(sfDict);
	}

	return true;
}

pFlow::insertionRegion::insertionRegion
(
	const dictionary& dict
)
:
	timeFlowControl(dict)
{

	if(!readInsertionRegion(dict))
	{
		fatalExit;
	}
}

pFlow::insertionRegion::insertionRegion
(
	const insertionRegion& src
)
:
	timeFlowControl(src),
	name_(src.name_),
	type_(src.type_),
	pRegion_( src.pRegion_? src.pRegion_->clone(): nullptr),
	mixture_( src.mixture_? src.mixture_->clone(): nullptr),
	setFields_( src.setFields_? src.setFields_->clone(): nullptr)
{}

pFlow::insertionRegion& pFlow::insertionRegion::operator=
(
	const insertionRegion& src
)
{

	if(&src == this)return *this;
	timeFlowControl::operator=(src);

	name_ = src.name_;
	type_ = src.type_;
	pRegion_ = (src.pRegion_? src.pRegion_->clone(): nullptr);
	mixture_ = (src.mixture_? src.mixture_->clone(): nullptr);
	setFields_ = (src.setFields_? src.setFields_->clone(): nullptr);

	return *this;
}


