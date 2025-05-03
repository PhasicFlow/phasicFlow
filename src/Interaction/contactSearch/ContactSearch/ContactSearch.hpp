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


#ifndef __ContactSearch_hpp__
#define __ContactSearch_hpp__

#include "contactSearchGlobals.hpp"
#include "contactSearch.hpp"
#include "box.hpp"
#include "particles.hpp"
#include "geometry.hpp"
#include "boundaryContactSearchList.hpp"

namespace pFlow
{

template<
	class searchMethod
>
class ContactSearch
:
	public contactSearch
{
public:
	
	using IdType 			= uint32;

	using ExecutionSpace 	= DefaultExecutionSpace;

	using SearchMethodType = searchMethod;

private:
	
	uniquePtr<SearchMethodType> 	ppwContactSearch_ = nullptr;

	boundaryContactSearchList 		csBoundaries_;

	bool BroadSearch(
		const timeInfo& ti,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		bool force = false) override
	{
		
		auto position = Particles().pointPosition().deviceViewAll();
		auto flags = Particles().dynPointStruct().activePointsMaskDevice();
		auto diam = Particles().boundingSphere().deviceViewAll();

		return ppwContactSearch_().broadSearch(
			ti.iter(),
			ti.t(),
			ti.dt(),
			ppPairs, 
			pwPairs,
			position, 
			flags, 
			diam,
			force
		);
	}

	bool BoundaryBroadSearch(
		uint32 bndryIndex,
		const timeInfo& ti,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		bool force = false)override
	{		
		return csBoundaries_[bndryIndex].broadSearch(
			ti.iter(), 
			ti.t(), 
			ti.dt(), 
			ppPairs, 
			pwPairs, 
			force
		);
	}

public:

	TypeInfoTemplate11("ContactSearch", SearchMethodType);

	ContactSearch(
		const dictionary& csDict,
		const box& extDomain,
	 	const particles& prtcl,
	 	const geometry& geom,
	 	Timers& timers)
	:
		contactSearch(
			csDict, 
			extDomain, 
			prtcl, 
			geom, 
			timers),
		csBoundaries_(
			csDict, 
			Particles().pStruct().boundaries(),
			*this)
	{

		real minD;
		real maxD;
		this->Particles().boundingSphereMinMax(minD, maxD);
		
		const auto& position = this->Particles().pointPosition().deviceViewAll();
		const auto& flags = this->Particles().dynPointStruct().activePointsMaskDevice();
		const auto& diam = this->Particles().boundingSphere().deviceViewAll();

		uint32 wnPoints = this->Geometry().numPoints();
		uint32 wnTri    = this->Geometry().size();
		const auto& wPoints = this->Geometry().points().deviceViewAll();
		const auto& wVertices = this->Geometry().vertices().deviceViewAll();
		const auto& wNormals = this->Geometry().normals().deviceViewAll();

		ppwContactSearch_ = 
			makeUnique<SearchMethodType>
			(
				csDict,
				this->extendedDomainBox(),
				minD,
				maxD,
				position,
				flags,
				diam,
				wnPoints,
				wnTri,
				wPoints,
				wVertices,
				wNormals
			);
	}

	add_vCtor(
		contactSearch,
		ContactSearch,
		dictionary);

	bool enterBroadSearchBoundary(const timeInfo& ti, bool force=false)const override;
	

	real sizeRatio()const override
	{
		return ppwContactSearch_().sizeRatio();
	}

	real cellExtent()const override
	{
		return ppwContactSearch_().cellExtent();
	}
	
};

template <class searchMethod>
inline bool ContactSearch<searchMethod>::enterBroadSearchBoundary(const timeInfo &ti, bool force) const
{
	return performSearch(ti.iter(),force) || csBoundaries_.boundariesUpdated();
}

}

#endif //__ContactSearch_hpp__
