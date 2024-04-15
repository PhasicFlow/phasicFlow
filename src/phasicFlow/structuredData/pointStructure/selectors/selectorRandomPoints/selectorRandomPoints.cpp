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


#include "selectorRandomPoints.hpp"
#include "dictionary.hpp"
#include "uniformRandomUint32.hpp"
#include "Set.hpp"


bool pFlow::selectorRandomPoints::selectAllPointsInRange()
{
	// to reduct allocations
	uint32 maxNum = number_+1;
	
	selectedPoints_.reserve	(maxNum);

	selectedPoints_.clear();
	
	uniformRandomUint32 intRand (begin_, end_);


	uint32 n = 0;
	uint32 iter = 0;
	bool finished = false;

	Set<uint32> selctedIndices;

	while( iter < number_*100)
	{
		uint32 newInd = intRand.randomNumber();

		if( auto [it, inserted] = selctedIndices.insert(newInd); inserted )
		{
			n++;

			if(n == number_)
			{
				finished = true;
				break;
			}
		}
		iter++;
	}

	
	if(finished)
	{
		for(auto& ind:selctedIndices)
		{
			selectedPoints_.push_back(ind);
		}

		return true;

	}else
	{
		fatalErrorInFunction<< "Could not find random indices in the range."<<endl;
		return false;	
	}

	
}
	
pFlow::selectorRandomPoints::selectorRandomPoints
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
		dict.subDict("randomPointsInfo").getVal<uint32>("begin")
	),
	end_
	(
		dict.subDict("randomPointsInfo").getValOrSet("end", pStruct.size())
	),
	number_
	(
		dict.subDict("randomPointsInfo").getValOrSet("number", 1)
	)
{
	begin_ 	= max(begin_,1u);
	end_ 	= min(end_, static_cast<uint32>(pStruct.size()));
	number_ = max(number_,0u);
	if(end_-begin_ < number_)
	{

		warningInFunction<< "In dictionary " << dict.globalName()<<
		" number is greater than the interval defined by begine and end ["<<
		begin_<<" "<<end_<<"), resetting it to "<<end_-begin_<<endl;

		number_ = end_-begin_;
	}

	if(!selectAllPointsInRange())
	{
		fatalExit;
	}
}
