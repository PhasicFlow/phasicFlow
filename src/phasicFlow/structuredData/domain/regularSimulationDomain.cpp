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

#include "regularSimulationDomain.hpp"
#include "processors.hpp"
#include "streams.hpp"

bool pFlow::regularSimulationDomain::createBoundaryDicts()
{

	dictionary& thisBoundaries = this->subDict(thisBoundariesDictName());

	for(uint32 i=0; i<sizeOfBoundaries(); i++)
	{
		word bName = bundaryName(i);
		dictionary& bDict = thisBoundaries.subDict(bName);

		if(!bDict.add("neighborProcessorNo",(uint32) processors::globalRank()))
		{
			fatalErrorInFunction<<"error in adding neighborProcessorNo to "<< bName <<
			" in dictionary "<< thisBoundaries.globalName()<<endl;
			return false;
		}

		auto bType = bDict.getVal<word>("type");
		uint32 mirrorIndex = 0;

		if(bType == "periodic")
		{
			if(bName == bundaryName(0)) mirrorIndex = 1;
			if(bName == bundaryName(1)) mirrorIndex = 0;
			if(bName == bundaryName(2)) mirrorIndex = 3;
			if(bName == bundaryName(3)) mirrorIndex = 2;
			if(bName == bundaryName(4)) mirrorIndex = 5;
			if(bName == bundaryName(5)) mirrorIndex = 4;

			if(! bDict.addOrReplace("mirrorBoundaryIndex", mirrorIndex))
			{
				fatalErrorInFunction<<
				"canno add entry mirroBoundaryIndex to dictionary "<<
				bDict.globalName()<<endl;
				return false;
			}
		}
	}
    
    return true;
}

bool pFlow::regularSimulationDomain::setThisDomain()
{
	thisDomain_ = domain(globalBox());
    return true;
}

pFlow::regularSimulationDomain::regularSimulationDomain
(
    systemControl& control
)
:
    simulationDomain(control)
{}

bool pFlow::regularSimulationDomain::initialUpdateDomains(span<realx3> pointPos)
{
    initialNumPoints_ = pointPos.size();
    if(!setThisDomain()) return false;
    if(!createBoundaryDicts()) return false;
    return true;
}

pFlow::uint32 pFlow::regularSimulationDomain::initialNumberInThis() const
{
    return initialNumPoints_;
}

/*bool pFlow::regularSimulationDomain::updateDomains(
    span<realx3> pointPos,
    pFlagTypeHost flags)
{	
    return true;
}*/

pFlow::uint32 pFlow::regularSimulationDomain::numberToBeImported() const
{
	return 0;	
}

pFlow::uint32 pFlow::regularSimulationDomain::numberToBeExported() const
{
    return 0;
}

bool
pFlow::regularSimulationDomain::domainActive() const
{
	return true;
}

const pFlow::domain&
pFlow::regularSimulationDomain::thisDomain() const
{
	return thisDomain_;
}

bool
pFlow::regularSimulationDomain::initialTransferBlockData(
  span<char> src,
  span<char> dst,
  size_t     sizeOfElement
) const
{
	size_t requiredSize = sizeOfElement*initialNumberInThis();
	if(dst.size() < requiredSize)
	{
		fatalErrorInFunction<< 
		"size of destination data block [in byte]"<< dst.size()<<
		"is smaller than the required size [in byte]"<< requiredSize<<endl;
		return false;
	}
	
	if(src.size() < requiredSize )
	{	
		fatalErrorInFunction<< 
		"size of source data block [in byte]"<< src.size()<<
		"is smaller than the required size [in byte]"<< requiredSize<<endl;
		return false;

	}

	std::memcpy(dst.data(), src.data(), requiredSize);

    return true;
}

bool pFlow::regularSimulationDomain::initialTransferBlockData
(
    span<realx3> src, 
    span<realx3> dst
) const
{
    return initialTransferBlockData(
        charSpan(src), 
        charSpan(dst), 
        sizeof(realx3)); 
}

bool pFlow::regularSimulationDomain::initialTransferBlockData
(
    span<real> src, 
    span<real> dst
) const
{
    return initialTransferBlockData(
        charSpan(src), 
        charSpan(dst), 
        sizeof(real)); 
}

bool pFlow::regularSimulationDomain::initialTransferBlockData
(
    span<uint32> src, 
    span<uint32> dst
) const
{
    return initialTransferBlockData(
        charSpan(src), 
        charSpan(dst), 
        sizeof(uint32)); 
}

bool pFlow::regularSimulationDomain::initialTransferBlockData
(
    span<int32> src, 
    span<int32> dst
) const
{
    return initialTransferBlockData(
        charSpan(src), 
        charSpan(dst), 
        sizeof(int32)); 
}
