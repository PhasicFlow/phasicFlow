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

	const particles& 		particles_;

	const geometry&			geometry_;

	Timer 		ppTimer_;

	Timer 		pwTimer_;

	dictionary 	dict_;

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

	
	const auto& dict()const
	{
		return dict_;
	}

	const auto& extendedDomainBox()const
	{
		return extendedDomainBox_;
	}

	const auto& Particles()const
	{
		return particles_;
	}

	const pointStructure& pStruct()const;

	const auto& Geometry()const
	{
		return geometry_;
	}

	auto& ppTimer()
	{
		return ppTimer_;
	}

	auto& pwTimer()
	{
		return pwTimer_;
	}

	virtual 
	bool broadSearch(
		uint32 iter,
		real t,
		real dt,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		bool force = false) = 0;
	
	virtual 
	bool boundaryBroadSearch(
		uint32 i,
		uint32 iter,
		real t,
		real dt,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		bool force = false)=0;

	virtual 
	bool enterBroadSearch(uint32 iter, real t, real dt)const = 0;

	virtual 
	bool performedBroadSearch()const = 0;

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
