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

#ifndef __stationaryWall_hpp__
#define __stationaryWall_hpp__


#include "MotionModel.hpp"
#include "stationary.hpp"
#include "fileDictionary.hpp"

namespace pFlow
{


/**
 * stationary model for walls
 * 
 * This class is used for simulaiton that all wall components 
 * are fixed.
 * 
\verbatim
// In geometryDict file, this will defines stationary walls
...
motionModel stationary;

// this dictionary is optional
stationaryInfo
{
	
}
...
\endverbatim
 *
 */
class stationaryWall
:
    public fileDictionary,
    public MotionModel<stationaryWall, stationary>
{
protected:

	friend MotionModel<stationaryWall, stationary>;
  
	bool impl_isMoving()const
	{
		return false;
	}

	bool impl_move(uint32, real, real)const
	{
		return true;
	}
	
	void impl_setTime(uint32 ,real ,real )const
	{}

public:

	TypeInfo("stationaryWall");

	stationaryWall(const objectFile& objf, repository* owner);

	stationaryWall(
		const objectFile& objf, 
		const dictionary& dict, 
		repository* owner);


	bool write(iOstream& os, const IOPattern& iop)const override;

	static
    auto noneComponent()
    {
      return stationary();
    }
};

} // pFlow


#endif // __stationaryWall_hpp__