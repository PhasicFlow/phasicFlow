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

#ifndef __interactionBase_hpp__ 
#define __interactionBase_hpp__

#include "interactionTypes.hpp"
#include "particles.hpp"
#include "geometry.hpp"

namespace pFlow
{

class interactionBase
{
public:

	using IndexType    			= CELL_INDEX_TYPE;

	using IdType  				= ID_TYPE;

	using ExecutionSpace 	 	= DefaultExecutionSpace;

protected:

	const particles& 		particles_;

	const geometry& 		geometry_;

public:

	interactionBase(
		const particles& prtcl,
		const geometry& geom)
	:
		particles_(prtcl),
		geometry_(geom)
	{}

	inline
	const auto& pStruct()const
	{
		return particles_.pStruct();
	}

	inline
	const auto& surface()const
	{
		return geometry_.surface();
	}

	inline 
	const auto& Particles()const
	{
		return particles_;
	}

	inline auto& Geometry()const
	{
		return geometry_;
	}
};

}


#endif //__interactionBase_hpp__
