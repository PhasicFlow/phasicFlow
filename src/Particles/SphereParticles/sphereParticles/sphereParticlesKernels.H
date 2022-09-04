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

#ifndef __sphereParticlesKernels_H__
#define __sphereParticlesKernels_H__

namespace pFlow::sphereParticlesKernels
{

using rpAcceleration = Kokkos::RangePolicy<
			DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<int32>
			>;

template<typename IncludeFunctionType>
void acceleration( 
	realx3		g,
	deviceViewType1D<real>  	mass,
	deviceViewType1D<realx3>  	force,
	deviceViewType1D<real>  	I,
	deviceViewType1D<realx3>  	torque,
	IncludeFunctionType 		incld,
	deviceViewType1D<realx3> 	lAcc,
	deviceViewType1D<realx3> 	rAcc
	)
{

	auto activeRange = incld.activeRange();
	if(incld.allActive())
	{
		Kokkos::parallel_for(
		"pFlow::sphereParticlesKernels::acceleration",
		rpAcceleration(activeRange.first, activeRange.second),
		LAMBDA_HD(int32 i){
				lAcc[i] = force[i]/mass[i] + g;
				rAcc[i] = torque[i]/I[i];
		});
	}
	else
	{
		Kokkos::parallel_for(
		"pFlow::sphereParticlesKernels::acceleration",
		rpAcceleration(activeRange.first, activeRange.second),
		LAMBDA_HD(int32 i){
			if(incld(i))
			{
				lAcc[i] = force[i]/mass[i] + g;
				rAcc[i] = torque[i]/I[i];
			}
		});

	}
	
	Kokkos::fence();
}

}

#endif 
