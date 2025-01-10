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
#include "timeInfo.hpp"


namespace pFlow
{

// forward 

class internalPoints;
class dictionary;
class boundaryList;
class pointStructure;
class Time;

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

	const plane&           boundaryPlane_;

	/// list of particles indices on device
	uint32Vector_D         indexList_;

	/// list of particles indieces on host
	mutable uint32Vector_H indexListHost_;

	/// device and host list are sync
	mutable bool           listsSync_ = false;

	/// The length defined for creating neighbor list
	real                   neighborLength_;

	bool 				   updateTime_ = false;

	bool 				   iterBeforeUpdate_ = false;

	/// the extra boundary extension beyound actual limits of boundary 
	real 				   boundaryExtntionLengthRatio_;

	/// a reference to internal points
	internalPoints&        internal_;

	/// a reference to the list of boundaries
	/// (never use this in the constructor).
	boundaryList&          boundaries_;

	uint32                 thisBoundaryIndex_;

	int                    neighborProcessorNo_;

	bool                   isBoundaryMaster_;

	word                   name_;

	word                   type_;

protected:
	
	/// @brief Set the size of indexList. 
	/// It is virtual to let derived classed to be aware of 
	/// the fact that the size of boundary points has been changed.
	/// So, any drived class that override this method should call 
	/// boundaryBase::setSize(newSize) too. 
	virtual void setSize(uint32 newSize);


	void setUpdateTime(bool val)
	{
		updateTime_ = val;
	}

	void setNewIndices(const uint32Vector_D& newIndices);

	bool appendNewIndices(const uint32Vector_D& newIndices);
	
	bool removeIndices(
		uint32 numRemove, 
		const uint32Vector_D& removeMask);
	
	bool setRemoveKeepIndices(
		const uint32Vector_D& removeIndices,
		const uint32Vector_D& keepIndices);
	
	bool transferPointsToMirror(
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

	/// Update this boundary data in two steps (1 and 2).
	/// This is called after calling beforeIteration for 
	/// all boundaries, so any particle addition, deletion,
	/// and transfer has been finished up to this point.
	/// This two-step update help to have a flexible mechanism
	/// for data transfer, mostly for MPI related jobs. 
	virtual 
	bool updataBoundaryData(int step)
	{
		return true;
	}

	/// @brief This method is called when a transfer of data 
	/// is to be performed between processors (in afterIteration).
	/// @param step is the step in the transfer of data.
	/// @param callAgain if operation requires at least one additional step 
	/// to complete it should be set to true and if the operation is complete and no need for
	/// additional step, it should be set to false;  
	/// @return true: succesful, false: fail
	virtual 
	bool transferData(uint32 iter, int step, bool& callAgain)
	{
		callAgain = false;
		return true;
	}
	

public:

	uint32 markInNegativeSide(const word& name, uint32Vector_D& markedIndices )const;


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

	/// The length from boundary plane into the domain 
	/// where beyond that distance internal points exist.
	/// By conventions is it always equal to neighborLength_  
	inline
	real neighborLengthIntoInternal()const
	{
		return neighborLength_;
	}

	/// The distance length from boundary plane 
	/// where neighbor particles still exist in that distance. 
	/// This length may be modified in each boundary type 
	/// as required. In this case the boundaryExtensionLength
	/// method should also be modified accordingly. 
	virtual 
	real neighborLength()const
	{
		return (1+boundaryExtntionLengthRatio_)*neighborLength_;
	}

	/// The extention length (in vector form) for the boundary
	/// as required by  each boundary type. It is allowed for 
	/// each boundary type to be extended outward to allow 
	/// particles to stay more in its list before being removed 
	/// from its list. 
	virtual 
	realx3 boundaryExtensionLength()const
	{
		return -boundaryExtntionLengthRatio_*neighborLength_ * boundaryPlane_.normal();
	}

	/// Is this iter the right time for updating bounday list
	inline
	bool performBoundarytUpdate()const
	{
		return updateTime_;
	}

	inline
	bool iterBeforeBoundaryUpdate()const
	{
		return iterBeforeUpdate_;
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
	int neighborProcessorNo()const
	{
		return neighborProcessorNo_;
	}

	inline 
	int thisProcessorNo()const
	{
		return pFlowProcessors().localRank();
	}

	inline
    bool isBoundaryMaster()const
    {
        return isBoundaryMaster_;
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

	const pointStructure& pStruct()const;

	const Time& time()const;

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
	
	/// the actual boundary plane of this boundary 
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
	bool beforeIteration(
		uint32 step, 
		const timeInfo& ti, 
		bool updateIter, 
		bool iterBeforeUpdate , 
		bool& callAgain
	)
	{
		updateTime_ = updateIter;
		iterBeforeUpdate_ = iterBeforeUpdate;
		return true;
	}

	virtual 
	bool iterate(const timeInfo& ti) = 0;

	virtual 
	bool afterIteration(const timeInfo& ti) = 0;
	
    pointFieldAccessType thisPoints()const;

	/// @brief Return number of points in the neighbor processor boundary
	virtual 
	uint32 neighborProcSize()const
	{
		return 0;
	}

	/// @brief Return a reference to point positions in the neighbor
	/// processor boundary.
    virtual
    realx3Vector_D& neighborProcPoints();

	/// @brief Return a const reference to point positions in the 
	/// neighbor processor boundary.
	virtual
	const realx3Vector_D& neighborProcPoints()const;
    
	virtual 
	uint32 numToTransfer()const
	{
		return 0u;
	}

	virtual 
	uint32 numToRecieve()const
	{
		return 0u;
	}

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