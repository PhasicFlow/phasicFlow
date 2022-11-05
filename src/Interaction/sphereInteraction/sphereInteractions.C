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

#include "sphereInteraction.H"

#include "geometryMotions.H"

#include "contactForceModels.H"
#include "unsortedContactList.H"
#include "sortedContactList.H"

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedLinearNormalRolling,
	pFlow::fixedGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedLinearNormalRolling,
	pFlow::fixedGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedLinearNormalRolling,
	pFlow::fixedGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedLinearNormalRolling,
	pFlow::fixedGeometry,
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



/// non-linear models 

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearNormalRolling,
	pFlow::fixedGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearNormalRolling,
	pFlow::fixedGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearNormalRolling,
	pFlow::fixedGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearNormalRolling,
	pFlow::fixedGeometry,
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


// - nonLinearMod models
	template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearModNormalRolling,
	pFlow::fixedGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::limitedNonLinearModNormalRolling,
	pFlow::fixedGeometry,
	pFlow::sortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearModNormalRolling,
	pFlow::fixedGeometry,
	pFlow::unsortedContactList>;

template class pFlow::sphereInteraction<
	pFlow::cfModels::nonLimitedNonLinearModNormalRolling,
	pFlow::fixedGeometry,
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


	

