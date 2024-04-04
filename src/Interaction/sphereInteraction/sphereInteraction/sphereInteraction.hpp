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

#ifndef __sphereInteraction_hpp__ 
#define __sphereInteraction_hpp__

#include "interaction.hpp"
#include "sphereParticles.hpp"
#include "boundarySphereInteractionList.hpp"
#include "sphereInteractionKernels.hpp"
//#include "unsortedContactList.hpp"



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

	using MotionModel 			= typename geometryMotionModel::MotionModel;

	using ModelStorage 			= typename ContactForceModel::contactForceStorage;

	using BoundaryListType 		= boundarySphereInteractionList<ContactForceModel,GeometryMotionModel>;
	
	using IdType 			= uint32;

	using IndexType    		= uint32;

	using ContactListType 	= 
			contactListType<ModelStorage, DefaultExecutionSpace, IdType>;

	//using BoundaryContactListType = unsortedContactList<ModelStorage, DefaultExecutionSpace, IdType>;
	


private:
	
	/// const reference to geometry
	const GeometryMotionModel& 			geometryMotion_;

	/// const reference to particles 
	const sphereParticles& 				sphParticles_;

	/// particle-particle and particle-wall interactions at boundaries 
	BoundaryListType 					boundaryInteraction_;	

	/// contact search object for pp and pw interactions 
	uniquePtr<contactSearch> 			contactSearch_ = nullptr;

	/// contact force model 
	uniquePtr<ContactForceModel>  		forceModel_ 	= nullptr;

	/// contact list for particle-particle interactoins (keeps the history)
	uniquePtr<ContactListType> 			ppContactList_ = nullptr;

	/// contact list for particle-wall interactions (keeps the history)
	uniquePtr<ContactListType> 			pwContactList_ = nullptr;


	/// timer for particle-particle interaction computations
	Timer 		ppInteractionTimer_;

	/// timer for particle-wall interaction computations
	Timer       pwInteractionTimer_;

	/// timer for managing contact lists (only inernal points)
	Timer 		contactListMangementTimer_;

	/// timer for boundary interaction time 
	Timer 		boundaryInteractionTimer_;

	

	bool createSphereInteraction();

	bool sphereSphereInteraction();

	bool sphereWallInteraction();

	//bool managePPContactLists();

	//bool managePWContactLists();

	/// range policy for p-p interaction execution 
	using rpPPInteraction = 
		Kokkos::RangePolicy<Kokkos::IndexType<uint32>, Kokkos::Schedule<Kokkos::Dynamic>>;

	/// range policy for p-w interaction execution 
	using rpPWInteraction = rpPPInteraction;

public:

	TypeInfoTemplate13("sphereInteraction", ContactForceModel, MotionModel, ContactListType);

	/// Constructor from components
	sphereInteraction(
		systemControl& control,
		const particles& prtcl,
		const geometry& geom);
	

	/// Add virtual constructor 
	add_vCtor
	(
		interaction,
		sphereInteraction, 
		systemControl
	);

	/// This is called in time loop, before iterate. (overriden from demComponent)
	bool beforeIteration() override;
	
	/// This is called in time loop. Perform the main calculations 
	/// when the component should evolve along time. (overriden from demComponent)
	bool iterate() override; 
	
	/// This is called in time loop, after iterate. (overriden from demComponent)
	bool afterIteration() override;

	/// Check for changes in the point structures. (overriden from observer)
	bool hearChanges(
		real t,
		real dt,
		uint32 iter,
		const message& msg, 
		const anyList& varList)override;
	
	
};


}

#include "sphereInteraction.cpp"

#endif //__sphereInteraction_hpp__
