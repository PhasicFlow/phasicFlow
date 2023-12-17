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


//#include "indexContainer.hpp"

namespace pFlow
{

class internalPoints
:
	public subscriber
{
public:
	
	inline static const uint32 initialCapacity_ = 10000;

	using pointsType 		= realx3Field_D;

	using device_type 		= typename pointsType::device_type;

	using memory_space 		= typename pointsType::memory_space;

	using execution_space 	= typename pointsType::execution_space;

	using pFlagTypeDevice 	= pointFlag<execution_space>;

	using pFlagTypeHost 	= pointFlag<DefaultHostExecutionSpace>;


protected:

	//// - data members

		/// Position of points on device
		realx3Field_D 				pointPosition_;

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
		virtual ~internalPoints() = default;


	//// - Methods
		FUNCTION_H
		const pFlagTypeDevice& activePointsMaskD()const;


		FUNCTION_H
		const pFlagTypeHost& activePointsMaskH()const;

		// - Const access pointPosition
		FUNCTION_H
		const realx3Field_D& pointPosition()const;
		
		

		/*INLINE_FUNCTION_H
		auto pointPositionHostAll()
		{
			return pointPosition_.hostVectorAll();
		}*/

		// - size of data structure
		INLINE_FUNCTION_H
		uint32 size()const
		{
			return pointPosition_.size();
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

		
		FUNCTION_H
		void updateFlag(
			const domain& dm, 
			const std::array<real,6>& dist);

		/*FUNCTION_H
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

	
	//// - IO operations 
		
		/// Read 
		FUNCTION_H
		bool read(iIstream& is, IOPattern::IOType iotype);
		

		/// Write 
		FUNCTION_H
		bool write(iOstream& os, IOPattern::IOType iotype)const;	

};

inline
iOstream& operator<<(iOstream& os, const internalPoints& ip)
{
	if( !ip.write(os, IOPattern::AllProcessorsDifferent) )
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