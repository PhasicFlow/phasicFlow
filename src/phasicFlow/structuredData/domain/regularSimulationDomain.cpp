#include <cstring>

#include "regularSimulationDomain.hpp"
#include "processors.hpp"
#include "streams.hpp"

bool pFlow::regularSimulationDomain::createBoundaryDicts()
{
    auto& boundaries = this->subDict("boundaries");
    
    this->addDict("regularBoundaries", boundaries);
    auto& rbBoundaries = this->subDict("regularBoundaries");
    
	real neighborLength = boundaries.getVal<real>("neighborLength");

	for(uint32 i=0; i<sizeOfBoundaries(); i++)
	{
		word bName = bundaryName(i);
		if( !boundaries.containsDictionay(bName) )
		{
			fatalErrorInFunction<<"dictionary "<< bName<<
			"does not exist in "<< boundaries.globalName()<<endl;
			return false;
		}
		auto& bDict = rbBoundaries.subDict(bName);

		if(!bDict.addOrKeep("neighborLength", neighborLength))
		{
			fatalErrorInFunction<<"error in adding neighborLength to "<< bName <<
			"in dictionary "<< boundaries.globalName()<<endl;
			return false;
		}

		if(!bDict.add("mirrorProcessorNo",(uint32) processors::globalRank()))
		{
			fatalErrorInFunction<<"error in adding mirrorProcessorNo to "<< bName <<
			" in dictionary "<< boundaries.globalName()<<endl;
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
	thisDomain_ = domain(globalBox_);
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

bool pFlow::regularSimulationDomain::initialTransferBlockData
(
	span<char> src, 
	span<char> dst,
	size_t sizeOfElement
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

const pFlow::dictionary &pFlow::regularSimulationDomain::thisBoundaryDict() const
{
    return this->subDict("regularBoundaries");
}

bool pFlow::regularSimulationDomain::requiresDataTransfer()const
{
    return false;
}
