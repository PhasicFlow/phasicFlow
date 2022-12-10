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


#include "selectRange.hpp"
#include "dictionary.hpp"

void pFlow::selectRange::selectAllPointsInRange()
{
	// to reduct allocations
	int32 maxNum = (end_ - begin_)/stride_+2;
	
	selectedPoints_.reserve	(maxNum);

	selectedPoints_.clear();
		
	for(int32 i = begin_; i<end_; i+= stride_)
	{
		selectedPoints_.push_back(i);
	}
}

pFlow::selectRange::selectRange
(
	const pointStructure& pStruct,
	const dictionary& dict
)
:
	pStructSelector
	(
		pStruct, dict
	),
	begin_
	(
		dict.subDict("selectRangeInfo").getVal<int32>("begin")
	),
	end_
	(
		dict.subDict("selectRangeInfo").getValOrSet("end", pStruct.size())
	),
	stride_
	(
		dict.subDict("selectRangeInfo").getValOrSet("stride", 1)
	)
{
	begin_ 	= max(begin_,1);
	end_ 	= min(end_, static_cast<int32>(pStruct.size()));
	stride_ = max(stride_,1);

	selectAllPointsInRange();
}
