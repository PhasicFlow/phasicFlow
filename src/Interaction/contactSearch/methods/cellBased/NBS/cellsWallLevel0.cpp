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

#include "cellsWallLevel0.hpp"
#include "streams.hpp"

pFlow::cellsWallLevel0::cellsWallLevel0
(
    real cellExtent, 
    uint32 numPoints, 
    uint32 numElements, 
    const ViewType1D<realx3, memory_space> &points,
    const ViewType1D<uint32x3, memory_space> &vertices,
	const ViewType1D<realx3, memory_space>& normals
)
:
    cellExtent_( max(cellExtent, half ) ),
    numElements_(numElements),
    numPoints_(numPoints),
    vertices_(vertices),
    points_(points),
	normals_(normals)
{
    allocateArrays();
}

bool pFlow::cellsWallLevel0::resetElements
(
	uint32 numElements, 
	uint32 numPoints, 
	const ViewType1D<realx3, memory_space>& points, 
	const ViewType1D<uint32x3, memory_space>& vertices,
	const ViewType1D<realx3, memory_space>& normals
)
{

	numElements_ = numElements;
	numPoints_ = numPoints;
	points_ 	= points;
	vertices_ 	= vertices;
	normals_ 	= normals;

	allocateArrays();

	return true;
}

bool pFlow::cellsWallLevel0::broadSearch
(
	csPairContainerType &pairs,
	const cells& searchBox,  
	const mapperNBS::CellIterator &particleMap,
	const deviceViewType1D<realx3>& pPoints,
	const deviceViewType1D<real>&  pDiams,
	real sizeRatio
)
{
	
	// map walls onto the cells
	
	this->build(searchBox);
	
	this->particleWallFindPairs(pairs, particleMap, pPoints, pDiams, sizeRatio);
	
	return true;
}

bool pFlow::cellsWallLevel0::build(const cells & searchBox)
{
	
    const auto& points = points_;
    const auto& vertices = vertices_;
    const auto& elementBox = elementBox_;
    const auto cellExtent = cellExtent_;

	Kokkos::parallel_for(
		"pFlow::cellsWallLevel0::build",
		deviceRPolicyStatic(0,numElements_),
		LAMBDA_HD(uint32 i)
		{
			auto v = vertices[i];
			auto p1 = points[v.x()];
			auto p2 = points[v.y()];
			auto p3 = points[v.z()];
			
			realx3 minP;
			realx3 maxP;

			searchBox.extendBox(p1, p2, p3, cellExtent, minP, maxP);
			elementBox[i] = iBoxType(searchBox.pointIndex(minP), searchBox.pointIndex(maxP));
		});
	Kokkos::fence();

	return true;
}

bool pFlow::cellsWallLevel0::particleWallFindPairs
(
	csPairContainerType &pairs, 
	const mapperNBS::CellIterator &particleMap,
	const deviceViewType1D<realx3>& pPoints,
	const deviceViewType1D<real>&  pDiams,
	real sizeRatio
)
{

	uint32 getFull = 1;

	while (getFull)
	{
		
		getFull = findPairsElementRangeCount(pairs, particleMap, pPoints, pDiams, sizeRatio);
		
		if(getFull)
		{
			// - resize the container
			//   note that getFull now shows the number of failed insertions.
			uint32 len = max(getFull, 50u);
			auto oldCap = pairs.capacity();
			pairs.increaseCapacityBy(len);

			INFORMATION<<"Contact pair container capacity increased from "<<
					oldCap << " to " 
					<< pairs.capacity() <<" in cellsWallLevel0."<<END_INFO;
			
			Kokkos::fence();
		}
	}
	
	return true;
}

pFlow::int32 pFlow::cellsWallLevel0::findPairsElementRangeCount
(
	csPairContainerType &pairs, 
	const mapperNBS::CellIterator &particleMap,
	const deviceViewType1D<realx3>& pPoints,
	const deviceViewType1D<real>&  pDiams,
	real sizeRatio
)
{
	uint32 getFull =0;
			
	const auto& elementBox = elementBox_;
    const auto& normals = normals_;
    const auto& points = points_;
    const auto& vertices = vertices_;
    const auto cellExtent = cellExtent_;

	Kokkos::parallel_reduce(
		"pFlow::cellsWallLevel0::findPairsElementRangeCount",
		tpPWContactSearch(numElements_, Kokkos::AUTO),
		LAMBDA_HD(
			const typename tpPWContactSearch::member_type & teamMember,
			uint32& valueToUpdate){
			
			const uint32 iTri = teamMember.league_rank();

			const auto triBox = elementBox[iTri];
			const auto triPlane = infinitePlane(
				normals[iTri], 
				points[vertices[iTri].x()]);

			uint32 getFull2 = 0;

			auto bExtent = boxExtent(triBox);
			uint32 numCellBox = bExtent.x()*bExtent.y()*bExtent.z();
			
			Kokkos::parallel_reduce( 
				Kokkos::TeamThreadRange( teamMember, numCellBox ),
				[&] ( const uint32 linIndex, uint32 &innerUpdate )
					{
						
						int32x3 cell;
						indexToCell(linIndex, triBox, cell);
						
						uint32 n = particleMap.start(cell.x(),cell.y(),cell.z());
						
						while( n != particleMap.NoPos)
						{
							// id is wall id the pair is (particle id, wall id)
							if( abs(triPlane.pointFromPlane(pPoints[n]))< pDiams[n]*sizeRatio*cellExtent)
							{
								if( pairs.insert(
									static_cast<csIdType>(n),
									static_cast<csIdType>(iTri) ) == static_cast<csIdType>(-1) 
                                )
									innerUpdate++;
							}
							n = particleMap.next(n);
						
						}

					},
				getFull2
			);

			if ( teamMember.team_rank() == 0 ) valueToUpdate += getFull2;
		},
		getFull
		);
	return getFull;
}