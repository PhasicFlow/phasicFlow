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

#ifndef __indexContainer_hpp__ 
#define __indexContainer_hpp__

#include <vector>

#include "phasicFlowKokkos.hpp"


namespace pFlow
{

/**
 * It holds two vectors of indecis on Host and Device.
 * Host vector should be used for threads running on
 * Host and Device vector should be used for threads
 * running on Device.
 */
template<typename IndexType>
class indexContainer
{
public:
	
	using DVType 					= DualViewType1D<IndexType>;
	///  Device type on device 
	using DeviceViewType 	= typename DVType::t_dev;

	/// Host type on device 
	using HostViewType 		= typename DVType::t_host;

	/// Host memory type
	using HostType 			= typename HostViewType::device_type;

	/// Device memory ype
	using DeviceType 		= typename DeviceViewType::device_type;

	/**
	 * Helper class for accessing index on host or device 
	 */
  	template<typename ViewType>
  	class IndexAccessor
  	{
  	protected:
  		ViewType 	view_;

  	public:
  		IndexAccessor(ViewType v):
  		view_(v){}

  		INLINE_FUNCTION_HD
  		IndexType operator()(uint32 i)const
  		{
  			return view_[i];
  		}

  		uint32 size()const
  		{
  			return view_.extent(0);
  		}
  	};

protected:

	/// min value in indices
	IndexType 		min_  	= 0;

	/// max value in the indices
	IndexType 		max_ 	= 0;

	/// number/size of index vector 
	uint32 			size_	= 0; 	

	/// views to hold indices on Host and Device
	DVType 			views_;

public:

	//// - Constructors 

		/// Default
		indexContainer(){}

		/// Construct from a Range (half open)
		template<typename T>
		indexContainer(const Range<T>& rng)
		:
			indexContainer
			(
				static_cast<IndexType>(rng.begin()), 
				static_cast<IndexType>(rng.end())
			)
		{}

		/// Construct half open [begin,end)
		indexContainer(IndexType begin, IndexType end)
		:
			min_(begin),
			max_(end-1),
			size_(end-begin),
			views_("indexContainer", size_)
		{
			pFlow::fillSequence(views_.h_view, 0, size_, min_);
			copy(views_.d_view, views_.h_view);
		}

		/// From data and number of elements in data. 
		/// data is a pointer in the Host memory 
		indexContainer(IndexType* data, int32 numElems)
		:
			size_(numElems),
			views_("indexContainer", numElems)
		{
			HostViewType hData(data, numElems);
			copy(views_.h_view, hData);
			copy(views_.d_view, views_.h_view);
			min_ 	= pFlow::min(views_.d_view, 0, numElems);
			max_ 	= pFlow::max(views_.d_view, 0, numElems);
		}

		indexContainer(std::vector<IndexType> &ind)
		:
			indexContainer(ind.data(), ind.size())
		{}

		/// Copy
		indexContainer(const indexContainer&) = default;

		/// Copy assignment
		indexContainer& operator = (const indexContainer&) = default;

		/// Move
		indexContainer(indexContainer&&) = default;

		/// Mover assignement 
		indexContainer& operator = (indexContainer&&) = default;	

		/// Destructor
		~indexContainer() = default;

	//// - Methods 

		/// Size 
		INLINE_FUNCTION_HD
		auto size()const
		{
			return size_;
		}

		/// If the container empty
		INLINE_FUNCTION_HD
		bool empty()const
		{
			return size_==0;
		}
		
		/// Min value of indices 
		INLINE_FUNCTION_HD
		IndexType min()const
		{
			return min_;
		}

		/// Max value of indices 
		INLINE_FUNCTION_HD
		IndexType max()const
		{
			return max_;
		}

		/// Return Host veiw
		const HostViewType& hostView()const
		{
			return views_.h_view;
		}

		/// Return Device view
		const DeviceViewType& deviceView()const
		{
			return views_.d_view;
		}

		/// Return Host veiw
		HostViewType& hostView()
		{
			return views_.h_view;
		}

		/// Return Device veiw
		DeviceViewType& deviceView()
		{
			return views_.d_view;
		}

		/// Return index accessor that works on Host
		auto indicesHost()const
		{
			return IndexAccessor<HostViewType>(views_.h_view);
		}

		/// Return index accessor that works on Device 
		auto indicesDevice()const
		{
			return IndexAccessor<DeviceViewType>(views_.d_view);
		}

		/// Mark host is modified 
		void modifyOnHost()
		{
			views_.modify_host();
		}

		/// Mark device is modified
		void modifyOnDevice()
		{
			views_.modify_device();
		}

		/// synchronize views 
		void syncViews()
		{
			bool findMinMax = false;
			if(views_.template need_sync<HostType>())
			{
				Kokkos::deep_copy(views_.d_view, views_.h_view);
				views_.clear_sync_state();
				findMinMax = true;
			}
			else if(views_.template need_sync<DeviceType>())
			{
				Kokkos::deep_copy(views_.h_view, views_.d_view);
				views_.clear_sync_state();
				findMinMax = true;
			}

			if(findMinMax)
			{
				min_ 	= pFlow::min(views_.d_view, 0, size_);
				max_ 	= pFlow::max(views_.d_view, 0, size_);
			}
		}

};



using int32IndexContainer = indexContainer<int32>;
using int64IndexContainer = indexContainer<int64>;

using uint32IndexContainer = indexContainer<uint32>;
using uint64IndexContainer = indexContainer<uint64>;


}


#endif 
