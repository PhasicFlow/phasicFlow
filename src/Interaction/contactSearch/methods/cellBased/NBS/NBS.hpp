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


#ifndef __NBS_hpp__
#define __NBS_hpp__

#include "particleWallContactSearchs.hpp"
#include "NBSLevel0.hpp"
#include "cellsWallLevel0.hpp"
#include "Vector.hpp"

namespace pFlow
{


class NBS
:
	public particleWallContactSearchs<NBS>
{
public:

	using CellIterator 	= typename NBSLevel0::CellIterator;

private:
	
	real 		sizeRatio_ = 1.0;

	real 		cellExtent_ = 0.5;

	Logical 	adjustableBox_;

	NBSLevel0 	NBSLevel0_;	

	cellsWallLevel0 cellsWallLevel0_;

protected:

	friend 		particleWallContactSearchs<NBS>;
	
	bool impl_broadSearch
	(
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		const deviceViewType1D<realx3>& pointPos,
    	const pFlagTypeDevice& flags,
    	const deviceViewType1D<real>& diameter
	)
	{
		bool searchBoxChanged;
		if( !NBSLevel0_.broadSearch(
			ppPairs, 
			pointPos, 
			flags, 
			diameter, 
			searchBoxChanged))
		{
			fatalErrorInFunction<<
			"Error in broadSearch for NBS (particle-particle)"<<endl;
			return false;
		}

		if(!cellsWallLevel0_.broadSearch(
			pwPairs,
			NBSLevel0_.getSearchCells(),
			NBSLevel0_.getCellIterator(),
			pointPos,
			diameter,
			sizeRatio_))
		{
			fatalErrorInFunction<<
			"Error in broadSearch for NBS (particle-wall)"<<endl;
			return false;
		}

		return true;
	}

public:

	TypeInfoNV("NBS");

		NBS(
			const dictionary& dict,
			const box& domainBox,
			real  	minBSSize,
			real 	maxBSSize,
			const deviceViewType1D<realx3> &position, 
    		const pFlagTypeDevice &flags, 
    		const deviceViewType1D<real> &diam, 
			uint32 					nWallPoints,
			uint32 					nWallElements,
			const ViewType1D<realx3,memory_space>& 		wallPoints, 
			const ViewType1D<uint32x3,memory_space>& 	wallVertices,
			const ViewType1D<realx3,memory_space>& 		wallNormals );
		
			
		INLINE_FUNCTION_HD
		NBS(const NBS&) = default;

		INLINE_FUNCTION_HD
		NBS(NBS&&) = default;

		INLINE_FUNCTION_HD
		NBS& operator = (const NBS&) = default;

		INLINE_FUNCTION_HD
		NBS& operator = (NBS&&) = default;

		INLINE_FUNCTION_HD
		~NBS()=default;


	//// - Methods 

		uint32 numLevels()const
		{
			return 1;
		}

		auto getCellIterator([[maybe_unused]] uint32 lvl)const
		{
			return NBSLevel0_.getCellIterator();
		}

		Vector<cells> getDomainCellsLevels()const
		{
			return Vector<cells>("Cells", 1, NBSLevel0_.getDomainCells());
		}

};

}

#endif
