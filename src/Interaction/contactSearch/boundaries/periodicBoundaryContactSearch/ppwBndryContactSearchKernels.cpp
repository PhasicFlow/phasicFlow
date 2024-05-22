#include "ppwBndryContactSearchKernels.hpp"

INLINE_FUNCTION_HD
bool sphereSphereCheckB(const pFlow::realx3& p1, const pFlow::realx3 p2, pFlow::real d1, pFlow::real d2)
{
	return pFlow::length(p2-p1) < 0.5*(d2+d1);
}

void pFlow::pweBndryContactSearchKernels::buildNextHead
(
    const deviceScatteredFieldAccess<realx3> &points,
    const cells              &searchCells,
    deviceViewType3D<uint32> &head, 
    deviceViewType1D<uint32> &next 
)
{
    if(points.empty())return;
    
    uint32 n= points.size();

    Kokkos::parallel_for(
		"pFlow::ppwBndryContactSearch::buildList",
		deviceRPolicyStatic(0,n),
		LAMBDA_HD(uint32 i)
		{
			int32x3 ind;
			if( searchCells.pointIndexInDomain(points[i], ind) )
			{
				// discards points out of searchCell
				uint32 old = Kokkos::atomic_exchange(&head(ind.x(), ind.y(), ind.z()), i);
				next[i] = old;
			}
		}
	);
	Kokkos::fence();
}

pFlow::uint32 pFlow::pweBndryContactSearchKernels::broadSearchPP
(
    csPairContainerType &ppPairs, 
    const deviceScatteredFieldAccess<realx3> &points, 
    const deviceScatteredFieldAccess<real> &diams, 
    const deviceScatteredFieldAccess<realx3> &mirrorPoints, 
    const deviceScatteredFieldAccess<real> &mirrorDiams, 
    const realx3 &transferVec, 
    const deviceViewType3D<uint32> &head, 
    const deviceViewType1D<uint32> &next, 
    const cells &searchCells,
    const real sizeRatio
)
{
    
    if(points.empty()) return 0;
	if(mirrorPoints.empty())return 0;

	auto nMirror = mirrorPoints.size();
    
    uint32 getFull = 0;

    Kokkos::parallel_reduce(
		"pFlow::pweBndryContactSearchKernels::broadSearchPP",
		deviceRPolicyStatic(0, nMirror),
		LAMBDA_HD(const uint32 mrrI, uint32 &getFullUpdate)
		{
			realx3 p_m = mirrorPoints(mrrI) + transferVec;
			
			int32x3 ind_m;
			if( !searchCells.pointIndexInDomain(p_m, ind_m))return;
			
			real   d_m = sizeRatio*mirrorDiams[mrrI];
			
			for(int ii=-1; ii<2; ii++)
			{
			for(int jj=-1; jj<2; jj++)
			{
			for(int kk =-1; kk<2; kk++)
			{
				auto ind = ind_m + int32x3{ii,jj,kk};
				
				if(!searchCells.inCellRange(ind))continue;

				uint32 thisI = head(ind.x(),ind.y(),ind.z());
				while (thisI!=static_cast<uint32>(-1))
				{				
					
					auto d_n = sizeRatio*diams[thisI];
					
                    // first item is for this boundary and second itme, for mirror 
					if(sphereSphereCheckB(p_m, points[thisI], d_m, d_n)&&
                       ppPairs.insert(thisI,mrrI) == static_cast<uint32>(-1))
					{
						getFullUpdate++;	
					}

					thisI = next(thisI);
				}
			}
			}
			}
		},
        getFull
	);

    return getFull;
}
