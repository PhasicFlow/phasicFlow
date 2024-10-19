

#ifndef __AB2Kernels_hpp__
#define __AB2Kernels_hpp__

#include "KokkosTypes.hpp"
#include "types.hpp"

namespace pFlow::AB2Kernels
{
inline
bool intAllActive(
    const word& name,
	real dt, 
    rangeU32 activeRng,
	const deviceViewType1D<realx3>& y, 
	const deviceViewType1D<realx3>& dy,
	const deviceViewType1D<realx3>& dy1
)
{
	Kokkos::parallel_for(
		name,
		deviceRPolicyStatic (activeRng.start(), activeRng.end()),
		LAMBDA_HD(uint32 i){
			y[i] +=  dt*(static_cast<real>(1.5) * dy[i] - static_cast<real>(0.5) * dy1[i]);
			dy1[i] = dy[i];
		});
	Kokkos::fence();

	return true;	
}

inline
bool intScattered
(
	const word& name,
	real dt, 
    const pFlagTypeDevice& activePoints,
	const deviceViewType1D<realx3>& y, 
	const deviceViewType1D<realx3>& dy,
	const deviceViewType1D<realx3>& dy1
)
{

	Kokkos::parallel_for(
		name,
		deviceRPolicyStatic (activePoints.activeRange().start(), activePoints.activeRange().end()),
		LAMBDA_HD(uint32 i){
			if( activePoints(i))
			{
				y[i] +=  dt*(static_cast<real>(1.5) * dy[i] - static_cast<real>(0.5) * dy1[i]);
				dy1[i] = dy[i];
			}
		});
	Kokkos::fence();


	return true;
}

}

#endif