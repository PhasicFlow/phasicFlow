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
#ifndef __boundarySphereInteraction_hpp__
#define __boundarySphereInteraction_hpp__

#include "virtualConstructor.hpp"
#include "generalBoundary.hpp"
#include "unsortedContactList.hpp"
#include "sphereParticles.hpp"

namespace pFlow
{

template<typename contactForceModel,typename geometryMotionModel>
class boundarySphereInteraction
:
    public generalBoundary
{
public:
	
	using BoundarySphereInteractionType
			= boundarySphereInteraction<contactForceModel, geometryMotionModel>;

    using GeometryMotionModel 	= geometryMotionModel;
	
	using ContactForceModel 	= contactForceModel;

	using MotionModel 			= typename geometryMotionModel::MotionModel;

	using ModelStorage 			= typename ContactForceModel::contactForceStorage;
	
	using IdType 			    = uint32;

	using IndexType    		    = uint32;
    
    using ContactListType = 
        unsortedContactList<ModelStorage, DefaultExecutionSpace, IdType>;

private:

	const GeometryMotionModel& 			geometryMotion_;

	/// const reference to sphere particles 
	const sphereParticles& 				sphParticles_;
    
	uniquePtr<ContactListType> 			ppPairs_;

	uniquePtr<ContactListType>			pwPairs_;

public:

	TypeInfoTemplate12("boundarySphereInteraction", ContactForceModel, MotionModel);

	boundarySphereInteraction(
		const boundaryBase& boundary,
		const sphereParticles& sphPrtcls,
		const GeometryMotionModel& geomMotion);

	create_vCtor
	(
		BoundarySphereInteractionType,
		boundaryBase,
		(
			const boundaryBase& boundary,
			const sphereParticles& sphPrtcls,
			const GeometryMotionModel& geomMotion
		),
		(boundary, sphPrtcls, geomMotion)
	);

	add_vCtor
	(
		BoundarySphereInteractionType,
		BoundarySphereInteractionType,
		boundaryBase
	);

	~boundarySphereInteraction()override=default;

	const auto& sphParticles()const
	{
		return sphParticles_;
	}

	const auto& geometryMotion()const
	{
		return geometryMotion_;
	}

	ContactListType& ppPairs()
	{
		return ppPairs_();
	}

	const ContactListType& ppPairs()const
	{
		return ppPairs_();
	}

	ContactListType& pwPairs()
	{
		return pwPairs_();
	}

	const ContactListType& pwPairs()const
	{
		return pwPairs_();
	}

	virtual
	bool sphereSphereInteraction(
		real dt,
		const ContactForceModel& cfModel)
	{
		// for default boundary, no thing to be done 
		return true;
	}

	bool hearChanges
	(
		real t,
		real dt,
		uint32 iter,
		const message& msg, 
    	const anyList& varList
	) override
    {
		
		pOutput<<"Function (hearChanges in boundarySphereInteractions)is not implmented Message "<<
		 msg <<endl<<" name "<< this->name() <<" type "<< this->type()<<endl;;
		//notImplementedFunction;
		return true;
	}

	void fill(const std::any& val)override
	{
		notImplementedFunction;
	}

	static
	uniquePtr<BoundarySphereInteractionType> create(
		const boundaryBase& boundary,
		const sphereParticles& sphPrtcls,
		const GeometryMotionModel& geomMotion
	);

};

}

#include "boundarySphereInteraction.cpp"

#endif //__boundarySphereInteraction_hpp__
