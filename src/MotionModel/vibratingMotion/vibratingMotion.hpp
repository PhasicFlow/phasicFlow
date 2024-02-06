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

#ifndef __vibratingMotion_hpp__
#define __vibratingMotion_hpp__


#include "MotionModel.hpp"
#include "vibrating.hpp"
#include "fileDictionary.hpp"




namespace pFlow
{


/**
 * Vibrating motion model for walls
 * 
 * This class is used for simulaiton that at least one wall components 
 * are moving according to a sinoidal viration defined in class vibrating.
 * One or more than one motion components can be defined in 
 * vibratingMotionInfo dictionary
 * 
\verbatim
// In geometryDict file, this will defines vibrating walls during simulation
...
motionModel vibratingMotion;

vibratingMotionInfo
{
	vibComponent1
	{
		// the definition based on class vibrating
	}
	vibComponent2
	{
		// the definition based on calss vibrating
	}
}
...
\endverbatim
 *
 */
class vibratingMotion
:
    public fileDictionary,
    public MotionModel<vibratingMotion, vibrating>
{

protected:

	bool impl_isMoving()const
	{
		return true;
	}

public:
	
	/// Type info
	TypeInfo("vibratingMotion");
	
	vibratingMotion(const objectFile& objf, repository* owner);

	vibratingMotion(
		const objectFile& objf, 
		const dictionary& dict, 
		repository* owner);

	/// Destructor 
	~vibratingMotion()override = default;

    
	bool write(iOstream& os, const IOPattern& iop)const override;

	static
    auto noneComponent()
    {
      return vibrating();
    }

};

} // pFlow

#endif //__vibratingMotion_hpp__
