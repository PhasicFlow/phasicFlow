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
#include "createBoundarySphereInteraction.hpp"



#define createInteraction(ForceModel,GeomModel) 	\
													\
	template class pFlow::sphereInteraction< 		\
		ForceModel,									\
		GeomModel,									\
		pFlow::unsortedContactList>;				\
													\
	template class pFlow::sphereInteraction< 		\
		ForceModel,									\
		GeomModel,									\
		pFlow::sortedContactList>;					\
	createBoundarySphereInteraction(ForceModel, GeomModel)


// stationaryGeometry
createInteraction(pFlow::cfModels::limitedNonLinearNormalRolling, pFlow::stationaryGeometry);
createInteraction(pFlow::cfModels::nonLimitedNonLinearNormalRolling,pFlow::stationaryGeometry);

// rotationAxisMotionGeometry
createInteraction(pFlow::cfModels::limitedNonLinearNormalRolling, pFlow::rotationAxisMotionGeometry);
createInteraction(pFlow::cfModels::nonLimitedNonLinearNormalRolling,pFlow::rotationAxisMotionGeometry);

// vibratingMotionGeometry
createInteraction(pFlow::cfModels::limitedNonLinearNormalRolling, pFlow::vibratingMotionGeometry);
createInteraction(pFlow::cfModels::nonLimitedNonLinearNormalRolling,pFlow::vibratingMotionGeometry);

// conveyorBeltMotionGeometry
createInteraction(pFlow::cfModels::limitedNonLinearNormalRolling, pFlow::conveyorBeltMotionGeometry);
createInteraction(pFlow::cfModels::nonLimitedNonLinearNormalRolling,pFlow::conveyorBeltMotionGeometry);

// multiRotationAxisMotionGeometry
createInteraction(pFlow::cfModels::limitedNonLinearNormalRolling, pFlow::multiRotationAxisMotionGeometry);
createInteraction(pFlow::cfModels::nonLimitedNonLinearNormalRolling,pFlow::multiRotationAxisMotionGeometry);
