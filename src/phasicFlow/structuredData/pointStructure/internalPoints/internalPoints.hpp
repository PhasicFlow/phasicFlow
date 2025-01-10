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
#include "VectorSingles.hpp"
#include "Fields.hpp"
#include "pointFlag.hpp"



namespace pFlow
{

class domain;
class boundaryBase;
class Time; 

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

private:

	//// - friend et al.
	
		friend boundaryBase;

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

		bool deletePoints(const uint32Vector_D& delPoints);

		bool changePointsFlagPosition(
			const uint32Vector_D& changePoints,
			realx3 transferVector, 
			uint32 fromBoundaryIndex,
			uint32 toBoundaryIndex);


protected:
	
	void syncPFlag()const;

	inline
	void unSyncFlag()
	{
		pFlagSync_ = false;
	}

	inline 
	void createDeviceFlag(uint32 cap, uint32 start, uint32 end)
	{
		unSyncFlag();
		pFlagsD_ = pFlagTypeDevice(cap, start, end);
	}

	bool sortPoints(const uint32IndexContainer& sortedIndices);

public:

	//friend class dynamicinternalPoints;
	
	// - type info
	TypeInfo("internalPoints");
	

	//// - Constructors 

		// - an empty internalPoints, good for reading from file 
		internalPoints();

		
		/// Construct from point positions, assume all points are active
		explicit internalPoints(const realx3Vector& posVec);

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
		
		const pFlagTypeDevice& activePointsMaskDevice()const;

		
		const pFlagTypeHost& activePointsMaskHost()const;

		// - Const access pointPosition
		const PointsType& pointPosition()const;
		
		
        PointsType& pointPosition();

        inline
        auto pointPositionHost()const
        {
            return pointPosition_.hostView();
        }

        inline
        auto pointPositionDevice()const
        {
            return pointPosition_.deviceView();
        }

		PointsTypeHost activePointsHost()const;

		// - size of data structure
		inline
		uint32 size()const
		{
			return pFlagsD_.activeRange_.end();
		}
		
		// - maximum capacity of data structure 
		inline
		uint32 capacity()const
		{
			return pFlagsD_.capacity();
		}

		inline
		bool empty()const
		{
			return size()==0u;
		}
		
		// - number of active points 
		inline
		uint32 numActive() const
		{
			return pFlagsD_.numActive();
		}
				
		// - if all points are active 
		inline
		bool isAllActive()const
		{
			return pFlagsD_.isAllActive();
		}

		inline
		auto activeRange()const
		{
			return pFlagsD_.activeRange();
		}
		
		virtual 
		Time& time() = 0;

		virtual
		const Time& time() const = 0;

		virtual 
		boundaryBase& boundary(size_t boundaryIndex ) = 0;

		virtual 
		const boundaryBase& boundary(size_t boundaryIndex ) const = 0;

		///@brief delete points at indices given in delPoints.
		/// The default is that delPoints contains sorted indices 
		
		bool deletePoints(
			scatteredFieldAccess<uint32, memory_space> delPoints);
		
		
		uint32 updateFlag(
			const domain& dm, 
			const std::array<real,6>& dist);
		
		
		void fillNeighborsLists(
			ViewType1D<uint32, memory_space> leftList,
			ViewType1D<uint32, memory_space> rightList,
			ViewType1D<uint32, memory_space> bottomList,
			ViewType1D<uint32, memory_space> topList,
			ViewType1D<uint32, memory_space> rearList,
			ViewType1D<uint32, memory_space> frontList);
		
		
		bool insertPoints(const realx3Vector& points, anyList& varList);

		bool insertPointsOnly(const realx3Vector_D& points, message& msg, anyList& varList);
		
	
	//// - IO operations 
		
		/// Read 
		bool read(iIstream& is);
		

		/// Write 
		bool write(iOstream& os)const;

		/// Read 
		bool read(iIstream& is, const IOPattern& iop);
		

		/// Write 
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

