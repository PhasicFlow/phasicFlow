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


#ifndef __pointStructure_H__
#define __pointStructure_H__



#include "Vectors.H"
#include "VectorSingles.H"
#include "VectorDuals.H"
#include "Fields.H"
#include "eventSubscriber.H"
#include "indexContainer.H"



namespace pFlow
{

//forward
class box;
class setFieldList;
class repository;

class pointStructure
:
	public eventSubscriber
{
public:
	
	enum PointFlag: int8
	{
		DELETED 	= -1,
		ACTIVE 		= 1
	};

	
	inline static const size_t maxSizeDefault_ = 10000;

	class activePointsDevice
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
	};

protected:

	//// - data members 

		// number of points / size of structure
		size_t 			numPoints_ 		= 0;	

		// maximum number of points
		size_t 			maxPoints_ 		= maxSizeDefault_;

		// flag of points on device
		int8Field_HD 			pointFlag_;

		// position of points on device
		realx3Field_D 			pointPosition_;

		// number of active points
		size_t 					numActivePoints_ = 0;

		// index range of active points (half-open range)
		range 					activeRange_;	

		// - index vector for points to be inserted 
		int32IndexContainer   		tobeInsertedIndex_;
	

	//// - protected methods
		FUNCTION_H
		bool evaluatePointStructure();

		FUNCTION_H
		void setNumMaxPoints();

		// - access to pointPosition
		FUNCTION_H
		realx3Field_D& pointPosition();
		
		// - access to pointFlag
		FUNCTION_H
		int8Field_HD& pointFlag();

		FUNCTION_H
		uniquePtr<int32IndexContainer> 
			getNewPointsIndices(int32 numNewPoints)const;

public:

	friend class dynamicPointStructure;
	
	// - type info
	TypeName("pointStructure");
	

	//// - Constructors 

		// - an empty pointStructure, good for reading from file 
		pointStructure();

		// - construct from components
		pointStructure(const int8Vector& flgVec, const realx3Vector& posVec);

		// - construct from point positions, assume all points are active
		pointStructure(const realx3Vector& posVec);

		// - copy construct
		//
		//   should be changed, may causs undefined behavior 
		//////////////////////////////////////////////////////////////
		pointStructure(const pointStructure&) = default;

		
		// - no move construct 
		pointStructure(pointStructure&&) = delete;

		// - copy assignment 
		//
		//   should be changed, may causs undefined behavior 
		//////////////////////////////////////////////////////////////
		pointStructure& operator=(const pointStructure&) = default;

		// - no move assignment 
		pointStructure& operator=(pointStructure&&) = delete;

		// - destructor 
		virtual ~pointStructure() = default;


	//// - Methods
		activePointsDevice activePointsMaskD()const
		{
			return activePointsDevice(
				this->allActive(),
				activeRange(),
				pointFlag_.deviceVectorAll()
				);
		}

		activePointsHost activePointsMaskH()const
		{
			return activePointsHost(
				this->allActive(),
				activeRange(),
				pointFlag_.hostVectorAll()
				);
		}

		// - const access pointPosition
		FUNCTION_H
		const realx3Field_D& pointPosition()const;
		
		// - const access to pointFlag
		FUNCTION_H
		const int8Field_HD& pointFlag()const;

		// - size of data structure
		FUNCTION_H
		label size()const;
		
		// - maximum capacity of data structure 
		FUNCTION_H
		label capacity()const;
		
		// - number of active points 
		FUNCTION_H
		label numActive() const;
				
		// - if all points are active 
		FUNCTION_H
		bool allActive()const;

		INLINE_FUNCTION_H
		range activeRange()const
		{
			return activeRange_;
		}

		INLINE_FUNCTION_H
		bool isActive(label i)const
		{
			return pointFlag_[i] == ACTIVE;
		}

		FUNCTION_H
		size_t markDeleteOutOfBox(const box& domain);

		// - update data structure by removing the marked points 
		//   Notifies all the fields that are built based on this data structure
		//   and then apply removing to the pointPosition_ and pointFlag_
		FUNCTION_H
		virtual bool updateForDelete();

		///////////////////////////////////////////////////////////////////////////////////////////////////
		
		// - const access to points to be newly inserted
		FUNCTION_H 
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


		// - update data structure by inserting/setting new points 
		//   Notifies all the fields in the registered list of data structure
		//   and exclude the fields that re in the exclusionList
		//   retrun nullptr if it fails 
		FUNCTION_H
		virtual uniquePtr<int32IndexContainer> insertPoints(
			const realx3Vector& pos,
			const setFieldList& setField,
			repository& owner,
			const List<eventObserver*>& exclusionList={nullptr}
		);

	
	//// - IO operations 
		// - read pointStructure from is
		FUNCTION_H
		bool readPointStructure(iIstream& is);

		// - write pointStructure to os
		FUNCTION_H
		bool writePointStructure(iOstream& os)const;

		// - read 
		FUNCTION_H
		bool read(iIstream& is)
		{
			return readPointStructure(is);
		}

		// - write 
		FUNCTION_H
		bool write(iOstream& os)const
		{
			return writePointStructure(os);
		}

};


} // pFlow




#endif //__pointStructure_H__
