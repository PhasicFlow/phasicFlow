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

#ifndef __rotatingAxisMotion_hpp__
#define __rotatingAxisMotion_hpp__


#include "MotionModel.hpp"
#include "rotatingAxis.hpp"
#include "fileDictionary.hpp"

namespace pFlow
{

/**
 * Rotating axis motion model for walls
 * 
 * This class is used for simulaiton that at least one wall components 
 * is moving according to rotatingAxis motion mode. One or more than one
 * motion components can be defined in rotatingAxisMotionInfo dictionary
 * 
\verbatim
// In geometryDict file, this will defines rotating walls during simulation
...
motionModel rotatingAxis;

rotatingAxisInfo
{
	rotationAxis1
	{
		// the definition based on class rotatingAxis
	}
	rotatoinAxis2
	{
		// the definition based on calss rotatingAxis
	}
}
...
\endverbatim
 *
 */
class rotatingAxisMotion
:
    public fileDictionary,
    public MotionModel<rotatingAxisMotion, rotatingAxis>
{
protected:

	bool impl_isMoving()const
	{
		return true;
	}
	
public:

	TypeInfo("rotatingAxisMotion");

	rotatingAxisMotion(const objectFile& objf, repository* owner);

	rotatingAxisMotion(
		const objectFile& objf, 
		const dictionary& dict, 
		repository* owner);


	bool write(iOstream& os, const IOPattern& iop)const override;

	static
    auto noneComponent()
    {
      return rotatingAxis({0,0,0}, {1,0,0}, 0.0);
    }
};

} // pFlow


#endif // __rotatingAxisMotion_hpp__