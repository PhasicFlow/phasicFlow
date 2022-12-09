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


#ifndef __setFields_hpp__
#define __setFields_hpp__

#include "pStructSelector.hpp"
#include "pointFields.hpp"
#include "setFieldList.hpp"
#include "systemControl.hpp"

namespace pFlow
{


bool applySelector(systemControl& control, const pointStructure& pStruct, const dictionary& selDict)
{

	
	auto selector = pStructSelector::create(pStruct, selDict);

	auto& selected = selector().selectedPoinsts();

	int32IndexContainer selIndex(selected.data(), selected.size());

	setFieldList sfList(selDict.subDict("fieldValue"));

	for(auto& sfEntry:sfList)
	{
		if(!sfEntry.setPointFieldSelectedAll(control.time(), selIndex, true ))return false;
	}
	
	return true;
}

} // pFlow

#endif //__setFields_hpp__
