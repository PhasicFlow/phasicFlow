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

#ifndef __sphereInteraction_H__ 
#define __sphereInteraction_H__

#include "interaction.H"
#include "sphereParticles.H"

#include "sphereInteractionKernels.H"

namespace pFlow
{

template<
	typename contactForceModel,
	typename geometryMotionModel,
	template <class, class, class> class contactListType>
class sphereInteraction
:
	public interaction
{
public:

	using GeometryMotionModel 	= geometryMotionModel;
	
	using ContactForceModel 	= contactForceModel;

	using MotionModel 		= typename geometryMotionModel::MotionModel;

	using ModelStorage 		= typename ContactForceModel::contactForceStorage;
	
	using IdType 			= typename interaction::IdType;

	using IndexType    		= typename interaction::IndexType;

	using ExecutionSpace 	= typename interaction::ExecutionSpace;

	using ContactListType 	= 
			contactListType<ModelStorage, ExecutionSpace, IdType>;

	using PairsContainerType= typename contactSearch::PairContainerType;

protected:
	
	/// const reference to geometry
	const GeometryMotionModel& 			geometryMotion_;

	/// const reference to particles 
	const sphereParticles& 				sphParticles_;


	/// contact force model 
	uniquePtr<ContactForceModel>  	forceModel_ 	= nullptr;

	/// contact list for particle-particle interactoins (keeps the history)
	uniquePtr<ContactListType> 		ppContactList_ = nullptr;

	/// contact list for particle-wall interactions (keeps the history)
	uniquePtr<ContactListType> 		pwContactList_ = nullptr;	

	/// timer for particle-particle interaction computations
	Timer 		ppInteractionTimer_;

	/// timer for particle-wall interaction computations
	Timer       pwInteractionTimer_;

	bool createSphereInteraction();

	bool managePPContactLists();

	bool managePWContactLists();

	/// range policy for p-p interaction execution 
	using rpPPInteraction = 
		Kokkos::RangePolicy<Kokkos::IndexType<int32>, Kokkos::Schedule<Kokkos::Dynamic>>;

	/// range policy for p-w interaction execution 
	using rpPWInteraction = rpPPInteraction;

public:

	TypeNameTemplate3("sphereInteraction", ContactForceModel, MotionModel, ContactListType);

	// constructor

	sphereInteraction(
		systemControl& control,
		const particles& prtcl,
		const geometry& geom)
	:
		interaction(control, prtcl, geom),
		geometryMotion_(dynamic_cast<const GeometryMotionModel&>(geom)),
		sphParticles_(dynamic_cast<const sphereParticles&>(prtcl)),
		ppInteractionTimer_("sphere-sphere interaction", &this->timers()),
		pwInteractionTimer_("sphere-wall interaction", &this->timers())
	{
		if(!createSphereInteraction())
		{
			fatalExit;
		}
	}

	add_vCtor
	(
		interaction,
		sphereInteraction, 
		systemControl
	);


	bool beforeIteration() override
	{
		return true;
	}


	bool iterate() override
	{
		
		if(this->contactSearch_)
		{
			if( this->contactSearch_().ppEnterBroadSearch())
			{
				ppContactList_().beforeBroadSearch();
			}

			if(this->contactSearch_().pwEnterBroadSearch())
			{
				pwContactList_().beforeBroadSearch();
			}

			if( !contactSearch_().broadSearch(
					ppContactList_(),
					pwContactList_()) )
			{
				fatalErrorInFunction<<
				"unable to perform broadSearch.\n";
				fatalExit;
			}

			if(this->contactSearch_().ppPerformedBroadSearch())
			{
				ppContactList_().afterBroadSearch();
			}

			if(this->contactSearch_().pwPerformedBroadSearch())
			{
				pwContactList_().afterBroadSearch();
			}
		}

		if( sphParticles_.numActive()<=0)return true;

		ppInteractionTimer_.start();
			sphereSphereInteraction();
		ppInteractionTimer_.end();
		
		pwInteractionTimer_.start();
			sphereWallInteraction();
		pwInteractionTimer_.end();
		
		return true;
	}


	bool afterIteration() override
	{
		return true;
	}


	bool update(const eventMessage& msg)override
	{
		// it requires not action regarding any changes in the 
		// point structure 
		return true;
	}

	bool sphereSphereInteraction();

	bool sphereWallInteraction();
};


}

#include "sphereInteraction.C"

#endif //__sphereInteraction_H__
