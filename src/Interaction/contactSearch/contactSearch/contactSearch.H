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


#ifndef __contactSearch_H__
#define __contactSearch_H__


#include "interactionBase.H"
#include "unsortedPairs.H"
#include "box.H"
#include "dictionary.H"

namespace pFlow
{


class contactSearch
:
	public interactionBase
{
public:
	using IdType 			= typename interactionBase::IdType;

	using IndexType 		= typename interactionBase::IndexType;

	using ExecutionSpace 	= typename interactionBase::ExecutionSpace;

	using PairContainerType   = unsortedPairs<ExecutionSpace, IdType>;

protected:

	const box& 			domain_;

	dictionary 	dict_;

	Timer 		sphereSphereTimer_;

	Timer 		sphereWallTimer_;

	auto& dict()
	{
		return dict_;
	}
	
public:

	TypeName("contactSearch");

	contactSearch(
		const dictionary& dict,
		const box& domain,
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

	const auto& domain()const
	{
		return domain_;
	}

	const auto& dict()const
	{
		return dict_;
	}


	virtual 
	bool broadSearch(
		PairContainerType& ppPairs,
		PairContainerType& pwPairs,
		bool force = false) = 0;

	virtual 
	bool ppEnterBroadSearch()const = 0;

	virtual 
	bool pwEnterBroadSearch()const = 0;

	virtual 
	bool ppPerformedBroadSearch()const = 0;

	virtual 
	bool pwPerformedBroadSearch()const = 0;
	

	static 
	uniquePtr<contactSearch> create(
		const dictionary& dict,
		const box& domain,
	 	const particles& prtcl,
	 	const geometry& geom,
	 	Timers& timers);

};


}


#endif //__ContactSearch_H__
