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

template<typename cFM,typename gMM,template <class, class, class> class cLT>
bool pFlow::sphereInteraction<cFM,gMM, cLT>::createSphereInteraction()
{
	
	realVector_D rhoD("densities", this->densities());

	auto modelDict = this->subDict("model");

	forceModel_ = makeUnique<ContactForceModel>(
		this->numMaterials(),
		rhoD.deviceView(),
		modelDict );


	uint32 nPrtcl = sphParticles_.size();

	contactSearch_ = contactSearch::create(
	subDict("contactSearch"),
	sphParticles_.extendedDomain().domainBox(),
	sphParticles_,
	geometryMotion_,
	timers());

	ppContactList_ = makeUnique<ContactListType>(nPrtcl+1);
	
	pwContactList_ = makeUnique<ContactListType>(nPrtcl/5+1);

	return true;
}


template<typename cFM,typename gMM,template <class, class, class> class cLT>
bool pFlow::sphereInteraction<cFM,gMM, cLT>::sphereSphereInteraction()
{
	auto lastItem = ppContactList_().loopCount();

	// create the kernel functor 
	pFlow::sphereInteractionKernels::ppInteractionFunctor 
		ppInteraction(
			this->dt(),
			this->forceModel_(),
			ppContactList_(), // to be read
			sphParticles_.diameter().deviceViewAll(),
			sphParticles_.propertyId().deviceViewAll(),
			sphParticles_.pointPosition().deviceViewAll(),
			sphParticles_.velocity().deviceViewAll(),
			sphParticles_.rVelocity().deviceViewAll(),
			sphParticles_.contactForce().deviceViewAll(),
			sphParticles_.contactTorque().deviceViewAll()
			);
	
	Kokkos::parallel_for(
		"ppInteraction",
		rpPPInteraction(0,lastItem),
		ppInteraction
		);

	Kokkos::fence();	

	return true;
}


template<typename cFM,typename gMM,template <class, class, class> class cLT>
bool pFlow::sphereInteraction<cFM,gMM, cLT>::sphereWallInteraction()
{
	
	uint32 lastItem = pwContactList_().loopCount();
	uint32 iter = this->currentIter();
	real t = this->currentTime();
	real dt = this->dt();

	pFlow::sphereInteractionKernels::pwInteractionFunctor
		pwInteraction(
			dt,
			this->forceModel_(),
			pwContactList_(),
			geometryMotion_.getTriangleAccessor(), 
			geometryMotion_.getModel(iter, t, dt) , 
			sphParticles_.diameter().deviceViewAll() ,
			sphParticles_.propertyId().deviceViewAll(),
			sphParticles_.pointPosition().deviceViewAll(),
			sphParticles_.velocity().deviceViewAll(),
			sphParticles_.rVelocity().deviceViewAll() ,
			sphParticles_.contactForce().deviceViewAll(),
			sphParticles_.contactTorque().deviceViewAll() ,
			geometryMotion_.triMotionIndex().deviceViewAll(),
			geometryMotion_.propertyId().deviceViewAll(),
			geometryMotion_.contactForceWall().deviceViewAll()
		);

	Kokkos::parallel_for(
		"",
		rpPWInteraction(0,lastItem),
		pwInteraction
		);

	
	Kokkos::fence();

	return true;
}


template<typename cFM,typename gMM,template <class, class, class> class cLT>
pFlow::sphereInteraction<cFM,gMM, cLT>::sphereInteraction
(
	systemControl& control,
	const particles& prtcl,
	const geometry& geom
)
:
	interaction(control, prtcl, geom),
	geometryMotion_(dynamic_cast<const GeometryMotionModel&>(geom)),
	sphParticles_(dynamic_cast<const sphereParticles&>(prtcl)),
	boundaryInteraction_(sphParticles_,geometryMotion_),
	ppInteractionTimer_("sphere-sphere interaction", &this->timers()),
	pwInteractionTimer_("sphere-wall interaction", &this->timers()),
	contactListMangementTimer_("contact-list management", &this->timers()),
	boundaryInteractionTimer_("interaction for boundary", &this->timers())
{
	
	if(!createSphereInteraction())
	{
		fatalExit;
	}
}

template<typename cFM,typename gMM,template <class, class, class> class cLT>
bool pFlow::sphereInteraction<cFM,gMM, cLT>::beforeIteration()
{
	return true;
}

template<typename cFM,typename gMM,template <class, class, class> class cLT>
bool pFlow::sphereInteraction<cFM,gMM, cLT>::iterate()
{
	
	auto iter = this->currentIter();
	auto t = this->currentTime();
	auto dt = this->dt();

	
	bool broadSearch = contactSearch_().enterBroadSearch(iter, t, dt);

	sphParticles_.diameter().updateBoundaries(DataDirection::SlaveToMaster);
	sphParticles_.velocity().updateBoundaries(DataDirection::SlaveToMaster);
	sphParticles_.rVelocity().updateBoundaries(DataDirection::SlaveToMaster);
	sphParticles_.mass().updateBoundaries(DataDirection::SlaveToMaster);
	sphParticles_.I().updateBoundaries(DataDirection::SlaveToMaster);
	sphParticles_.propertyId().updateBoundaries(DataDirection::SlaveToMaster);
	
	
	if(broadSearch)
	{
		contactListMangementTimer_.start();
		ppContactList_().beforeBroadSearch();
		pwContactList_().beforeBroadSearch();
		contactListMangementTimer_.pause();
	} 

	for(uint32 i=0; i<6u; i++)
	{
		boundaryInteraction_[i].ppPairs().beforeBroadSearch();
		boundaryInteraction_[i].pwPairs().beforeBroadSearch();
	}
	
	if( sphParticles_.numActive()<=0)return true;
	
	
	if( !contactSearch_().broadSearch(
			iter,
			t,
			dt,		
			ppContactList_(),
			pwContactList_()) )
	{
		fatalErrorInFunction<<
		"unable to perform broadSearch.\n";
		fatalExit;
	}
	
	for(uint32 i=0; i<6u; i++)
	{
		if( !contactSearch_().boundaryBroadSearch(
			i,
			iter,
			t,
			dt,
			boundaryInteraction_[i].ppPairs(),
			boundaryInteraction_[i].pwPairs()))
		{
			fatalErrorInFunction<<
			"failed to perform broadSearch for boundary index "<<i<<endl;
			return false;
		}
	}
	
	if(broadSearch && contactSearch_().performedBroadSearch())
	{
		contactListMangementTimer_.resume();
		ppContactList_().afterBroadSearch();
		pwContactList_().afterBroadSearch();
		contactListMangementTimer_.end();
	}

	for(uint32 i=0; i<6u; i++)
	{
		boundaryInteraction_[i].ppPairs().afterBroadSearch();
		boundaryInteraction_[i].pwPairs().afterBroadSearch();
	}
	
	ppInteractionTimer_.start();
		sphereSphereInteraction();
	ppInteractionTimer_.end();

	
	pwInteractionTimer_.start();
		sphereWallInteraction();
	pwInteractionTimer_.end();

	boundaryInteractionTimer_.start();
	for(uint32 i=0; i<6u; i++)
	{
		boundaryInteraction_[i].sphereSphereInteraction(
			dt, 
			this->forceModel_());
	}
	boundaryInteractionTimer_.end();
	return true;
}

template<typename cFM,typename gMM,template <class, class, class> class cLT>
bool pFlow::sphereInteraction<cFM,gMM, cLT>::afterIteration()
{
	return true;
}

template<typename cFM,typename gMM,template <class, class, class> class cLT>
bool pFlow::sphereInteraction<cFM,gMM, cLT>::hearChanges
(
	real t,
	real dt,
	uint32 iter,
	const message& msg, 
	const anyList& varList
)
{
	if(msg.equivalentTo(message::ITEM_REARRANGE))
	{
		notImplementedFunction;
	}
	return true;
}

