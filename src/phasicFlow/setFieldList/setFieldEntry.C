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


#include "setFieldEntry.H"

pFlow::setFieldEntry::setFieldEntry
(
	const dataEntry& entry
)
:
	entry_(entry)
{}

pFlow::word pFlow::setFieldEntry::fieldName()const
{
	return entry_.keyword();
}

bool pFlow::setFieldEntry::checkForTypeAndValueAll()const
{
	if( 
		!(
			checkForTypeAndValue<int8>() ||
			checkForTypeAndValue<int16>() ||
			checkForTypeAndValue<int32>() ||
			checkForTypeAndValue<int64>() ||
			checkForTypeAndValue<uint32>() ||
			checkForTypeAndValue<label>() ||
			checkForTypeAndValue<real>()  ||
			checkForTypeAndValue<realx3>()
		 )
	   )
	{
		fatalErrorInFunction<< 
		"  un-supported data type "<<entry_.firstPart() << " in setField for field " << fieldName() <<endl;
		return false;
	}

	return true;
}

bool pFlow::setFieldEntry::setPointFieldDefaultValueNewAll
(
	repository& owner,
	pointStructure& pStruct,
	bool verbose
)
{
	if( 
		!(
			setPointFieldDefaultValueNew<int8> (owner, pStruct, verbose) ||
			setPointFieldDefaultValueNew<int16>(owner, pStruct, verbose) ||
			setPointFieldDefaultValueNew<int32> (owner, pStruct, verbose) ||
			setPointFieldDefaultValueNew<int64> (owner, pStruct, verbose) ||
			setPointFieldDefaultValueNew<uint32> (owner, pStruct, verbose) ||
			setPointFieldDefaultValueNew<label>(owner, pStruct, verbose) ||
			setPointFieldDefaultValueNew<real>(owner, pStruct, verbose)  ||
			setPointFieldDefaultValueNew<realx3>(owner, pStruct, verbose) ||
			setPointFieldDefaultValueStdNew<word>(owner, pStruct, verbose)
		 )
	   )
	{
		fatalErrorInFunction<< 
		"  un-supported data type "<<entry_.firstPart() << " in setField for field " << fieldName() <<endl;
		return false;
	}

	return true;
}

bool pFlow::setFieldEntry::setPointFieldSelectedAll
(
	repository& owner,
	int32IndexContainer& selected,
	bool verbose
)
{

	if( 
		!(
			setPointFieldSelected<int8> (owner, selected, verbose) ||
			setPointFieldSelected<int16>(owner, selected, verbose) ||
			setPointFieldSelected<int32> (owner, selected, verbose) ||
			setPointFieldSelected<int64> (owner, selected, verbose) ||
			setPointFieldSelected<uint32> (owner, selected, verbose) ||
			setPointFieldSelected<label>(owner, selected, verbose) ||
			setPointFieldSelected<real>(owner, selected, verbose)  ||
			setPointFieldSelected<realx3>(owner, selected, verbose) ||
			setPointFieldSelectedStd<word>(owner, selected, verbose)
		 )
	   )
	{
		fatalErrorInFunction<< 
		"  un-supported data type "<<entry_.firstPart() << " in setField for field " << fieldName() <<endl;
		return false;
	}

	return true;

}
