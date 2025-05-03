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

#include "boundaryList.hpp"
#include "pointStructure.hpp"

void
pFlow::boundaryList::setExtendedDomain()
{
	if (!listSet_)
	{
		fatalErrorInFunction
		  << "boundary list is not set yet and you used the objects." << endl;
		fatalExit;
	}

	realx3 lowerExt = boundary(0).boundaryExtensionLength() +
	                  boundary(2).boundaryExtensionLength() +
	                  boundary(4).boundaryExtensionLength();

	realx3 upperExt = boundary(1).boundaryExtensionLength() +
	                  boundary(3).boundaryExtensionLength() +
	                  boundary(5).boundaryExtensionLength();

	extendedDomain_ = makeUnique<domain>(
		pStruct_.simDomain().extendThisDomain(lowerExt, upperExt));
}

bool
pFlow::boundaryList::resetLists()
{
	clear();
	listSet_ = false;
	return true;
}

bool
pFlow::boundaryList::updateNeighborLists()
{
	if (!listSet_)
	{
		createBoundaries();
	}

	std::array<real, 6> dist;
	dist[0] = boundary(0).neighborLength();
	dist[1] = boundary(1).neighborLength();
	dist[2] = boundary(2).neighborLength();
	dist[3] = boundary(3).neighborLength();
	dist[4] = boundary(4).neighborLength();
	dist[5] = boundary(5).neighborLength();

	pStruct_.updateFlag(extendedDomain_(), dist);

	const auto& maskD = pStruct_.activePointsMaskDevice();
	boundary(0).setSize(maskD.leftSize());
	boundary(1).setSize(maskD.rightSize());
	boundary(2).setSize(maskD.bottomSize());
	boundary(3).setSize(maskD.topSize());
	boundary(4).setSize(maskD.rearSize());
	boundary(5).setSize(maskD.frontSize());

	pStruct_.fillNeighborsLists(
	  boundary(0).indexList().deviceViewAll(),
	  boundary(1).indexList().deviceViewAll(),
	  boundary(2).indexList().deviceViewAll(),
	  boundary(3).indexList().deviceViewAll(),
	  boundary(4).indexList().deviceViewAll(),
	  boundary(5).indexList().deviceViewAll()
	);

	return true;
}

pFlow::boundaryList::boundaryList(pointStructure& pStruct): 
	boundaryListPtr<boundaryBase>(),
    pStruct_(pStruct)
{
	const dictionary& dict= pStruct_.simDomain().thisBoundariesDict();

	neighborListUpdateInterval_ = dict.getValMax(
			"neighborListUpdateInterval",
			1u
		);

	updateInterval_ = dict.getVal<uint32>("updateInterval");
}

bool
pFlow::boundaryList::updateNeighborLists(uint32 iter, bool force)
{
	neighborListUpdate_ = false;
	boundaryUpdate_ = false;
	iterBeforeBoundaryUpdate_ = false;

	if(iter%neighborListUpdateInterval_==0u || iter == 0u || force)
	{
		if(updateNeighborLists())
		{
			neighborListUpdate_ = true;
			lastNeighborUpdated_ = iter;
		}
		else
		{
			fatalErrorInFunction<<"error in updating neighbor lists of boundaries"<<endl;
			return false;
		}
		
	}

	if( iter%updateInterval_ == 0u || iter == 0u || force )
	{
		boundaryUpdate_ = true; 
	}

	if((iter+1)%updateInterval_ == 0u)
	{
		iterBeforeBoundaryUpdate_ = true;
	}
	
	return true;
}

bool
pFlow::boundaryList::createBoundaries()
{
	if (listSet_)
		return true;

	ForAllBoundariesPtr(i, this)
	{
		this->set(
		  i,
		  boundaryBase::create(
		    pStruct_.simDomain().boundaryDict(i),
		    pStruct_.simDomain().boundaryPlane(i),
		    pStruct_,
		    *this,
		    i
		  )
		);

	}
	
	listSet_ = true;
	setExtendedDomain();
	return true;
}

pFlow::box
pFlow::boundaryList::internalDomainBox() const
{
	const auto&  thisBox = pStruct_.thisDomain().domainBox();

	const realx3 lowerPointDisplacement = {
		boundary(0).neighborLengthIntoInternal(),
		boundary(2).neighborLengthIntoInternal(),
		boundary(4).neighborLengthIntoInternal()
	};

	const realx3 upperPointDisplacement = {
		boundary(1).neighborLengthIntoInternal(),
		boundary(3).neighborLengthIntoInternal(),
		boundary(5).neighborLengthIntoInternal()
	};

	return { thisBox.minPoint() + lowerPointDisplacement,
		     thisBox.maxPoint() - upperPointDisplacement };
}

bool
pFlow::boundaryList::beforeIteration(const timeInfo& ti, bool force)
{
	// it is time to update lists
	if(!updateNeighborLists(ti.iter() , force) )
	{
		return false;
	}

	auto callAgain = boundariesMask<6>(true);

	uint32 step = 1;

	while(callAgain.anyElement(true) && step <= 10u)
	{
		ForAllBoundariesPtr(i,this)
		{
			if(callAgain[i])
			{
				if(! boundary(i).beforeIteration(
					step, 
					ti,
					boundaryUpdate_,
					iterBeforeBoundaryUpdate_,
					callAgain[i]))
				{
					fatalErrorInFunction<<"error in performing beforeIteration for boundary"<<
					boundary(i).name()<<" at step "<< step<<endl;
					return false;
				}
			}
		}
		/*for(size_t i=0; i<6ul; i++)
		{

			if(callAgain[i])
			{
				if(! boundary(i).beforeIteration(
					step, 
					ti,
					boundaryUpdate_,
					iterBeforeBoundaryUpdate_,
					callAgain[i]))
				{
					fatalErrorInFunction<<"error in performing beforeIteration for boundary"<<
					boundary(i).name()<<" at step "<< step<<endl;
					return false;
				}
			}			
		}*/

		step++;
	}
	
	ForAllBoundariesPtr(i,this)
	{
		boundary(i).updataBoundaryData(1);
	}
	/*for (auto bdry : *this)
	{
		bdry->updataBoundaryData(1);
	}*/

	ForAllBoundariesPtr(i,this)
	{
		boundary(i).updataBoundaryData(2);
	}
	/*for (auto bdry : *this)
	{
		bdry->updataBoundaryData(2);
	}*/

	return true;
}

bool
pFlow::boundaryList::iterate(const timeInfo& ti, bool force)
{
	ForAllBoundariesPtr(i, this)
	{
		if (!boundary(i).iterate(ti))
		{
			fatalErrorInFunction << "Error in iterate in boundary "
			                     << boundary(i).name() << endl;
			return false;
		}
	}
	/*for (auto& bdry : *this)
	{
		if (!bdry->iterate(ti))
		{
			fatalErrorInFunction << "Error in iterate in boundary "
			                     << bdry->name() << endl;
			return false;
		}
	}*/
	return true;
}

bool
pFlow::boundaryList::afterIteration(const timeInfo& ti, bool force)
{
	
	auto callAgain = boundariesMask<6>(true);

	int step = 1;
	while(callAgain.anyElement(true)&& step <=10)
	{
		ForAllBoundariesPtr(i,this)
		{
			if(callAgain[i])
			{
				if( !boundary(i).transferData(ti.iter(), step, callAgain[i]))
				{
					fatalErrorInFunction<<"Error in transfering data for boundary "<<
					boundary(i).name()<<endl;
					return false;
				}
			}
		}
		step++;
	}

	return true;
}
