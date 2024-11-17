#include "boundarySphereInteraction.hpp"
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

template <typename cFM, typename gMM>
void pFlow::boundaryGrainInteraction<cFM, gMM>::allocatePPPairs()
{
	ppPairs_.reset(nullptr);
	ppPairs_ = makeUnique<ContactListType>(1);
}

template <typename cFM, typename gMM>
void pFlow::boundaryGrainInteraction<cFM, gMM>::allocatePWPairs()
{
	pwPairs_.reset(nullptr);
	pwPairs_ = makeUnique<ContactListType>(1);
}


template <typename cFM, typename gMM>
pFlow::boundaryGrainInteraction<cFM, gMM>::boundaryGrainInteraction(
	const boundaryBase &boundary,
	const grainParticles &grnPrtcls,
	const GeometryMotionModel &geomMotion)
	: generalBoundary(boundary, grnPrtcls.pStruct(), "", ""),
	  geometryMotion_(geomMotion),
	  grnParticles_(grnPrtcls)
{
}

template <typename cFM, typename gMM>
pFlow::uniquePtr<pFlow::boundaryGrainInteraction<cFM, gMM>>
pFlow::boundaryGrainInteraction<cFM, gMM>::create(
	const boundaryBase &boundary,
	const grainParticles &grnPrtcls,
	const GeometryMotionModel &geomMotion)
{
	word cfTypeName = ContactForceModel::TYPENAME();
	word gmTypeName = MotionModel::TYPENAME();
	word bType = boundary.type();

	word boundaryTypeName = angleBracketsNames3(
		"boundaryGrainInteraction",
		bType,
		cfTypeName,
		gmTypeName);

	word altBTypeName = angleBracketsNames2(
		"boundaryGrainInteraction",
		cfTypeName,
		gmTypeName);

	if (boundaryBasevCtorSelector_.search(boundaryTypeName))
	{
		pOutput.space(4) << "Creating boundry type " << Green_Text(boundaryTypeName) << 
		" for boundary " << boundary.name() << " . . ." << END_REPORT;
		return boundaryBasevCtorSelector_[boundaryTypeName](
			boundary,
			grnPrtcls,
			geomMotion);
	}
	else if(boundaryBasevCtorSelector_[altBTypeName])
	{
		// if boundary condition is not implemented, the default is used
		
		pOutput.space(4) << "Creating boundry type " << Green_Text(altBTypeName) << 
		" for boundary " << boundary.name() << " . . ." << END_REPORT;
		return boundaryBasevCtorSelector_[altBTypeName](
			boundary,
			grnPrtcls,
			geomMotion);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< boundaryTypeName<<
			" and "<< altBTypeName << " do not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			boundaryBasevCtorSelector_
		);
		fatalExit;
	}

	return nullptr;
}
