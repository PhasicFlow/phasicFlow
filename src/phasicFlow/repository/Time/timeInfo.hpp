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

#ifndef __timeInfo__hpp_
#define __timeInfo__hpp_

#include "types.hpp"

namespace pFlow
{

class timeInfo
{
private:

	uint32 currentIter_;

	real   currentTime_;

	real   dt_;

public:

	timeInfo(uint32 cIter, real cTime, real dt)
	  : currentIter_(cIter),
	    currentTime_(cTime),
	    dt_(dt)
	{
	}

	inline const real& t() const
	{
		return currentTime_;
	}
	inline const real& dt() const
	{
		return dt_;
	}
	inline const uint32& iter() const
	{
		return currentIter_;
	}
};

} // namespace pFlow

#endif //__timeInfo__hpp_