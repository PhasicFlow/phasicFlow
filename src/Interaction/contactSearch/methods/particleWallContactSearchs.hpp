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

#ifndef __particleWallContactSearchs_hpp__
#define __particleWallContactSearchs_hpp__

#include "pointFlag.hpp"
#include "contactSearchGlobals.hpp"
#include "box.hpp"

namespace pFlow
{
template<typename method>
class particleWallContactSearchs
{
public:

	using MethodType 		= method;

	using IndexType 		= uint32;

	using execution_space 	= DefaultExecutionSpace;

	using memory_space 		= typename execution_space::memory_space;

private:

	// friend 
	friend 	MethodType;

	/// @brief  box enclosing the simulation domain (local to processor)
	box 		domainBox_;

	/*/// @brief box enclosing the area for contact search (region with points)
	box 		searchBox_;*/

	/// @brief update interval in terms of iteration numebr 
	uint32 		updateInterval_= 1;

	/// @brief  last iteration number which contact search has been performed
	uint32 		lastUpdated_ 	= 0;

	/// @brief performed search? 
	bool 		performedSearch_ = false;

protected:

	inline
	auto& getMethod()
	{
		return static_cast<MethodType&>(*this);
	}

	inline
	const auto& getMethod()const
	{
		return static_cast<const MethodType&>(*this);
	}

public:

	particleWallContactSearchs(
		const 	dictionary& dict,
		const 	box& domain,
		real  	minSize,
		real 	maxSize,
		const 	ViewType1D<realx3, memory_space>& position,
		const 	pFlagTypeDevice &flags, 
		const 	ViewType1D<real, memory_space>& diam
	);

	bool broadSearch
	(
		uint32 iter,
		real t,
		real dt,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		const deviceViewType1D<realx3>& pointPos,
    	const pFlagTypeDevice& flags,
    	const deviceViewType1D<real>& diameter,
		bool  force = false
	);
		
	bool performedSearch()const
	{
		return performedSearch_;
	}

	bool performSearch(uint32 iter, bool force = false)const
	{
		if((iter-lastUpdated_) % updateInterval_ == 0 || iter == 0 || force )
		{
			return true;
		}
		return false;		
	}

};


} // pFlow

#include "particleWallContactSearchs.cpp"

#endif //__particleWallContactSearchs_hpp__