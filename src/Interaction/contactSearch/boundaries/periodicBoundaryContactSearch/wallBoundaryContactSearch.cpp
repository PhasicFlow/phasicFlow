#include "wallBoundaryContactSearch.hpp"
#include "streams.hpp"

pFlow::wallBoundaryContactSearch::wallBoundaryContactSearch
(
    real cellExtent, 
    uint32 numPoints, 
    uint32 numElements, 
    const ViewType1D<realx3, memory_space> &points, 
    const ViewType1D<uint32x3, memory_space> &vertices, 
    const ViewType1D<realx3, memory_space> &normals
)
:
    cellExtent_( max(cellExtent, 0.5 ) ),
    numElements_(numElements),
    numPoints_(numPoints),
    vertices_(vertices),
    points_(points),
	normals_(normals)
{
    allocateArrays();
}

bool pFlow::wallBoundaryContactSearch::build(const cells &searchBox, const realx3& transferVec)
{
    Kokkos::parallel_for(
		"pFlow::cellsWallLevel0::build",
		deviceRPolicyStatic(0,numElements_),
		CLASS_LAMBDA_HD(uint32 i)
		{
			auto v = vertices_[i];
			auto p1 = points_[v.x()]+transferVec;
			auto p2 = points_[v.y()]+transferVec;
			auto p3 = points_[v.z()]+transferVec;
			
			realx3 minP;
			realx3 maxP;

			searchBox.extendBox(p1, p2, p3, cellExtent_, minP, maxP);
			elementBox_[i] = iBoxType(searchBox.pointIndex(minP), searchBox.pointIndex(maxP));
            auto d = elementBox_[i].maxPoint()-elementBox_[i].minPoint();
            validBox_[i] = (d.x()*d.y()*d.z())==0? 0:1; 
		});
	Kokkos::fence();

	return true;
}

bool pFlow::wallBoundaryContactSearch::broadSearch
(
    csPairContainerType &pairs, 
    const cells &searchCells, 
    const deviceScatteredFieldAccess<realx3> &thisPoints, 
    const deviceScatteredFieldAccess<real> &thisDiams, 
    const deviceScatteredFieldAccess<realx3> &mirrorPoints, 
    const deviceScatteredFieldAccess<real> &mirroDiams, 
    const realx3 &transferVec, 
    real sizeRatio
)
{
    uint32 nNotInserted = 1;

    while (nNotInserted>0u)
    {
        build(searchCells,{0,0,0});
        nNotInserted = findPairsElementRangeCount(
            pairs,
            searchCells,
            thisPoints,
            thisDiams,
            {0,0,0},
            0
        );

        build(searchCells, transferVec);
        nNotInserted += findPairsElementRangeCount(
            pairs,
            searchCells,
            mirrorPoints,
            mirroDiams,
            transferVec,
            BASE_MIRROR_WALL_INDEX
        );

        if(nNotInserted>0u)
        {
            //   note that getFull now shows the number of failed insertions.
			uint32 incCap = max(nNotInserted,50u) ;
			
			auto oldCap = pairs.capacity();
			
			pairs.increaseCapacityBy(incCap);

			INFORMATION<< "The contact pair container capacity increased from "<<
			oldCap << " to "<<pairs.capacity()<<" in wallBoundaryContactSearch."<<END_INFO;
        }
    }
    return true;
}

pFlow::uint32 pFlow::wallBoundaryContactSearch::findPairsElementRangeCount
(
    csPairContainerType &pairs, 
    const cells &searchCells, 
    const deviceScatteredFieldAccess<realx3> &pPoints, 
    const deviceScatteredFieldAccess<real> &pDiams, 
    const realx3 &transferVec, 
    uint  baseTriIndex
)
{
    
    if(pPoints.empty())return 0u;

    uint32 nNotInserted = 0;
    uint32 nThis = pPoints.size();
    
    Kokkos::parallel_reduce(
        "pFlow::wallBoundaryContactSearch::findPairsElementRangeCount",
        deviceRPolicyDynamic(0,nThis),
        LAMBDA_HD(uint32 i, uint32 &notInsertedUpdate)
        {
            auto p = pPoints[i]+transferVec;
            int32x3 ind;
            if( searchCells.pointIndexInDomain(p, ind) )
            {
                for(uint32 nTri=0; nTri<numElements_; nTri++)
                {
                    if( validBox_[nTri]== 0)continue;
                    if( elementBox_[nTri].isInside(ind)&& 
                        pairs.insert(i,nTri+baseTriIndex) == -1)
                    {
                        notInsertedUpdate++;    
                    }
                }
            }
        },
        nNotInserted
    );

    return nNotInserted;
}
