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

#ifndef __boundaryProcessor_hpp__
#define __boundaryProcessor_hpp__


#include "boundaryBase.hpp"

namespace pFlow
{

class boundaryProcessor
:
 	public boundaryBase
{
protected:

	
public:

	TypeInfo("boundary<processor>");

	boundaryProcessor(
		const dictionary& 	dict,
		const plane&    	bplane,
		internalPoints& 	internal);

	virtual 
	~boundaryProcessor() = default;
	
	add_vCtor
	(
		boundaryBase,
		boundaryProcessor,
		dictionary
	);

	bool beforeIteratoin(uint32 iterNum, real t) override;

	bool iterate(uint32 iterNum, real t) override;

	bool afterIteration(uint32 iterNum, real t) override;


};

}

#endif //__boundaryProcessor_hpp__