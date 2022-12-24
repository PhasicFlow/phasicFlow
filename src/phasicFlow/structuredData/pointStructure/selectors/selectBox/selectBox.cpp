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


#include "selectBox.hpp"


void pFlow::selectBox::selectAllPointsInBox()
{
	// to reduct allocations
	selectedPoints_.reserve
	(
		static_cast<size_t>(0.2*pStruct().size())
	);

	selectedPoints_.clear();
	auto pPos = pStruct().pointPosition().hostVector();
	
	ForAll(i , pPos)
	{
		if( box_.isInside( pPos[i] )) selectedPoints_.push_back(i);
	}
}

pFlow::selectBox::selectBox
(
	const pointStructure& pStruct,
	const dictionary& dict
)
:
	pStructSelector
	(
		pStruct, dict
	),
	box_
	(
		dict.subDict("selectBoxInfo")
	)
{
	selectAllPointsInBox();
}
