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

#include "simulationDomain.hpp"
#include "pFlowProcessors.hpp"
#include "systemControl.hpp"
#include "vocabs.hpp"

bool pFlow::simulationDomain::prepareBoundaryDicts()
{
	
    dictionary& boundaries = this->subDict("boundaries");

	real boundaryExtntionLengthRatio = 
        boundaries.getValOrSetMax("boundaryExtntionLengthRatio", static_cast<real>(0.1));
	
    real neighborLength = boundaries.getValOrSetMax<real>(
		"neighborLength", 
		(1+boundaryExtntionLengthRatio)*maxBoundingSphere_);

	uint32 updateInterval = 
        boundaries.getValOrSet<uint32>("updateInterval", 5u);

	updateInterval = max(updateInterval, 1u);

    uint32 neighborListUpdateInterval = 
        boundaries.getValOrSet("neighborListUpdateInterval", 50u);
	
	neighborListUpdateInterval = max(neighborListUpdateInterval, updateInterval);
    
	boundaries.addOrReplace("neighborListUpdateInterval", neighborListUpdateInterval);
	boundaries.addOrReplace("updateInterval", updateInterval);
	boundaries.addOrReplace("neighborLength", neighborLength);
	boundaries.addOrReplace("boundaryExtntionLengthRatio", boundaryExtntionLengthRatio);

	// create this boundaries dictionary 
	this->addDict(thisBoundariesDictName(), boundaries);
    dictionary& thisBDict = this->subDict(thisBoundariesDictName());
	thisBDict.addOrReplace("neighborLength", neighborLength);
	thisBDict.addOrReplace("boundaryExtntionLengthRatio", boundaryExtntionLengthRatio);
	thisBDict.addOrReplace("updateInterval", updateInterval);
	thisBDict.addOrReplace("neighborListUpdateInterval", neighborListUpdateInterval);


    for(uint32 i=0; i<sizeOfBoundaries(); i++)
	{
		word bName = bundaryName(i);
		if( !boundaries.containsDictionay(bName) )
		{
			fatalErrorInFunction<<"dictionary "<< bName<<
			"does not exist in "<< boundaries.globalName()<<endl;
			return false;
		}
		auto& bDict = thisBDict.subDict(bName);

		if(!bDict.addOrKeep("neighborLength", neighborLength))
		{
			fatalErrorInFunction<<"error in adding neighborLength to "<< bName <<
			"in dictionary "<< boundaries.globalName()<<endl;
			return false;
		}

		if(!bDict.addOrReplace("updateInterval", updateInterval))
		{
			fatalErrorInFunction<<"error in adding updateInterval to "<< bName <<
			"in dictionary "<< boundaries.globalName()<<endl;
            return false;
		}

		bDict.addOrKeep("boundaryExtntionLengthRatio", boundaryExtntionLengthRatio);

	}

    return true;
}

pFlow::simulationDomain::simulationDomain(systemControl &control, real maxBSphere)
    : fileDictionary(
          objectFile(
              domainFile__,
              "",
              objectFile::READ_ALWAYS,
              objectFile::WRITE_NEVER),
          &control.settings()),
      globalBox_(subDict("globalBox")),
	  maxBoundingSphere_(maxBSphere)
{
	if( !prepareBoundaryDicts() )
    {
        fatalErrorInFunction<<
            "Error in preparing dictionaries for boundaries"<<endl;
        fatalExit;
    }
}

pFlow::domain pFlow::simulationDomain::extendThisDomain
(
	const realx3 &lowerPointExtension, 
	const realx3 &upperPointExtension
) const
{
    realx3 minP = thisDomain().minPoint() + lowerPointExtension;
	realx3 maxP = thisDomain().maxPoint() + upperPointExtension;
	return domain({minP, maxP});
}

const pFlow::plane&
pFlow::simulationDomain::boundaryPlane(uint32 i) const
{
	return thisDomain().boundaryPlane(i);
}

pFlow::uniquePtr<pFlow::simulationDomain>
pFlow::simulationDomain::create(systemControl& control, real maxBSphere)
{
	word sType = angleBracketsNames(
        "simulationDomain", 
        pFlowProcessors().localRunTypeName());

	if( systemControlvCtorSelector_.search(sType) )
	{
		return systemControlvCtorSelector_[sType] (control, maxBSphere);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< sType << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			systemControlvCtorSelector_
		);
		fatalExit;
	}
    return nullptr;
}
