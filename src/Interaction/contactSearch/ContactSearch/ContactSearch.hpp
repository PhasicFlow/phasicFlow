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

public:

	TypeInfoTemplate11("ContactSearch", SearchMethodType);

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
				
		auto nbDict = dict().subDict(method+"Info");

		real minD, maxD;
		this->Particles().boundingSphereMinMax(minD, maxD);
		
		const auto& position = this->Particles().pointPosition().deviceViewAll();
		const auto& flags = this->Particles().dynPointStruct().activePointsMaskDevice();
		const auto& diam = this->Particles().boundingSphere().deviceViewAll();

		uint32 wnPoints = this->Geometry().numPoints();
		uint32 wnTri    = this->Geometry().size();
		const auto& wPoints = this->Geometry().points().deviceViewAll();
		const auto& wVertices = this->Geometry().vertices().deviceViewAll();

		ppwContactSearch_ = 
			makeUnique<SearchMethodType>
			(
				nbDict,
				this->domainBox(),
				minD,
				maxD,
				position,
				flags,
				diam,
				wnPoints,
				wnTri,
				wPoints,
				wVertices
			);
		REPORT(2)<<"Contact search algorithm for particle-particle is "<<
				 Green_Text(ppwContactSearch_().typeName())<<END_REPORT;

	}


	add_vCtor(
		contactSearch,
		ContactSearch,
		dictionary);

	
	bool broadSearch(
		uint32 iter,
		real t,
		real dt,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		bool force = false) override
	{

		ppTimer().start();

		const auto& position = Particles().pointPosition().deviceViewAll();
		const auto& flags = Particles().dynPointStruct().activePointsMaskDevice();
		const auto& diam = Particles().boundingSphere().deviceViewAll();
		
		
		if( !ppwContactSearch_().broadSearch(
			iter,
			t,
			dt,
			ppPairs, 
			pwPairs,
			position, 
			flags, 
			diam,
			force) )
		{
			fatalErrorInFunction;
			return false;
		}
		ppTimer().end();

				
	
		return true;
	}


	bool enterBroadSearch(uint32 iter, real t, real dt)const override
	{
		if(ppwContactSearch_)
		{
			return ppwContactSearch_().performSearch(iter); 
		}
		return false;
	}

	bool performedBroadSearch()const override
	{
		return ppwContactSearch_().performedSearch();
	}
	
};

}


#endif //__ContactSearch_hpp__
