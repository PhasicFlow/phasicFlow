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

#include "MPISimulationDomain.hpp"
#include "systemControl.hpp"
#include "rcb1DPartitioning.hpp"
#include "scatteredMasterDistribute.hpp"
#include "scatteredMasterDistributeChar.hpp"

pFlow::MPI::MPISimulationDomain::MPISimulationDomain(systemControl& control)
:
    simulationDomain(control),
    communication_(pFlowProcessors()),
    subDomainsAll_(pFlowProcessors()),
    numPointsAll_(pFlowProcessors()),
    domainPartitioning_( makeUnique<rcb1DPartitioning>(subDict("decomposition"), globalBox()))
{}

bool pFlow::MPI::MPISimulationDomain::createBoundaryDicts()
{
    auto& boundaries = this->subDict("boundaries");
    
    this->addDict("MPIBoundaries", boundaries);
    auto& mpiBoundaries = this->subDict("MPIBoundaries");

    real neighborLength = boundaries.getVal<real>("neighborLength");

    auto neighbors = findPlaneNeighbors();

    for(uint32 i=0; i<sizeOfBoundaries(); i++)
	{
		word bName = bundaryName(i);
		if( !boundaries.containsDictionay(bName) )
		{
			fatalErrorInFunction<<"dictionary "<< bName<<
			"does not exist in "<< boundaries.globalName()<<endl;
			return false;
		}
		auto& bDict = mpiBoundaries.subDict(bName);

		if(!bDict.addOrKeep("neighborLength", neighborLength))
		{
			fatalErrorInFunction<<"error in adding neighborLength to "<< bName <<
			"in dictionary "<< boundaries.globalName()<<endl;
			return false;
		}
        if( thisDomainActive_ )
        {
            if( neighbors[i] == -1 )
            {
                bDict.add("neighborProcessorNo", processors::globalRank());
            }
            else
            {
                bDict.add("neighborProcessorNo", neighbors[i]);
                bDict.addOrReplace("type", "processor");
            }
        }
        else
        {
            bDict.add("neighborProcessorNo", processors::globalRank());
            bDict.addOrReplace("type", "none");
        }
		
        if( bDict.getVal<word>("type") == "periodic")
        {
            fatalErrorInFunction<<
            "periodic is not implemented "<<endl;
            fatalExit;
        }
	}

    return true;
}

bool pFlow::MPI::MPISimulationDomain::setThisDomain()
{
    thisDomain_ = domain(domainPartitioning_->localBox());
    uint32 thisNumPoints = initialNumberInThis();

    if(!communication_.collectAllToAll(thisNumPoints, numPointsAll_))
    {
        fatalErrorInFunction<<
        "Failed to distribute number of points."<<endl;
        return false;
    }
    uint32 allNumPoints = std::accumulate(numPointsAll_.begin(), numPointsAll_.end(), 0u);

    if( thisNumPoints != 0u )
    {
        thisDomainActive_ = true;
    }
    else 
    {
        if(communication_.localMaster()&& allNumPoints == 0u)
            thisDomainActive_ = true;
        else
            thisDomainActive_ = false;
    }

    if( thisDomainActive_ )
    {
        bool allInactive = true;
        for(int32 i=0; i<communication_.localSize(); i++ )
        {
            if(i == communication_.localRank() )continue;
            if(numPointsAll_[i]!=0)
            {
                allInactive = false;
                break;
            }
        }

        if(allInactive)
        {
            thisDomain_ = domain(globalBox());
        }
    }
    
    if(!communication_.collectAllToAll(thisDomain_, subDomainsAll_))
    {
        fatalErrorInFunction<< "Failed to distributed domains"<<endl;
        return false;
    }


    return true;
}

std::vector<int> pFlow::MPI::MPISimulationDomain::findPlaneNeighbors() const
{

    std::vector<int> neighbors(sizeOfBoundaries(),  -2);
    domain gDomain(globalBox());

    // left 
    if( thisDomain_.left().parallelTouch( gDomain.left() ) )
    {
        neighbors[0] = -1;
    }
    
    for(int i=0; i<subDomainsAll_.size(); i++)
    {   
        if(i == subDomainsAll_.rank())continue;

        if( thisDomain_.left().parallelTouch(
            subDomainsAll_[i].right()) )
        {
            neighbors[0] = i;
            break;
        }
    }

    // right 
    if( thisDomain_.right().parallelTouch( gDomain.right() ) )
    {
        neighbors[1] = -1;
    }

    for(int i=0; i<subDomainsAll_.size(); i++)
    {
        
        if(i == subDomainsAll_.rank())continue;

        if( thisDomain_.right().parallelTouch(
            subDomainsAll_[i].left()) )
        {
            neighbors[1] = i;
            break;
        }
    }

    // bottom
    if( thisDomain_.bottom().parallelTouch( gDomain.bottom() ) )
    {
        neighbors[2] = -1;
    }
    
    for(int i=0; i<subDomainsAll_.size(); i++)
    {   
        if(i == subDomainsAll_.rank())continue;

        if( thisDomain_.bottom().parallelTouch(
            subDomainsAll_[i].top()) )
        {
            neighbors[2] = i;
            break;
        }
    }

    // top
    if( thisDomain_.top().parallelTouch( gDomain.top() ) )
    {
        neighbors[3] = -1;
    }
    
    for(int i=0; i<subDomainsAll_.size(); i++)
    {   
        if(i == subDomainsAll_.rank())continue;

        if( thisDomain_.top().parallelTouch(
            subDomainsAll_[i].bottom()) )
        {
            neighbors[3] = i;
            break;
        }
    }

    // rear 
    if( thisDomain_.rear().parallelTouch( gDomain.rear() ) )
    {
        neighbors[4] = -1;
    }
    
    for(int i=0; i<subDomainsAll_.size(); i++)
    {   
        if(i == subDomainsAll_.rank())continue;

        if( thisDomain_.rear().parallelTouch(
            subDomainsAll_[i].front()) )
        {
            neighbors[4] = i;
            break;
        }
    }

    // front
    if( thisDomain_.front().parallelTouch( gDomain.front() ) )
    {
        neighbors[5] = -1;
    }
    
    for(int i=0; i<subDomainsAll_.size(); i++)
    {   
        if(i == subDomainsAll_.rank())continue;

        if( thisDomain_.front().parallelTouch(
            subDomainsAll_[i].rear()) )
        {
            neighbors[5] = i;
            break;
        }
    }
    return neighbors;
}

const pFlow::dictionary &
pFlow::MPI::MPISimulationDomain::thisBoundaryDict() const
{
    return this->subDict("MPIBoundaries");
}

bool pFlow::MPI::MPISimulationDomain::initialUpdateDomains(span<realx3> pointPos)
{
    pFlagTypeHost flags(pointPos.size(), 0 , pointPos.size());
    initialNumPoints_ = pointPos.size();
    if( !domainPartitioning_->partition(pointPos, flags) )
    {
        fatalErrorInFunction<<
        "Point partitioning failed."<<endl;
        return false;
    }
    
    if(!setThisDomain()) return false;

    if(!createBoundaryDicts()) return false;

    return true;
}

pFlow::uint32 pFlow::MPI::MPISimulationDomain::initialNumberInThis() const
{
    uint32 numImport = domainPartitioning_->numberImportThisProc();
    uint32 numExport = domainPartitioning_->numberExportThisProc();
    return max(initialNumPoints_+ numImport - numExport, 0u);
}

bool pFlow::MPI::MPISimulationDomain::initialTransferBlockData
(  
    span<char> src, 
    span<char> dst, 
    size_t sizeOfElement
)const
{
    MPI::scatteredMasterDistribute<char> dataDist(sizeOfElement, pFlowProcessors());
    
    auto lists = domainPartitioning_->allExportLists();
    
    if(!dataDist.setDataMaps( lists ))
    {
        fatalErrorInFunction;
        return false;
    }

    if(!dataDist.distribute(src, dst))
    {
        fatalErrorInFunction<<
        "Error in distribute"<<endl;
        return false;
    }
    return true;
}

bool pFlow::MPI::MPISimulationDomain::initialTransferBlockData
(
    span<realx3> src, 
    span<realx3> dst
)const
{
    
    MPI::scatteredMasterDistribute<realx3> 
        dataDist(pFlowProcessors());
    auto lists = domainPartitioning_->allExportLists();
    
    if(!dataDist.setDataMaps( lists ))
    {
        fatalErrorInFunction;
        return false;
    }
    
    if(!dataDist.distribute(src, dst))
    {
        fatalErrorInFunction<<
        "Error in distribute"<<endl;
        return false;
    }

    return true;
}

bool pFlow::MPI::MPISimulationDomain::initialTransferBlockData
(
    span<real> src, 
    span<real> dst
)const
{
    MPI::scatteredMasterDistribute<real> 
        dataDist(pFlowProcessors());

    auto lists = domainPartitioning_->allExportLists();

    if(!dataDist.setDataMaps( lists ))
    {
        fatalErrorInFunction;
        return false;
    }

    if(!dataDist.distribute(src, dst))
    {
        fatalErrorInFunction<<
        "Error in distribute"<<endl;
        return false;
    }

    return true;
}

bool pFlow::MPI::MPISimulationDomain::initialTransferBlockData
(
    span<uint32> src, 
    span<uint32> dst
)const
{
    MPI::scatteredMasterDistribute<uint32> 
        dataDist(pFlowProcessors());

    auto lists = domainPartitioning_->allExportLists();

    if(!dataDist.setDataMaps( lists ))
    {
        fatalErrorInFunction;
        return false;
    }

    if(!dataDist.distribute(src, dst))
    {
        fatalErrorInFunction<<
        "Error in distribute"<<endl;
        return false;
    }

    return true;
}

bool pFlow::MPI::MPISimulationDomain::initialTransferBlockData
(
    span<int32> src, 
    span<int32> dst
)const
{
    MPI::scatteredMasterDistribute<int32> 
        dataDist(pFlowProcessors());

    auto lists = domainPartitioning_->allExportLists();

    if(!dataDist.setDataMaps( lists ))
    {
        fatalErrorInFunction;
        return false;
    }

    if(!dataDist.distribute(src, dst))
    {
        fatalErrorInFunction<<
        "Error in distribute"<<endl;
        return false;
    }

    return true;
}


pFlow::uint32 pFlow::MPI::MPISimulationDomain::numberToBeImported() const
{
    return domainPartitioning_->numberImportThisProc();
}

pFlow::uint32 pFlow::MPI::MPISimulationDomain::numberToBeExported() const
{
    return domainPartitioning_->numberExportThisProc();
}

bool
pFlow::MPI::MPISimulationDomain::domainActive() const
{
	return thisDomainActive_;
}

const pFlow::domain&
pFlow::MPI::MPISimulationDomain::thisDomain() const
{
	return thisDomain_;
}
