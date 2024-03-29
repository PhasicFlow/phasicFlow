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

#include "sphereInteraction.hpp"

#include "geometryMotions.hpp"

#include "contactForceModels.hpp"
#include "unsortedContactList.hpp"
#include "sortedContactList.hpp"

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedLinearNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedLinearNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedLinearNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedLinearNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedLinearNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedLinearNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedLinearNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedLinearNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::sortedContactList>;


template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedLinearNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedLinearNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedLinearNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedLinearNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::sortedContactList>;


/*template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedLinearNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedLinearNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedLinearNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedLinearNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::sortedContactList>;*/

/// non-linear models 

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::sortedContactList>;


template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::sortedContactList>;

/*template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::sortedContactList>;*/


// - nonLinearMod models
template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearModNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearModNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearModNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearModNormalRolling,
	pFlow::stationaryGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearModNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearModNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearModNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearModNormalRolling,
	pFlow::rotationAxisMotionGeometry,
	pFlow::sortedContactList>;


template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearModNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearModNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearModNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearModNormalRolling,
	pFlow::vibratingMotionGeometry,
	pFlow::sortedContactList>;


/*template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearModNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearModNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearModNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearModNormalRolling,
	pFlow::multiRotationAxisMotionGeometry,
	pFlow::sortedContactList>;
*/