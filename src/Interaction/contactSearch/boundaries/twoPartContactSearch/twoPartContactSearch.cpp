
#include "twoPartContactSearch.hpp"
#include "twoPartContactSearchKernels.hpp"
#include "phasicFlowKokkos.hpp"
#include "streams.hpp"

void pFlow::twoPartContactSearch::checkAllocateNext(uint32 n)
{
	if( nextCapacity_ < n)
	{
		nextCapacity_ = n;	
		reallocNoInit(next_, n);
	}
}

void pFlow::twoPartContactSearch::nullifyHead()
{
	fill(head_, static_cast<uint32>(-1));
}

void pFlow::twoPartContactSearch::nullifyNext(uint32 n)
{
	fill(next_, 0u, n, static_cast<uint32>(-1));
}

void pFlow::twoPartContactSearch::buildList(
	const deviceScatteredFieldAccess<realx3> &points)
{
	if(points.empty())return;
	uint32 n = points.size();
	checkAllocateNext(n);
	nullifyNext(n);
	nullifyHead();
	
	pFlow::twoPartContactSearchKernels::buildNextHead(
		points,
		searchCells_,
		head_,
		next_
	);
}

pFlow::twoPartContactSearch::twoPartContactSearch
(
    const box &domain,
    real cellSize,
	real sizeRatio
)
:
	searchCells_(domain, cellSize),
	head_("periodic:head",searchCells_.nx(), searchCells_.ny(), searchCells_.nz()),
	sizeRatio_(sizeRatio)
{

}

bool pFlow::twoPartContactSearch::broadSearchPP
(
	csPairContainerType &ppPairs, 
	const deviceScatteredFieldAccess<realx3> &points1, 
	const deviceScatteredFieldAccess<real>& diams1, 
	const deviceScatteredFieldAccess<realx3> &points2, 
	const deviceScatteredFieldAccess<real>& diams2, 
	const realx3& transferVec
)
{
	if(points1.empty())return true;
	if(points2.empty()) return true;

	buildList(points1);

	uint32 nNotInserted = 1;

	// loop until the container size fits the numebr of contact pairs
	while (nNotInserted > 0)
	{

		nNotInserted = pFlow::twoPartContactSearchKernels::broadSearchPP
		(
			ppPairs,
			points1,
			diams1,
			points2,
			diams2,
			transferVec,
			head_,
			next_,
			searchCells_,
			sizeRatio_
		);
		
	
		if(nNotInserted)
		{
			// - resize the container
			//   note that getFull now shows the number of failed insertions.
			uint32 len = max(nNotInserted,100u) ;
			
			auto oldCap = ppPairs.capacity();
			
			ppPairs.increaseCapacityBy(len);

			INFORMATION<< "Particle-particle contact pair container capacity increased from "<<
			oldCap << " to "<<ppPairs.capacity()<<" in contact search in boundary region."<<END_INFO;
			
		}

	}	
	
    return true;
}

bool pFlow::twoPartContactSearch::broadSearchPP
(
	csPairContainerType &ppPairs,
	const deviceScatteredFieldAccess<realx3> &points1,
	const deviceScatteredFieldAccess<real> &diams1,
	const realx3Vector_D& points2,
	const realVector_D& diams2,
	const word& name
)
{
	buildList(points1);

	uint32 nNotInserted = 1;

	// loop until the container size fits the numebr of contact pairs
	while (nNotInserted > 0)
	{

		nNotInserted = pFlow::twoPartContactSearchKernels::broadSearchPP
		(
			ppPairs,
			points1,
			diams1,
			points2,
			diams2,
			head_,
			next_,
			searchCells_,
			sizeRatio_
		);
		
	
		if(nNotInserted)
		{
			// - resize the container
			//   note that getFull now shows the number of failed insertions.
			uint32 len = max(nNotInserted,100u) ;
			
			auto oldCap = ppPairs.capacity();
			
			ppPairs.increaseCapacityBy(len);
			
			INFORMATION<< "Particle-particle contact pair container capacity increased from "<<
			oldCap << " to "<<ppPairs.capacity()<<" in boundary contact search in "<< name <<END_INFO;
			
		}

	}	
	
    return true;
}
