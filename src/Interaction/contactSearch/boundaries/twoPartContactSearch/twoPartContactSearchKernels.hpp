#ifndef __twoPartContactSearchKernels_hpp__
#define __twoPartContactSearchKernels_hpp__

#include "contactSearchGlobals.hpp"
#include "cells.hpp"
#include "contactSearchFunctions.hpp"
#include "scatteredFieldAccess.hpp"
#include "VectorSingles.hpp"

namespace pFlow::twoPartContactSearchKernels
{

void buildNextHead(
    const deviceScatteredFieldAccess<realx3> &points,
    const cells              &searchCells,
    deviceViewType3D<uint32> &head, 
    deviceViewType1D<uint32> &next );


uint32 broadSearchPP
(
	csPairContainerType 						&ppPairs,
	const deviceScatteredFieldAccess<realx3> 	&points, 
	const deviceScatteredFieldAccess<real> 		&diams, 
	const deviceScatteredFieldAccess<realx3>	&mirrorPoints, 
	const deviceScatteredFieldAccess<real> 		&mirrorDiams, 
	const realx3 								&transferVec,
    const deviceViewType3D<uint32>				&head,
    const deviceViewType1D<uint32>				&next,
    const cells									&searchCells,
	real sizeRatio
);

uint32
broadSearchPP(
	csPairContainerType&                      ppPairs,
	const deviceScatteredFieldAccess<realx3>& points1,
	const deviceScatteredFieldAccess<real>&   diams1,
	const realx3Vector_D&                     points2,
	const realVector_D&                       diams2,
	const deviceViewType3D<uint32>&           head,
	const deviceViewType1D<uint32>&           next,
	const cells&                              searchCells,
	real                                      sizeRatio
);
}


#endif //__twoPartContactSearchKernels_hpp__