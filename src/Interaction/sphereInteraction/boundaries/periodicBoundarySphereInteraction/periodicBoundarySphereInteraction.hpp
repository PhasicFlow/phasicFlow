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

#ifndef __periodicBoundarySphereInteraction_hpp__
#define __periodicBoundarySphereInteraction_hpp__

#include "boundarySphereInteraction.hpp"

namespace pFlow
{

template<typename contactForceModel,typename geometryMotionModel>
class periodicBoundarySphereInteraction
:
    public boundarySphereInteraction<contactForceModel, geometryMotionModel>
{
public:

    using PBSInteractionType = 
        periodicBoundarySphereInteraction<contactForceModel,geometryMotionModel>;

    using BSInteractionType = 
        boundarySphereInteraction<contactForceModel, geometryMotionModel>;

    using GeometryMotionModel 	= typename BSInteractionType::GeometryMotionModel;
	
	using ContactForceModel 	= typename BSInteractionType::ContactForceModel;

	using MotionModel 			= typename geometryMotionModel::MotionModel;

	using ModelStorage 			= typename ContactForceModel::contactForceStorage;
	
	using IdType 			    = typename BSInteractionType::IdType;

	using IndexType    		    = typename BSInteractionType::IndexType;
    
    using ContactListType       = typename BSInteractionType::ContactListType; 

private:

    realx3  transferVec_;
    
    bool    masterInteraction_;
public:

    TypeInfoTemplate22("boundarySphereInteraction", "periodic",ContactForceModel, MotionModel);

	
    periodicBoundarySphereInteraction(
        const boundaryBase& boundary,
		const sphereParticles& sphPrtcls,
		const GeometryMotionModel& geomMotion
    );

    add_vCtor
    (
        BSInteractionType,
        PBSInteractionType,
        boundaryBase
    );
    
    ~periodicBoundarySphereInteraction()override = default;


    

    bool sphereSphereInteraction(
        real dt,
		const ContactForceModel& cfModel)override;
	
};

}

#include "periodicBoundarySphereInteraction.cpp"


#endif //__periodicBoundarySphereInteraction_hpp__