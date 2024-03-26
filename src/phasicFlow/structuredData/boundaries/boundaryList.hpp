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


#ifndef __boundaryList_hpp__
#define __boundaryList_hpp__

#include "boundaryBase.hpp"
#include "ListPtr.hpp"
#include "baseTimeControl.hpp"


namespace pFlow
{

class pointStructure;

class boundaryList
:
    public ListPtr<boundaryBase>
{

protected:

	//// - data members
		pointStructure& 		pStruct_;

		baseTimeControl 		timeControl_;

		bool                    listSet_ = false;

		bool resetLists();

		/// @brief update neighbor list of boundaries regardless
		/// of the time intervals 
		bool updateLists();

public:
	
	/// type info
	TypeInfo("boundaryList");
	

	//// - Constructors
	boundaryList(pointStructure& pStruct);


  	~boundaryList() = default;

	/// @brief update neighbor list of boundaries based on 
	/// the time intervals
	bool updateLists(uint32 iter, real t, real dt);

	bool setLists();

	const pointStructure& pStruct()const
	{
		return pStruct_;
	}

	auto& boundary(size_t i)
	{
		return ListPtr<boundaryBase>::operator[](i);
	}

	const auto& boundary(size_t i)const
	{
		return ListPtr<boundaryBase>::operator[](i);
	}

	const baseTimeControl& timeControl()const
	{
		return timeControl_;
	}

	bool beforeIteration(uint32 iter, real t, real dt);

	bool iterate(uint32 iter, real t, real dt);

	bool afterIteration(uint32 iter, real t, real dt);
	
};

} // pFlow




#endif //__boundaryList_hpp__


