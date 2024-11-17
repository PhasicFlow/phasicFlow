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
#ifndef __boundaryGrainInteraction_hpp__
#define __boundaryGrainInteraction_hpp__

#include "virtualConstructor.hpp"
#include "generalBoundary.hpp"
#include "sortedContactList.hpp"
#include "grainParticles.hpp"

namespace pFlow
{

template<typename contactForceModel,typename geometryMotionModel>
class boundaryGrainInteraction
:
    public generalBoundary
{
public:
	
	using BoundaryGrainInteractionType
			= boundaryGrainInteraction<contactForceModel, geometryMotionModel>;

    using GeometryMotionModel 	= geometryMotionModel;
	
	using ContactForceModel 	= contactForceModel;

	using MotionModel 			= typename geometryMotionModel::MotionModel;

	using ModelStorage 			= typename ContactForceModel::contactForceStorage;
	
	using IdType 			    = uint32;

	using IndexType    		    = uint32;
    
    using ContactListType = 
        sortedContactList<ModelStorage, DefaultExecutionSpace, IdType>;

private:

	const GeometryMotionModel& 			geometryMotion_;

	/// const reference to sphere particles 
	const grainParticles& 				grnParticles_;
    
	uniquePtr<ContactListType> 			ppPairs_ = nullptr;

	uniquePtr<ContactListType>			pwPairs_ = nullptr;

protected:

	void allocatePPPairs();

	void allocatePWPairs();

public:

	TypeInfoTemplate12("boundaryGrainInteraction", ContactForceModel, MotionModel);

	boundaryGrainInteraction(
		const boundaryBase& boundary,
		const grainParticles& grnPrtcls,
		const GeometryMotionModel& geomMotion);

	create_vCtor
	(
		BoundaryGrainInteractionType,
		boundaryBase,
		(
			const boundaryBase& boundary,
			const grainParticles& grnPrtcls,
			const GeometryMotionModel& geomMotion
		),
		(boundary, grnPrtcls, geomMotion)
	);

	add_vCtor
	(
		BoundaryGrainInteractionType,
		BoundaryGrainInteractionType,
		boundaryBase
	);

	~boundaryGrainInteraction()override=default;

	const auto& grnParticles()const
	{
		return grnParticles_;
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

	bool ppPairsAllocated()const
	{
		if( ppPairs_)return true;
		return false;
	}

	bool pwPairsAllocated()const
	{
		if( pwPairs_)return true;
		return false;
	}

	virtual
	bool grainGrainInteraction(
		real dt,
		const ContactForceModel& cfModel,
		uint32 step)
	{
		// for default boundary, no thing to be done 
		return false;
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
		 msg <<endl<<" name "<< this->boundaryName() <<" type "<< this->type()<<endl;;
		//notImplementedFunction;
		return true;
	}

	static
	uniquePtr<BoundaryGrainInteractionType> create(
		const boundaryBase& boundary,
		const grainParticles& sphPrtcls,
		const GeometryMotionModel& geomMotion
	);

};

}

#include "boundaryGrainInteraction.cpp"

#endif //__boundaryGrainInteraction_hpp__
