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

#ifndef __multiRotatingAxisMotion_hpp__
#define __multiRotatingAxisMotion_hpp__


#include "MotionModel.hpp"
#include "multiRotatingAxis.hpp"
#include "fileDictionary.hpp"


namespace pFlow
{

// forward
// class dictionary;

/**
 * Rotating axis motion model for walls
 * 
 * This class is used for simulaiton that at least one wall components 
 * is moving according to multiRotatingAxis motion mode. One or more than one
 * motion components can be defined in multiRotatingAxisMotionInfo dictionary
 * 
\verbatim
// In geometryDict file, this will defines multi-rotating walls during simulation
...
motionModel multiRotatingAxisMotion;

multiRotatingAxisMotionInfo
{
	rotationAxis1
	{
		// the definition based on class multiRotatingAxis
	}
	rotatoinAxis2
	{
		// the definition based on calss multiRotatingAxis
	}
}
...
\endverbatim
 *
 */
class multiRotatingAxisMotion
:
    public fileDictionary,
    public MotionModel<multiRotatingAxisMotion, multiRotatingAxis>
{
protected:

	VectorSingle<int32> 	sortedIndex_;

	friend MotionModel<multiRotatingAxisMotion, multiRotatingAxis>;

	/// is the geometry attached to this component moving
	bool impl_isMoving()const
	{
		return true;
	}



	/// Read from dictionary 
	bool impl_readDictionary(const dictionary& dict);

	bool impl_writeDictionary(dictionary& dict)const;

public:
	
    TypeInfo("multiRotatingAxisMotion");

	multiRotatingAxisMotion(const objectFile& objf, repository* owner);

	multiRotatingAxisMotion(
		const objectFile& objf, 
		const dictionary& dict, 
		repository* owner);

    using fileDictionary::write;

	bool write(iOstream& os, const IOPattern& iop)const override;

	static
    multiRotatingAxis noneComponent()
    {
        return multiRotatingAxis();
    }

    // TODO: make this method protected
    void impl_setTime(uint32 iter, real t, real dt)const;

	/// move the component itself
	bool impl_move(uint32 iter, real t, real dt)const;
};

} // pFlow

#endif //__multiRotatingAxisMotion_hpp__
