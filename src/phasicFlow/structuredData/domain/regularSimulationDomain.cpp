#include <cstring>

#include "regularSimulationDomain.hpp"
#include "processors.hpp"
#include "streams.hpp"

bool pFlow::regularSimulationDomain::createBoundaryDicts()
{
    auto& boundaries = globalDomainDict_.subDict("boundaries");
    
    globalDomainDict_.addDict("regularSimulationDomainBoundaries", boundaries);
    auto& rbBoundaries = globalDomainDict_.subDict("regularSimulationDomainBoundaries");
    
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

	}
    
    
    return true;
}

bool pFlow::regularSimulationDomain::setThisDomain()
{
	thisDomain_ = domain(globalBox_);
    return true;
}

pFlow::regularSimulationDomain::regularSimulationDomain(
    const dictionary &dict)
    : simulationDomain(dict)
{
}

bool pFlow::regularSimulationDomain::updateDomains
(
	const realx3Vector& pointPos
)
{
	thisNumPoints_ = pointPos.size();
	if(!createBoundaryDicts()) return false;
	if(!setThisDomain()) return false;
	
    return true;
}

pFlow::uint32 pFlow::regularSimulationDomain::thisNumPoints() const
{
	return thisNumPoints_;	
}

bool pFlow::regularSimulationDomain::transferBlockData
(
	span<char> src, 
	span<char> dst,
	uint32 sizeOfBlock
)
{
	size_t requiredSize = sizeOfBlock*thisNumPoints();
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

const pFlow::dictionary &pFlow::regularSimulationDomain::thisBoundaryDict() const
{
    return globalDomainDict_.subDict("regularSimulationDomainBoundaries");
}

bool pFlow::regularSimulationDomain::requiresDataTransfer()const
{
    return false;
}
