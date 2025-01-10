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


#ifndef __contactSearch_hpp__
#define __contactSearch_hpp__


#include "contactSearchGlobals.hpp"
#include "dictionary.hpp"
#include "virtualConstructor.hpp"
#include "timeInfo.hpp"
#include "Timer.hpp"

namespace pFlow
{

// - forward 
class box;
class particles;
class geometry;
class pointStructure;


class contactSearch
{
private:

	const box& 	extendedDomainBox_;

	/// @brief update interval in terms of iteration numebr 
	uint32 		updateInterval_= 1;

	/// @brief  last iteration number which contact search has been performed
	uint32 		lastUpdated_ 	= 0;

	/// @brief performed search? 
	bool 		performedSearch_ = false;

	/// @brief performed search in boundaries
	bool 		performedSearchBoundary_ = false;

	/// const ref to particles
	const particles& 		particles_;

	/// const ref to geometry 
	const geometry&			geometry_;

	Timer 		bTimer_;

	Timer 		ppTimer_;

	dictionary 	dict_;

	virtual
	bool BroadSearch(
		const timeInfo& ti,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		bool force
	)=0;

	virtual
	bool  BoundaryBroadSearch(
		uint32 bndryIndex,
		const timeInfo& ti,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		bool force = false
	)=0;

public:

	TypeInfo("contactSearch");

	contactSearch(
		const dictionary& dict,
		const box& extDomain,
	 	const particles& prtcl,
	 	const geometry& geom,
	 	Timers& timers);

	virtual ~contactSearch()=default;


	create_vCtor
	(
		contactSearch,
		dictionary,
		(
			const dictionary& dict,
			const box& domain,
		 	const particles& prtcl,
		 	const geometry&  geom,
		 	Timers& timers
	 	),
	 	(dict, domain, prtcl, geom, timers)
	);

	inline
	bool performedSearch()const
	{
		return performedSearch_;
	}

	inline 
	bool performedSearchBoundary()const 
	{
		return performedSearchBoundary_;
	}

	inline
	bool performSearch(uint32 iter, bool force = false)const
	{
		if((iter-lastUpdated_) % updateInterval_ == 0 || iter == 0 || force )
		{
			return true;
		}
		return false;		
	}

	bool enterBroadSearch(const timeInfo& ti, bool force = false)const
	{
		return performSearch(ti.iter(), force);
	}

	virtual 
	bool enterBroadSearchBoundary(const timeInfo& ti, bool force=false)const = 0;

	inline
	uint32 updateInterval()const
	{
		return updateInterval_;
	}

	inline
	const dictionary& dict()const
	{
		return dict_;
	}

	inline
	const box& extendedDomainBox()const
	{
		return extendedDomainBox_;
	}

	inline
	const particles& Particles()const
	{
		return particles_;
	}

	const pointStructure& pStruct()const;

	inline
	const geometry& Geometry()const
	{
		return geometry_;
	}

	inline
	Timer& ppTimer()
	{
		return ppTimer_;
	}

	inline
	Timer& bTimer()
	{
		return bTimer_;
	}
 
	bool broadSearch(
		const timeInfo& ti,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		bool force = false);
	
	bool boundaryBroadSearch(
		uint32 bndryIndex,
		const timeInfo& ti,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		bool force = false);

	virtual 
	real sizeRatio()const = 0;

	virtual 
	real cellExtent()const = 0;
	
	static 
	uniquePtr<contactSearch> create(
		const dictionary& dict,
		const box& extDomain,
	 	const particles& prtcl,
	 	const geometry& geom,
	 	Timers& timers);

};


}


#endif //__ContactSearch_hpp__
