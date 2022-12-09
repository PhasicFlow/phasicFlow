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


#include "contactSearch.hpp"
#include "box.hpp"

namespace pFlow
{

template<
	template<class> class BaseMethod,
	template<class> class WallMapping	
>
class ContactSearch
:
	public contactSearch
{
public:
	
	using IdType 			= typename contactSearch::IdType;

	using IndexType 		= typename contactSearch::IndexType;

	using ExecutionSpace 	= typename contactSearch::ExecutionSpace;

	using PairContainerType = typename contactSearch::PairContainerType;

	using ParticleContactSearchType = 
		BaseMethod<
			ExecutionSpace>;

	using WallMappingType =
		WallMapping<
			ExecutionSpace>;

protected:
	

	uniquePtr<ParticleContactSearchType> particleContactSearch_ = nullptr;

	uniquePtr<WallMappingType> 			 wallMapping_ = nullptr;

public:

	TypeInfoTemplate2("ContactSearch", ParticleContactSearchType, WallMappingType);

	ContactSearch(
		const dictionary& csDict,
		const box& domain,
	 	const particles& prtcl,
	 	const geometry& geom,
	 	Timers& timers)
	:
		contactSearch(csDict, domain, prtcl, geom, timers)
		
	{

		auto method = dict().getVal<word>("method");
		auto wmMethod = dict().getVal<word>("wallMapping");
		
		auto nbDict = dict().subDict(method+"Info");

		real minD, maxD;
		this->Particles().boundingSphereMinMax(minD, maxD);
		
		const auto& position = this->Particles().pointPosition().deviceVectorAll();
		const auto& diam = this->Particles().boundingSphere().deviceVectorAll();

		particleContactSearch_ = 
			makeUnique<ParticleContactSearchType>
			(
				nbDict,
				this->domain(),
				minD,
				maxD,
				position,
				diam
			);
		Report(2)<<"Contact search algorithm for particle-particle is "<<
				 greenText(particleContactSearch_().typeName())<<endReport;


		auto wmDict = dict().subDict(wmMethod+"Info");

		int32 wnPoints = this->Geometry().numPoints();
		int32 wnTri    = this->Geometry().size();

		const auto& wPoints = this->Geometry().points().deviceVectorAll();
		const auto& wVertices = this->Geometry().vertices().deviceVectorAll();

		wallMapping_ = 
			makeUnique<WallMappingType>(
				wmDict,
				particleContactSearch_().numLevels(),
				particleContactSearch_().getCellsLevels(),
				wnPoints,
				wnTri, 
				wPoints,
				wVertices
				);
		Report(2)<<"Wall mapping algorithm for particle-wall is "<<
				 greenText(wallMapping_().typeName())<< endReport;

	}


	add_vCtor(
		contactSearch,
		ContactSearch,
		dictionary);
	
	bool broadSearch(
		PairContainerType& ppPairs,
		PairContainerType& pwPairs,
		bool force = false) override
	{

		
		if(particleContactSearch_)
		{
			auto activeRange = this->Particles().activeRange();

			sphereSphereTimer_.start();

			if(this->Particles().allActive())
			{		
				particleContactSearch_().broadSearch(ppPairs, activeRange, force);
			}
			else
			{		
				particleContactSearch_().broadSearch(ppPairs, activeRange, this->Particles().activePointsMaskD(), force);
			}

			sphereSphereTimer_.end();

		}
		else
			return false;
		
		if(wallMapping_)
		{
			sphereWallTimer_.start();
			wallMapping_().broadSearch(pwPairs, particleContactSearch_(), force);
			sphereWallTimer_.end();
		}
		else
			return false;

		
		
		return true;
	}


	bool ppEnterBroadSearch()const override
	{
		if(particleContactSearch_)
		{
			return particleContactSearch_().enterBoadSearch(); 
		}
		return false;
	}

	bool pwEnterBroadSearch()const override
	{
		if(wallMapping_)
		{
			return wallMapping_().enterBoadSearch();
		}
		return false;
	}
	

	bool ppPerformedBroadSearch()const override
	{
		if(particleContactSearch_)
		{
			return particleContactSearch_().performedSearch();
		}
		return false;
	}

	
	bool pwPerformedBroadSearch()const override
	{
		if(wallMapping_)
		{
			return wallMapping_().performedSearch();
		}
		return false;
	}

	/*bool update(const eventMessage& msg) 
	{
		if(msg.isSizeChanged() ) 
		{
			auto newSize = this->prtcl().size();
			if(!particleContactSearch_().objectSizeChanged(newSize))
			{
				fatalErrorInFunction<<
				"erro in changing the size for particleContactSearch_ \n";
				return false;
			}
		}

		if(msg.isCapacityChanged() ) 
		{
			auto newSize = this->prtcl().capacity();
			if(!particleContactSearch_().objectSizeChanged(newSize))
			{
				fatalErrorInFunction<<
				"erro in changing the capacity for particleContactSearch_ \n";
				return false;
			}
		}	
		
		return true;
	}*/
	
	
};

}


#endif //__ContactSearch_hpp__
