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


#ifndef __NBSLevel0_hpp__
#define __NBSLevel0_hpp__

#include "NBSLevel0Kernels.hpp"
#include "mapperNBS.hpp"

namespace pFlow
{


class NBSLevel0
:
	public mapperNBS
{
public:

	using MapperType 		= mapperNBS;

	using CellIterator  	= typename MapperType::CellIterator;

	using HeadType 			= typename MapperType::HeadType;

	using NextType 			= typename MapperType::NextType;	

	
private:

	real 		sizeRatio_ = 1.0;

	bool findPairs
	(
		csPairContainerType& pairs,
		const deviceViewType1D<realx3>& pointPos,
    	const pFlagTypeDevice& flags,
    	const deviceViewType1D<real>& diameter
	);

public:

	TypeInfoNV("NBSLevel0");

		INLINE_FUNCTION_HD
		NBSLevel0() = default;

		NBSLevel0(
			const 	box& domain,
			real 	cellSize,
			real 	sizeRatio,
			const 	deviceViewType1D<realx3>& pointPos,
			const 	pFlagTypeDevice& flags,
			bool 	adjustableBox);

		INLINE_FUNCTION_HD
		NBSLevel0(const NBSLevel0&) = default;

		INLINE_FUNCTION_HD
		NBSLevel0& operator = (const NBSLevel0&) = default;

		INLINE_FUNCTION_HD
		~NBSLevel0()=default;


	bool broadSearch(
		csPairContainerType& pairs,
		const deviceViewType1D<realx3>& pointPos,
    	const pFlagTypeDevice& flags,
    	const deviceViewType1D<real>& diameter,
		bool& searchBoxChanged);
		
	
	

};

} // pFlow

#endif // __NBSLevel0_hpp__
