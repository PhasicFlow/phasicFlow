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

#ifndef __boundaryBase_hpp__
#define __boundaryBase_hpp__

#include "virtualConstructor.hpp"
#include "subscriber.hpp"
#include "VectorSingles.hpp"
#include "plane.hpp"
#include "scatteredFieldAccess.hpp"

#include "streams.hpp"

namespace pFlow
{

// forward 

class internalPoints;
class dictionary;
class boundaryList;

class boundaryBase
:
	public subscriber
{
public:

    using pointFieldAccessType = 
        deviceScatteredFieldAccess<realx3>;

private:

	// friend et al. 
	friend boundaryList;

	const plane& 	boundaryPlane_;

	/// list of particles indices on device 
	uint32Vector_D 	indexList_;

	/// list of particles indieces on host
	mutable uint32Vector_H 	indexListHost_;

	/// device and host list are sync
	mutable bool 			listsSync_ = false;

	/// The length defined for creating neighbor list 
	real  			neighborLength_;	

	/// a reference to internal points 
	internalPoints& internal_;

	/// a reference to the list of boundaries 
	/// (never use this in the constructor).
	boundaryList& 	boundaries_;

	uint32 			thisBoundaryIndex_;

	uint32 			mirrorProcessoNo_;

	word 		name_;

	word 		type_;

protected:
	
	/// @brief set the size of indexList 
	void setSize(uint32 newSize);

	void setNewIndices(const uint32Vector_D& newIndices);

	bool appendNewIndices(const uint32Vector_D& newIndices);
	
	bool removeIndices(
		uint32 numRemove, 
		const uint32Vector_D& removeMask);
	
	bool transferPoints(
		uint32 numTransfer, 
		const uint32Vector_D& transferMask,
		uint32 transferBoundaryIndex,
		realx3 transferVector);
	
	void unSyncLists()
	{
		listsSync_ = false;
	}

	void syncLists()const
	{
		if(!listsSync_)
		{
			indexListHost_.assignFromDevice(indexList_, true);
			listsSync_ = true;
		}
	}

	
public:

	TypeInfo("boundaryBase");
	
	boundaryBase(
		const dictionary &dict,
		const plane 	&bplane,
		internalPoints 	&internal,
		boundaryList	&bndrs,
		uint32 			thisIndex);

	boundaryBase(const boundaryBase&) = delete;

	boundaryBase& operator=(const boundaryBase&) = delete;

	boundaryBase(boundaryBase&&) = default;

	boundaryBase& operator=(boundaryBase&&) = default;

	~boundaryBase() override = default;
	

	create_vCtor
	(
		boundaryBase,
		dictionary,
		(
			const dictionary &dict,
    		const plane 	&bplane,
    		internalPoints 	&internal,
			boundaryList	&bndrs,
			uint32 			thisIndex
		),
		(dict, bplane, internal, bndrs, thisIndex)
	);

	virtual 
	real neighborLength()const
	{
		return neighborLength_;
	}

	virtual 
	realx3 boundaryExtensionLength()const
	{
		return {0,0,0};
	}

	inline
	const word& type()const
	{
		return type_;
	}

	inline
	const word& name()const
	{
		return name_;
	}

	inline
	bool empty()const
	{
		return indexList_.size()==0;
	}

	inline
	auto size()const
	{
		return indexList_.size();
	}

	inline
	auto capacity()const
	{
		return indexList_.capacity();
	}

	inline
	uint32 thisBoundaryIndex()const
	{
		return thisBoundaryIndex_;
	}

	inline
	uint32 mirrorBoundaryIndex()const
	{
		return thisBoundaryIndex_%2==0? thisBoundaryIndex_+1:thisBoundaryIndex_-1;
	}

	inline
	const internalPoints& internal()const
	{
		return internal_;
	}

	inline
	internalPoints& internal()
	{
		return internal_;
	}

	inline
	const auto& indexList()const
	{
		return indexList_;
	}

	inline
	const auto& indexListHost()const
	{
		syncLists();
		return indexListHost_;
	}

	boundaryBase& mirrorBoundary();

	const boundaryBase& mirrorBoundary()const;
	
	const plane& boundaryPlane()const
	{
		return boundaryPlane_;
	}

	/// @brief displacement vector that transfers points  
	/// to a distance that is equal to the distance between 
	/// this plane and the mirror plane, the vector points from 
	/// this plane to mirror plane 
	virtual
	realx3 displacementVectroToMirror()const;
	

	virtual 
    bool beforeIteration(uint32 iterNum, real t, real dt) = 0 ;

	virtual 
    bool iterate(uint32 iterNum, real t, real dt) = 0;

	virtual 
    bool afterIteration(uint32 iterNum, real t, real dt) = 0;
	
    pointFieldAccessType thisPoints()const;

    virtual
    pointFieldAccessType neighborPoints()const;
    
	/// - static create 
	static
	uniquePtr<boundaryBase> create
	(
		const dictionary &dict,
		const plane 	&bplane,
		internalPoints 	&internal,
		boundaryList	&bndrs,
		uint32 			thisIndex
	);

	
};

}


#endif //__boundaryBase_hpp__