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

#include "domain.hpp"
#include "boundaryBase.hpp"
#include "boundaryListPtr.hpp"
#include "timeInfo.hpp"
#include "boundariesMask.hpp"


namespace pFlow
{

class pointStructure;

class boundaryList
:
    public boundaryListPtr<boundaryBase>
{
private:

	//// - data members
	uint32 			neighborListUpdateInterval_;

	uint32 			updateInterval_;

	uint32 			lastNeighborUpdated_ = 0;

	bool 			neighborListUpdate_;

	bool 			boundaryUpdate_;

	bool 			iterBeforeBoundaryUpdate_;

	bool			listSet_ = false;

	pointStructure&   pStruct_;

	uniquePtr<domain> extendedDomain_;

	box				  internalDomainBox_;

	void setExtendedDomain();

	bool resetLists();

	/// @brief update neighbor list of boundaries regardless
	/// of the time intervals
	bool updateNeighborLists();

public:
	
	/// type info
	TypeInfo("boundaryList");
	

	//// - Constructors
	explicit boundaryList(pointStructure& pStruct);


  	virtual ~boundaryList() = default;

	/// @brief update neighbor list of boundaries based on 
	/// the time intervals
	bool updateNeighborLists(uint32 iter, bool force = false);

	bool createBoundaries();

	inline
	const pointStructure& pStruct()const
	{
		return pStruct_;
	}

	inline
	auto& boundary(size_t i)
	{
		return boundaryListPtr<boundaryBase>::operator[](i);
	}

	inline
	const auto& boundary(size_t i)const
	{
		return boundaryListPtr<boundaryBase>::operator[](i);
	}

	inline 
	bool boundariesUpdated()const 
	{
		return boundaryUpdate_;
	}

	inline
	const auto& extendedDomain()const
	{
		return extendedDomain_();
	}

	inline
	const auto& extendedDomainBox()const
	{
		return extendedDomain_->domainBox();
	}

	box internalDomainBox()const;
	
	bool beforeIteration(const timeInfo& tf, bool force = false);

	bool iterate(const timeInfo& tf, bool force = false);

	bool afterIteration(const timeInfo& tf, bool force = false);
	
};

} // pFlow




#endif //__boundaryList_hpp__


