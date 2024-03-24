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


#include "setFieldEntry.hpp"

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
			checkForTypeAndValue<uint8>() ||
			checkForTypeAndValue<int32>() ||
			checkForTypeAndValue<int64>() ||
			checkForTypeAndValue<uint32>() ||
			checkForTypeAndValue<uint64>() ||
			checkForTypeAndValue<real>()  ||
			checkForTypeAndValue<realx3>() ||
			checkForTypeAndValue<realx4>() ||
			checkForTypeAndValue<word>()
		 )
	   )
	{
		fatalErrorInFunction<< 
		"  un-supported data type "<<entry_.firstPart() << " in setField for field " << fieldName() <<endl;
		return false;
	}

	return true;
}

pFlow::uniquePtr<pFlow::IOobject> 
	pFlow::setFieldEntry::setPointFieldDefaultValueNewAll
(
	pointStructure& pStruct,
	bool verbose
)
{
	uniquePtr<IOobject> Ptr = nullptr;

	if(  Ptr =  setPointFieldDefaultValueNew<int8>(pStruct, verbose) ; Ptr)
	{
		return Ptr;
	}
	else if(Ptr = setPointFieldDefaultValueNew<uint8>(pStruct, verbose); Ptr)
	{
		return Ptr;
	}
	else if(Ptr = setPointFieldDefaultValueNew<int32> (pStruct, verbose); Ptr)
	{
		return Ptr;
	}
	else if(Ptr = setPointFieldDefaultValueNew<int64> (pStruct, verbose); Ptr)
	{
		return Ptr;
	}
	else if(Ptr = setPointFieldDefaultValueNew<uint32> (pStruct, verbose); Ptr)
	{
		return Ptr;
	}
	else if(Ptr = setPointFieldDefaultValueNew<int64>(pStruct, verbose); Ptr)
	{
		return Ptr;
	}
	else if(Ptr = setPointFieldDefaultValueNew<uint64>(pStruct, verbose); Ptr)
	{
		return Ptr;
	}
	else if(Ptr = setPointFieldDefaultValueNew<real>(pStruct, verbose); Ptr)
	{
		return Ptr;
	}
	else if(Ptr = setPointFieldDefaultValueNew<realx3>(pStruct, verbose); Ptr)
	{
		return Ptr;
	}
	else if(Ptr = setPointFieldDefaultValueNew<realx4>(pStruct, verbose); Ptr)
	{
		return Ptr;
	}
	else if(Ptr = setPointFieldDefaultValueNew<word>(pStruct, verbose); Ptr)
	{
		return Ptr;
	}else
	{
		fatalErrorInFunction<< 
		"  un-supported data type "<<entry_.firstPart() << " in setField for field " << fieldName() <<endl;
		return nullptr;
	}

}

bool pFlow::setFieldEntry::setPointFieldSelectedAll
(
	repository& owner,
	uint32IndexContainer& selected,
	bool verbose
)
{

	if( setPointFieldSelected<int8> (owner, selected, verbose))
	{
		return true;
	}
	else if( setPointFieldSelected<uint8>(owner, selected, verbose))
	{
		return true;
	}
	else if( setPointFieldSelected<int32>(owner, selected, verbose))
	{
		return true;
	}
	else if( setPointFieldSelected<uint32>(owner, selected, verbose))
	{
		return true;
	}
	else if( setPointFieldSelected<int64>(owner, selected, verbose))
	{
		return true;
	}
	else if( setPointFieldSelected<uint64>(owner, selected, verbose))
	{
		return true;
	}
	else if( setPointFieldSelected<real>(owner, selected, verbose))
	{
		return true;
	}
	else if( setPointFieldSelected<realx3>(owner, selected, verbose))
	{
		return true;
	}
	else if( setPointFieldSelected<realx4>(owner, selected, verbose))
	{
		return true;
	}
	else if( setPointFieldSelected<word>(owner, selected, verbose))
	{
		return true;
	}
	else
	{
		fatalErrorInFunction<< 
		"  un-supported data type "<<entry_.firstPart() << " in setField for field " << fieldName() <<endl;
		return false;
	}

}
