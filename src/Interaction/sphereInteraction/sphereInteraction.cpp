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

template<
	typename contactForceModel,
	typename geometryMotionModel,
	template <class, class, class> class contactListType >
bool pFlow::sphereInteraction<contactForceModel,geometryMotionModel, contactListType>::
	createSphereInteraction()
{
	
	realVector_D rhoD(this->densities());

	auto modelDict = this->fileDict().subDict("model");

	REPORT(1)<<"Createing contact force model . . ."<<endREPORT;
	forceModel_ = makeUnique<ContactForceModel>(
		this->numMaterials(),
		rhoD.deviceVector(),
		modelDict );


	uint32 nPrtcl = sphParticles_.size();

	ppContactList_ = makeUnique<ContactListType>(nPrtcl+1);
	
	pwContactList_ = makeUnique<ContactListType>(nPrtcl/4+1);

	return true;
}



template<
	typename contactForceModel,
	typename geometryMotionModel,
	template <class, class, class> class contactListType >
bool pFlow::sphereInteraction<contactForceModel,geometryMotionModel, contactListType>::
	sphereSphereInteraction()
{
	


	auto lastItem = ppContactList_().loopCount();

	// create the kernel functor 
	pFlow::sphereInteractionKernels::ppInteractionFunctor 
		ppInteraction(
			this->dt(),
			this->forceModel_(),
			ppContactList_(), // to be read
			sphParticles_.diameter().deviceVectorAll(),
			sphParticles_.propertyId().deviceVectorAll(),
			sphParticles_.pointPosition().deviceVectorAll(),
			sphParticles_.velocity().deviceVectorAll(),
			sphParticles_.rVelocity().deviceVectorAll(),
			sphParticles_.contactForce().deviceVectorAll(),
			sphParticles_.contactTorque().deviceVectorAll()
			);
	
	Kokkos::parallel_for(
		"",
		rpPPInteraction(0,lastItem),
		ppInteraction
		);

	Kokkos::fence();	

	return true;
}


template<
	typename contactForceModel,
	typename geometryMotionModel,
	template <class, class, class> class contactListType >
bool pFlow::sphereInteraction<contactForceModel,geometryMotionModel, contactListType>::
	sphereWallInteraction()
{
	
	int32 lastItem = pwContactList_().loopCount();
	real t = this->currentTime();

	pFlow::sphereInteractionKernels::pwInteractionFunctor
		pwInteraction(
			this->dt(),
			this->forceModel_(),
			pwContactList_(),
			geometryMotion_.getTriangleAccessor(), 
			geometryMotion_.getModel(t) , 
			sphParticles_.diameter().deviceVectorAll() ,
			sphParticles_.propertyId().deviceVectorAll(),
			sphParticles_.pointPosition().deviceVectorAll(),
			sphParticles_.velocity().deviceVectorAll(),
			sphParticles_.rVelocity().deviceVectorAll() ,
			sphParticles_.contactForce().deviceVectorAll(),
			sphParticles_.contactTorque().deviceVectorAll() ,
			geometryMotion_.triMotionIndex().deviceVectorAll(),
			geometryMotion_.propertyId().deviceVectorAll(),
			geometryMotion_.contactForceWall().deviceVectorAll()
		);

	Kokkos::parallel_for(
		"",
		rpPWInteraction(0,lastItem),
		pwInteraction
		);

	
	Kokkos::fence();

	return true;
}