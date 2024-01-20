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

pFlow::MPISimulationDomain::MPISimulationDomain(systemControl& control)
:
    simulationDomain(control),
    communication_(pFlowProcessors()),
    subDomains_(pFlowProcessors()),
    domainPartition_( makeUnique<rcb1DPartitioning>(subDict("decomposition"), globalBox_))
{}

bool pFlow::MPISimulationDomain::createBoundaryDicts()
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

		if( neighbors[i] == -1 )
        {
            bDict.add("mirrorProcessorNo", processors::globalRank());
        }
        else
        {
            bDict.add("mirrorProcessorNo", neighbors[i]);
            bDict.addOrReplace("type", "processor");
        }
	}

    return true;
}

bool pFlow::MPISimulationDomain::setThisDomain()
{
    thisDomain_ = domain(domainPartition_->localBox());
    if(!communication_.collectAllToAll(thisDomain_, subDomains_))
    {
        fatalErrorInFunction<< "Failed to distributed domains"<<endl;
        return false;
    }

    return true;
}

std::vector<int> pFlow::MPISimulationDomain::findPlaneNeighbors() const
{

    std::vector<int> neighbors(sizeOfBoundaries(),  -2);
    domain gDomain(globalBox_);

    // left 
    if( thisDomain_.left().parallelTouch( gDomain.left() ) )
    {
        neighbors[0] = -1;
    }
    
    for(int i=0; i<sizeOfBoundaries(); i++)
    {   
        if(i == subDomains_.rank())continue;

        if( thisDomain_.left().parallelTouch(
            subDomains_[i].right()) )
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

    for(int i=0; i<sizeOfBoundaries(); i++)
    {
        
        if(i == subDomains_.rank())continue;

        if( thisDomain_.right().parallelTouch(
            subDomains_[i].left()) )
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
    
    for(int i=0; i<sizeOfBoundaries(); i++)
    {   
        if(i == subDomains_.rank())continue;

        if( thisDomain_.bottom().parallelTouch(
            subDomains_[i].top()) )
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
    
    for(int i=0; i<sizeOfBoundaries(); i++)
    {   
        if(i == subDomains_.rank())continue;

        if( thisDomain_.top().parallelTouch(
            subDomains_[i].bottom()) )
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
    
    for(int i=0; i<sizeOfBoundaries(); i++)
    {   
        if(i == subDomains_.rank())continue;

        if( thisDomain_.rear().parallelTouch(
            subDomains_[i].front()) )
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
    
    for(int i=0; i<sizeOfBoundaries(); i++)
    {   
        if(i == subDomains_.rank())continue;

        if( thisDomain_.front().parallelTouch(
            subDomains_[i].rear()) )
        {
            neighbors[5] = i;
            break;
        }
    }
    return neighbors;
}

const pFlow::dictionary &
pFlow::MPISimulationDomain::thisBoundaryDict() const
{
    return this->subDict("MPIBoundaries");
}

bool pFlow::MPISimulationDomain::initialUpdateDomains(span<realx3> pointPos)
{
    pFlagTypeHost flags(pointPos.size(), 0 , pointPos.size());
    initialNumPoints_ = pointPos.size();
    if( !domainPartition_->partition(pointPos, flags) )
    {
        return false;
    }
    
    if(!setThisDomain()) return false;
    if(!createBoundaryDicts()) return false;

    return true;
}

pFlow::uint32 pFlow::MPISimulationDomain::initialNumberInThis() const
{
    uint32 numImport = domainPartition_->numberImportThisProc();
    uint32 numExport = domainPartition_->numberExportThisProc();
    return initialNumPoints_+ numImport - numExport;;
}

bool pFlow::MPISimulationDomain::initialTransferBlockData
(  
    span<char> src, 
    span<char> dst, 
    size_t sizeOfElement
)const
{
    MPI::scatteredMasterDistribute<char> dataDist(sizeOfElement, pFlowProcessors());
    
    auto lists = domainPartition_->allExportLists();
    
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

bool pFlow::MPISimulationDomain::initialTransferBlockData
(
    span<realx3> src, 
    span<realx3> dst
)const
{
    
    MPI::scatteredMasterDistribute<realx3> 
        dataDist(pFlowProcessors());
    auto lists = domainPartition_->allExportLists();

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

bool pFlow::MPISimulationDomain::initialTransferBlockData
(
    span<real> src, 
    span<real> dst
)const
{
    MPI::scatteredMasterDistribute<real> 
        dataDist(pFlowProcessors());

    auto lists = domainPartition_->allExportLists();

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

bool pFlow::MPISimulationDomain::initialTransferBlockData
(
    span<uint32> src, 
    span<uint32> dst
)const
{
    MPI::scatteredMasterDistribute<uint32> 
        dataDist(pFlowProcessors());

    auto lists = domainPartition_->allExportLists();

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

bool pFlow::MPISimulationDomain::initialTransferBlockData
(
    span<int32> src, 
    span<int32> dst
)const
{
    MPI::scatteredMasterDistribute<int32> 
        dataDist(pFlowProcessors());

    auto lists = domainPartition_->allExportLists();

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

/*bool pFlow::MPISimulationDomain::updateDomains(
    span<realx3> pointPos,
    pFlagTypeHost flags)
{
    if( !domainPartition_->partition(pointPos, flags) )
    {
        return false;
    }
        
    if(!setThisDomain()) return false;
    if(!createBoundaryDicts()) return false;
	
    return true;
}*/

pFlow::uint32 pFlow::MPISimulationDomain::numberToBeImported() const
{
    return domainPartition_->numberImportThisProc();
}

pFlow::uint32 pFlow::MPISimulationDomain::numberToBeExported() const
{
    return domainPartition_->numberExportThisProc();
}



bool pFlow::MPISimulationDomain::requiresDataTransfer() const
{
    notImplementedFunction;
    return false;
}
