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

#include "grainInteraction.hpp"
#include "geometryMotions.hpp"
#include "grainContactForceModels.hpp"
#include "unsortedContactList.hpp"
#include "sortedContactList.hpp"
#include "createBoundaryGrainInteraction.hpp"



#define createInteraction(ForceModel,GeomModel) 	\
													\
	template class pFlow::grainInteraction< 		\
		ForceModel,									\
		GeomModel,									\
		pFlow::unsortedContactList>;				\
													\
	template class pFlow::grainInteraction< 		\
		ForceModel,									\
		GeomModel,									\
		pFlow::sortedContactList>;					\
	createBoundaryGrainInteraction(ForceModel, GeomModel)


createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGAbsoluteLinear<true>>, pFlow::rotationAxisMotionGeometry);
createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGRelativeLinear<true>>, pFlow::rotationAxisMotionGeometry);

createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGAbsoluteLinear<false>>, pFlow::rotationAxisMotionGeometry);
createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGRelativeLinear<false>>, pFlow::rotationAxisMotionGeometry);


createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGAbsoluteLinear<true>>, pFlow::stationaryGeometry);
createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGRelativeLinear<true>>, pFlow::stationaryGeometry);

createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGAbsoluteLinear<false>>, pFlow::stationaryGeometry);
createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGRelativeLinear<false>>, pFlow::stationaryGeometry);


createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGAbsoluteLinear<true>>, pFlow::vibratingMotionGeometry);
createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGRelativeLinear<true>>, pFlow::vibratingMotionGeometry);

createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGAbsoluteLinear<false>>, pFlow::vibratingMotionGeometry);
createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGRelativeLinear<false>>, pFlow::vibratingMotionGeometry);

createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGAbsoluteLinear<true>>, pFlow::conveyorBeltMotionGeometry);
createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGRelativeLinear<true>>, pFlow::conveyorBeltMotionGeometry);

createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGAbsoluteLinear<false>>, pFlow::conveyorBeltMotionGeometry);
createInteraction(pFlow::cfModels::grainRolling<pFlow::cfModels::cGRelativeLinear<false>>, pFlow::conveyorBeltMotionGeometry);