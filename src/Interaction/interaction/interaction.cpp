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

#include "interaction.hpp"
#include "particles.hpp"
#include "vocabs.hpp"
#include "geometry.hpp"

pFlow::interaction::interaction
(
	systemControl& control,
	const particles& prtcl,
	const geometry& geom
)
:
	property(interactionFile__, &control.caseSetup()),
	observer
	(
		&prtcl.dynPointStruct(),
		msg_
	),
	demComponent("interaction", control),
	particles_(prtcl),
	geometry_(geom)
{

}

 
pFlow::uniquePtr<pFlow::interaction> pFlow::interaction::create
(
	systemControl& control,
	const particles& prtcl,
	const geometry& geom
)
{
	word shapeTypeName 	= prtcl.shapeTypeName();
	word motionTypeName = geom.motionModelTypeName();
	
	fileSystem file = control.caseSetup().path()+interactionFile__;

	dictionary dict(interactionFile__, file);
	auto interactionDict= dict.subDict("model");

	word clType  = dict.getVal<word>("contactListType");
	word cfModel = interactionDict.getVal<word>("contactForceModel");
	word rfModel = interactionDict.getVal<word>("rollingFrictionModel");


	auto interactionModel = angleBracketsNames3(
			shapeTypeName+"Interaction",
			angleBracketsNames(rfModel,cfModel),
			motionTypeName,
			clType);


	gSettings::sleepMiliSeconds(
			1000*(pFlowProcessors().localSize()-pFlowProcessors().localRank()-1));
	pOutput.space(2)<<"Creating interaction "<<Green_Text(interactionModel)<<" . . ."<<END_REPORT;
	if( systemControlvCtorSelector_.search(interactionModel) )
	{
		auto objPtr = 
			systemControlvCtorSelector_[interactionModel]
			(control, prtcl, geom);	
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< 
			interactionModel << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			systemControlvCtorSelector_
		);
		fatalExit;
	}

	return nullptr;

}