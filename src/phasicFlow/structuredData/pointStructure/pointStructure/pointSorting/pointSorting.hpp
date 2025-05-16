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
#ifndef __pointSorting_hpp__
#define __pointSorting_hpp__

#include "baseTimeControl.hpp"
#include "indexContainer.hpp"
#include "pointFlag.hpp"


namespace pFlow
{

class box;


class pointSorting
{
private:

	Logical                 performSorting_;

	baseTimeControl         timeControl_;

	real 					dx_;

public:

	explicit pointSorting(const dictionary& dict);

	bool performSorting()const
	{
		return performSorting_();
	}

	bool sortTime(const timeInfo& ti)const
	{
		return performSorting_() && timeControl_.eventTime(ti);
	}

	uint32IndexContainer getSortedIndices(
		const box& boundingBox, 
		const ViewType1D<realx3>& pos,
		const pFlagTypeDevice& flag
	)const;
	
};

}

#endif