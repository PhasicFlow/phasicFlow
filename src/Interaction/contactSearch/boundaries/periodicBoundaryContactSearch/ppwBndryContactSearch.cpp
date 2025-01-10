
#include "ppwBndryContactSearch.hpp"
#include "ppwBndryContactSearchKernels.hpp"
#include "phasicFlowKokkos.hpp"
#include "streams.hpp"

void pFlow::ppwBndryContactSearch::checkAllocateNext(uint32 n)
{
	if( nextCapacity_ < n)
	{
		nextCapacity_ = n;	
		reallocNoInit(next_, n);
	}
}

void pFlow::ppwBndryContactSearch::nullifyHead()
{
	fill(head_, static_cast<uint32>(-1));
}

void pFlow::ppwBndryContactSearch::nullifyNext(uint32 n)
{
	fill(next_, 0u, n, static_cast<uint32>(-1));
}

void pFlow::ppwBndryContactSearch::buildList(
	const deviceScatteredFieldAccess<realx3> &points)
{
	if(points.empty())return;
	uint32 n = points.size();
	checkAllocateNext(n);
	nullifyNext(n);
	nullifyHead();
	
	pFlow::pweBndryContactSearchKernels::buildNextHead(
		points,
		searchCells_,
		head_,
		next_
	);
}

pFlow::ppwBndryContactSearch::ppwBndryContactSearch
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

bool pFlow::ppwBndryContactSearch::broadSearchPP
(
	csPairContainerType &ppPairs, 
	const deviceScatteredFieldAccess<realx3> &points, 
	const deviceScatteredFieldAccess<real>& diams, 
	const deviceScatteredFieldAccess<realx3> &mirrorPoints, 
	const deviceScatteredFieldAccess<real>& mirrorDiams, 
	const realx3& transferVec
)
{
	
	buildList(points);

	uint32 nNotInserted = 1;

	// loop until the container size fits the numebr of contact pairs
	while (nNotInserted > 0)
	{

		nNotInserted = pFlow::pweBndryContactSearchKernels::broadSearchPP
		(
			ppPairs,
			points,
			diams,
			mirrorPoints,
			mirrorDiams,
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
			oldCap << " to "<<ppPairs.capacity()<<" in peiodicBoundaryContactSearch."<<END_INFO;
			
		}

	}	
	
    return true;
}
