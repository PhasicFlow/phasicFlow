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
#ifndef __internalPoints_hpp__
#define __internalPoints_hpp__

#include "subscriber.hpp"
#include "Vectors.hpp"
#include "Fields.hpp"
#include "pointFlag.hpp"



namespace pFlow
{

class domain;

class internalPoints
:
	public subscriber
{
public:
	
	inline static const uint32 initialCapacity_ = 10000;

	using PointsType 		= realx3Field_D;

	using PointsTypeHost 	= typename PointsType::FieldTypeHost;

	using device_type 		= typename PointsType::device_type;

	using memory_space 		= typename PointsType::memory_space;

	using execution_space 	= typename PointsType::execution_space;

protected:

	//// - data members

		/// Position of points on device
		PointsType 					pointPosition_;

		/// flag of points on device
		mutable pFlagTypeDevice 	pFlagsD_;

		/// flag of points on host
		mutable pFlagTypeHost 		pFlagsH_;

		/// if both host and device flags sync
		mutable bool 				pFlagSync_ = false;
	
	//// - protected members

		void syncPFlag()const;

public:

	//friend class dynamicinternalPoints;
	
	// - type info
	TypeInfo("internalPoints");
	

	//// - Constructors 

		// - an empty internalPoints, good for reading from file 
		internalPoints();

		
		/// Construct from point positions, assume all points are active
		internalPoints(const realx3Vector& posVec);

		/// No Copy construct
		internalPoints(const internalPoints&) = delete;

		/// Move construct 
		internalPoints(internalPoints&&) = default;

		/// No Copy assignment 
		internalPoints& operator=(const internalPoints&) = delete;

		/// Move assignment 
		internalPoints& operator=(internalPoints&&) = default;

		/// Destructor 
		~internalPoints()override = default;


	//// - Methods
		FUNCTION_H
		const pFlagTypeDevice& activePointsMaskDevice()const;


		FUNCTION_H
		const pFlagTypeHost& activePointsMaskHost()const;

		// - Const access pointPosition
		FUNCTION_H
		const PointsType& pointPosition()const;
		
		FUNCTION_H
        PointsType& pointPosition();

        INLINE_FUNCTION_H
        auto pointPositionHost()const
        {
            return pointPosition_.hostView();
        }

        INLINE_FUNCTION_H
        auto pointPositionDevice()const
        {
            return pointPosition_.deviceView();
        }

		PointsTypeHost activePointsHost()const;

		// - size of data structure
		INLINE_FUNCTION_H
		uint32 size()const
		{
			return pFlagsD_.activeRange_.end();
		}
		
		// - maximum capacity of data structure 
		INLINE_FUNCTION_H
		uint32 capacity()const
		{
			return pointPosition_.capacity();
		}
		
		// - number of active points 
		INLINE_FUNCTION_H
		uint32 numActive() const
		{
			return pFlagsD_.numActive();
		}
				
		// - if all points are active 
		INLINE_FUNCTION_H
		bool isAllActive()const
		{
			return pFlagsD_.isAllActive();
		}

		INLINE_FUNCTION_H
		auto activeRange()const
		{
			return pFlagsD_.activeRange();
		}

		///@brief delete points at indices given in delPoints.
		/// The default is that delPoints contains sorted indices 
		FUNCTION_H
		bool deletePoints(
			scatteredFieldAccess<uint32, memory_space> delPoints);
		
		FUNCTION_H
		uint32 updateFlag(
			const domain& dm, 
			const std::array<real,6>& dist);
		
		FUNCTION_H
		void fillNeighborsLists(
			ViewType1D<uint32, memory_space> leftList,
			ViewType1D<uint32, memory_space> rightList,
			ViewType1D<uint32, memory_space> bottomList,
			ViewType1D<uint32, memory_space> topList,
			ViewType1D<uint32, memory_space> rearList,
			ViewType1D<uint32, memory_space> frontList);
		

		
	
	//// - IO operations 
		
		/// Read 
		FUNCTION_H
		bool read(iIstream& is);
		

		/// Write 
		FUNCTION_H
		bool write(iOstream& os)const;

		/// Read 
		FUNCTION_H
		bool read(iIstream& is, const IOPattern& iop);
		

		/// Write 
		FUNCTION_H
		bool write(iOstream& os, const IOPattern& iop)const;	

};

inline
iOstream& operator<<(iOstream& os, const internalPoints& ip)
{
	if( !ip.write(os) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}

	return os;
}

} // pFlow


#endif //__internalPoints_hpp__


/*class activePointsDevice
	{
	protected:
		ViewType1D<int8> 	flag_;

		bool allActive_;

		range activeRange_;

	public:

		INLINE_FUNCTION_H
		activePointsDevice(bool allActive, range active, const ViewType1D<int8>& flag)
		:
			flag_(flag),
			allActive_(allActive),
			activeRange_(active)
		{}

		INLINE_FUNCTION_HD
		activePointsDevice(const activePointsDevice&) = default;

		INLINE_FUNCTION_HD
		activePointsDevice& operator=(const activePointsDevice&) = default;

		INLINE_FUNCTION_HD
		bool operator()(int32 i)const {
			if(i<activeRange_.second && flag_[i] == 1)return true;
			return false;
		}

		INLINE_FUNCTION_HD
		auto activeRange()const {
			return activeRange_; 
		}

		INLINE_FUNCTION_HD
		auto allActive()const {
			return allActive_;
		}

	};

	class activePointsHost
	{
	protected:
		
		ViewType1D<int8, HostSpace> 	flag_;

		bool allActive_;

		range activeRange_;

	public:

		INLINE_FUNCTION_H
		activePointsHost(bool allActive, range active, const ViewType1D<int8, HostSpace>& flag)
		:
			flag_(flag),
			allActive_(allActive),
			activeRange_(active){}

		INLINE_FUNCTION_H
		activePointsHost(const activePointsHost&) = default;

		INLINE_FUNCTION_H
		activePointsHost& operator=(const activePointsHost&) = default;

		INLINE_FUNCTION_H
		bool operator()(int32 i)const {
			if(i <activeRange_.second && flag_[i] == PointFlag::ACTIVE)return true;
			return false;
		}

		INLINE_FUNCTION_H
		auto activeRange()const{
			return activeRange_;
		}

		INLINE_FUNCTION_H
		bool allActive()const {
			return allActive_;
		}
	};*/


	/*FUNCTION_H`
		size_t markDeleteOutOfBox(const box& domain);*/

		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		
		// - const access to points to be newly inserted
		/*FUNCTION_H 
		auto insertedPointIndex()const
		{
			return tobeInsertedIndex_;
		}

		FUNCTION_H 
		auto insertedPointIndexH()const
		{
			return tobeInsertedIndex_.hostView();
		}

		FUNCTION_H 
		auto insertedPointIndexD()const
		{
			return tobeInsertedIndex_.deviceView();
		}


		FUNCTION_H
		auto mortonSortedIndex()const
		{
			return mortonSortedIndex_;
		}


		// - update data structure by inserting/setting new points 
		//   Notifies all the fields in the registered list of data structure
		//   and exclude the fields that re in the exclusionList
		//   retrun nullptr if it fails 
		/*FUNCTION_H
		virtual uniquePtr<int32IndexContainer> insertPoints(
			const realx3Vector& pos,
			const setFieldList& setField,
			repository& owner,
			const List<eventObserver*>& exclusionList={nullptr}
		);*/
