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
#ifndef __Range_hpp__
#define __Range_hpp__




#include <Kokkos_Core.hpp>

#include "pFlowMacros.hpp"
#include "typeInfo.hpp"
#include "builtinTypes.hpp"
#include "iOstream.hpp"


namespace pFlow
{


/**
 * Range for elements in an vector [start,end)
 *  
 */
template<typename T>
struct Range
:
public Kokkos::pair<T,T>
{
	using Pair = Kokkos::pair<T,T>;

	TypeInfoTemplateNV11("Range", T)

	//// - Constructors

		/// Default
		INLINE_FUNCTION_HD
		Range(){}

		/// From end, set start to 0
		INLINE_FUNCTION_HD
		Range(const T& e)
		:
			Range(0,e)
		{}
		
		/// From componeents
		INLINE_FUNCTION_HD
		Range(const T& s, const T& e)
		:
			Range::Pair(s,e)
		{}

		/// From pair
		INLINE_FUNCTION_HD
		Range(const Range::Pair &src )
		:
			Range::Pair(src)
		{}

		/// Copy
		INLINE_FUNCTION_HD
		Range(const Range&) = default;

		/// Move
		INLINE_FUNCTION_HD
		Range(Range&&) = default;

		/// Copy assignment
		INLINE_FUNCTION_HD
		Range& operator=(const Range&) = default;

		/// Move assignment
		INLINE_FUNCTION_HD
		Range& operator=(Range&&) = default;

		/// Destructor
		INLINE_FUNCTION_HD
		~Range()=default;

	//// - Methods

		/// Start
		INLINE_FUNCTION_HD
		T& start()
		{
			return this->first;
		}

		/// End
		INLINE_FUNCTION_HD
		T& end()
		{
			return this->second;
		}

		INLINE_FUNCTION_HD
		const T& start()const
		{
			return this->first;
		}

		INLINE_FUNCTION_HD
		const T& end()const
		{
			return this->second;
		}

		INLINE_FUNCTION_HD
		T numElements()
		{
			return end()-start();
		}

		INLINE_FUNCTION_HD
		auto getPair()const
		{
			return Pair(this->first, this->second);
		}

};

template<typename T>
INLINE_FUNCTION_H
iOstream& operator <<(iOstream& os, const Range<T>& rng)
{
	os<<"["<<rng.start()<<" "<<rng.end()<<")";
	return os;
}

using range32 	= Range<int32>;

using range64 	= Range<int64>;

using rangeU32 	= Range<uint32>;

using rangeU64 	= Range<uint64>;


} // pFlow

#endif  //__KokkosTypes_hpp__
