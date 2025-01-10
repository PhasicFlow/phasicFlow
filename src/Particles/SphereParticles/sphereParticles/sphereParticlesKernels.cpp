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

#include "sphereParticlesKernels.hpp"

using policy = Kokkos::RangePolicy<
			pFlow::DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<pFlow::uint32>>;

void pFlow::sphereParticlesKernels::addMassDiamInertiaProp
(
    deviceViewType1D<uint32>    shapeIndex,
    deviceViewType1D<real>  	mass,
	deviceViewType1D<real>  	diameter,
	deviceViewType1D<real>  	I,
    deviceViewType1D<uint32>  	propertyId,
    pFlagTypeDevice 		    incld,
    deviceViewType1D<real>  	src_mass,
	deviceViewType1D<real>  	src_diameter,
	deviceViewType1D<real>  	src_I,
    deviceViewType1D<uint32>  	src_propertyId
)
{
    auto aRange = incld.activeRange();

    Kokkos::parallel_for(
		"particles::initInertia",
		policy(aRange.start(), aRange.end()),
		LAMBDA_HD(uint32 i)
		{
			if(incld(i))
			{
				uint32 index = shapeIndex[i];
				I[i] = src_I[index];
				diameter[i] = src_diameter[index];
				mass[i] = src_mass[index];
				propertyId[i] = src_propertyId[index];
			}
		});

}

void pFlow::sphereParticlesKernels::acceleration
( 
	const realx3&					g,
	const deviceViewType1D<real>&  	mass,
	const deviceViewType1D<realx3>& force,
	const deviceViewType1D<real>&  	I,
	const deviceViewType1D<realx3>& torque,
	const pFlagTypeDevice& 		    incld,
	deviceViewType1D<realx3>  		lAcc,
	deviceViewType1D<realx3>  		rAcc
)
{

	auto activeRange = incld.activeRange();
	if(incld.isAllActive())
	{
		Kokkos::parallel_for(
		"pFlow::sphereParticlesKernels::acceleration",
		policy(activeRange.start(), activeRange.end()),
		LAMBDA_HD(uint32 i){
				lAcc[i] = force[i]/mass[i] + g;
				rAcc[i] = torque[i]/I[i];
		});
	}
	else
	{
		Kokkos::parallel_for(
		"pFlow::sphereParticlesKernels::acceleration",
		policy(activeRange.start(), activeRange.end()),
		LAMBDA_HD(uint32 i){
			if(incld(i))
			{
				lAcc[i] = force[i]/mass[i] + g;
				rAcc[i] = torque[i]/I[i];
			}
		});

	}
	
	Kokkos::fence();
}
