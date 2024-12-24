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

#ifndef __conveyorBeltMotion_hpp__
#define __conveyorBeltMotion_hpp__


#include "MotionModel.hpp"
#include "conveyorBelt.hpp"
#include "fileDictionary.hpp"

namespace pFlow
{


/**
 * conveyor belt model for walls
 * 
 * This class is used for simulaiton that wall components are conveyor belt.
 * 
\verbatim

// In geometryDict file, this will defines stationary walls
...
motionModel conveyorBelt;

// this dictionary is optional
conveyorBeltInfo
{
  conveyorBelt1
  {
    // the definition based on class conveyorBelt1
  }
}
...
\endverbatim
 *
 */
class conveyorBeltMotion
:
    public fileDictionary,
    public MotionModel<conveyorBeltMotion, conveyorBelt>
{
protected:

	friend MotionModel<conveyorBeltMotion, conveyorBelt>;
  
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

	TypeInfo("conveyorBeltMotion");

	conveyorBeltMotion(const objectFile& objf, repository* owner);

	conveyorBeltMotion(
		const objectFile& objf, 
		const dictionary& dict, 
		repository* owner);


    using fileDictionary::write;

	bool write(iOstream& os, const IOPattern& iop)const override;

	static
    auto noneComponent()
    {
      return conveyorBelt();
    }
};

} // pFlow


#endif // __conveyorBeltMotion_hpp__