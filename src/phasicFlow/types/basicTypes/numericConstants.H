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

#ifndef __numericConstants_H__
#define __numericConstants_H__


#include <limits>
#include "builtinTypes.H"


namespace pFlow
{

	const inline real Pi = real(3.1415926535897932384626433832);
	const inline real smallValue = 1.0e-15;
	const inline real verySmallValue = 1.0e-30;
	const inline real largeValue = 1.0e15;
	const inline real veryLargeValue = 1.0e30;

	// - largest negative value 
	template<typename T>
	constexpr inline T largestNegative()
	{
		return std::numeric_limits<T>::lowest();
	}

	template<typename T>
	constexpr inline T epsilonValue()
	{
		return std::numeric_limits<T>::min();
	}

	// largest positive value 
	template<typename T>
	constexpr inline T largestPositive()
	{
		return std::numeric_limits<T>::max();
	}

	const inline int32 largestNegInt32  		= largestNegative<int32>();
	const inline int32 largestPosInt32  		= largestPositive<int32>();

	const inline int64 largestNegInt64  		= largestNegative<int64>();
	const inline int64 largestPosInt64  		= largestPositive<int64>();
		
	const inline real largestNegREAL 	= largestNegative<real>();
	const inline real largestPosREAL 	= largestPositive<real>();
	const inline real epsilonREAL 		= epsilonValue<real>();
	
	

} // end of pFlow

#endif //__numericConstants_H__
