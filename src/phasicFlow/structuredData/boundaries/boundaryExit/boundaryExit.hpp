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

#ifndef __boundaryExit_hpp__
#define __boundaryExit_hpp__


#include "boundaryBase.hpp"

namespace pFlow
{

class boundaryExit
:
 	public boundaryBase
{
protected:

	/// @brief Length of margin in the negative side of 
	/// boundary to still allow points stay in the 
	/// simulation. 
	real 	exitMarginLength_ = 0;

	/// @brief between successive check for particle exit
	/// (iteration-based).
	uint32 	checkForExitInterval_ = 1;

public:

	TypeInfo("boundary<exit>");

	boundaryExit(
		const dictionary& 	dict,
		const plane&    	bplane,
		internalPoints& 	internal);

	virtual 
	~boundaryExit() = default;
	
	add_vCtor
	(
		boundaryBase,
		boundaryExit,
		dictionary
	);

	bool beforeIteration(uint32 iterNum, real t, real dt) override;

	bool iterate(uint32 iterNum, real t) override;

	bool afterIteration(uint32 iterNum, real t) override;


};

}

#endif