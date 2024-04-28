#include "twoPartContactSearchKernels.hpp"

INLINE_FUNCTION_HD
bool
sphereSphereCheckB(
  const pFlow::realx3& p1,
  const pFlow::realx3  p2,
  pFlow::real          d1,
  pFlow::real          d2
)
{
	return pFlow::length(p2 - p1) < 0.5 * (d2 + d1);
}

void
pFlow::twoPartContactSearchKernels::buildNextHead(
  const deviceScatteredFieldAccess<realx3>& points,
  const cells&                              searchCells,
  deviceViewType3D<uint32>&                 head,
  deviceViewType1D<uint32>&                 next
)
{
	if (points.empty())
		return;

	uint32 n = points.size();

	Kokkos::parallel_for(
	  "pFlow::ppwBndryContactSearch::buildList",
	  deviceRPolicyStatic(0, n),
	  LAMBDA_HD(uint32 i) {
		  int32x3 ind;
		  if (searchCells.pointIndexInDomain(points[i], ind))
		  {
			  // discards points out of searchCell
			  uint32 old =
			    Kokkos::atomic_exchange(&head(ind.x(), ind.y(), ind.z()), i);
			  next[i] = old;
		  }
	  }
	);
	Kokkos::fence();
}

pFlow::uint32
pFlow::twoPartContactSearchKernels::broadSearchPP(
  csPairContainerType&                      ppPairs,
  const deviceScatteredFieldAccess<realx3>& points,
  const deviceScatteredFieldAccess<real>&   diams,
  const deviceScatteredFieldAccess<realx3>& mirrorPoints,
  const deviceScatteredFieldAccess<real>&   mirrorDiams,
  const realx3&                             transferVec,
  const deviceViewType3D<uint32>&           head,
  const deviceViewType1D<uint32>&           next,
  const cells&                              searchCells,
  const real                                sizeRatio
)
{
	if (points.empty())
		return 0;
	if (mirrorPoints.empty())
		return 0;

	auto   nMirror = mirrorPoints.size();

	uint32 getFull = 0;

	Kokkos::parallel_reduce(
	  "pFlow::twoPartContactSearchKernels::broadSearchPP",
	  deviceRPolicyStatic(0, nMirror),
	  LAMBDA_HD(const uint32 mrrI, uint32& getFullUpdate) {
		  realx3  p_m = mirrorPoints(mrrI) + transferVec;

		  int32x3 ind_m;
		  if (!searchCells.pointIndexInDomain(p_m, ind_m))
			  return;

		  real d_m = sizeRatio * mirrorDiams[mrrI];

		  for (int ii = -1; ii < 2; ii++)
		  {
			  for (int jj = -1; jj < 2; jj++)
			  {
				  for (int kk = -1; kk < 2; kk++)
				  {
					  auto ind = ind_m + int32x3{ ii, jj, kk };

					  if (!searchCells.inCellRange(ind))
						  continue;

					  uint32 thisI = head(ind.x(), ind.y(), ind.z());
					  while (thisI != -1)
					  {
						  auto d_n = sizeRatio * diams[thisI];

						  // first item is for this boundary and second itme,
						  // for mirror
						  if(sphereSphereCheckB(p_m, points[thisI], d_m, d_n)&&
                       ppPairs.insert(thisI,mrrI) == -1)
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

pFlow::uint32
pFlow::twoPartContactSearchKernels::broadSearchPP(
  csPairContainerType&                      ppPairs,
  const deviceScatteredFieldAccess<realx3>& points1,
  const deviceScatteredFieldAccess<real>&   diams1,
  const realx3Vector_D&                     points2,
  const realVector_D&                       diams2,
  const deviceViewType3D<uint32>&           head,
  const deviceViewType1D<uint32>&           next,
  const cells&                              searchCells,
  real                                      sizeRatio
)
{
	if (points1.empty())
		return 0;
	if (points2.empty())
		return 0;

	auto   nP2         = points2.size();
	auto   points2View = points2.deviceView();
	auto   diams2View  = diams2.deviceView();

	uint32 getFull = 0;

	Kokkos::parallel_reduce(
	  "pFlow::twoPartContactSearchKernels::broadSearchPP",
	  deviceRPolicyStatic(0, nP2),
	  LAMBDA_HD(const uint32 i2, uint32& getFullUpdate) {
		  realx3  p_m = points2View(i2);

		  int32x3 ind_m;
		  if (!searchCells.pointIndexInDomain(p_m, ind_m))
			  return;

		  real d_m = sizeRatio * diams2View[i2];

		  for (int ii = -1; ii < 2; ii++)
		  {
			  for (int jj = -1; jj < 2; jj++)
			  {
				  for (int kk = -1; kk < 2; kk++)
				  {
					  auto ind = ind_m + int32x3{ ii, jj, kk };

					  if (!searchCells.inCellRange(ind))
					  {
						  continue;
					  }

					  uint32 i1 = head(ind.x(), ind.y(), ind.z());
					  while (i1 != -1)
					  {
						  auto d_n = sizeRatio * diams1[i1];

						  // first item is for this boundary and second itme,
						  // for mirror
						  if(sphereSphereCheckB(p_m, points1[i1], d_m, d_n)&&
                       ppPairs.insert(i1,i2) == -1)
						  {
							  getFullUpdate++;
						  }

						  i1 = next(i1);
					  }
				  }
			  }
		  }
	  },
	  getFull
	);

	return getFull;
}