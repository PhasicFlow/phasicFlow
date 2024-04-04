
#include "contactSearchGlobals.hpp"
#include "cells.hpp"
#include "contactSearchFunctions.hpp"
#include "scatteredFieldAccess.hpp"

namespace pFlow::pweBndryContactSearchKernels
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

}