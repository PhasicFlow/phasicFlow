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

#include "span.hpp"
#include "KokkosTypes.hpp"
#include "KokkosUtilities.hpp"
#include "ViewAlgorithms.hpp"

namespace pFlow
{

template<typename IndexType>
class indexContainer
{
public:
	
	using DualViewType 		= Kokkos::DualView<IndexType*>;

	// - viewType of data on device 
  	using DeviceViewType 	= typename DualViewType::t_dev;

  	// - viewType of data on host
  	using HostViewType 		= typename DualViewType::t_host;

  	template<typename ViewType>
  	class IndexAccessor
  	{
  	protected:
  		ViewType 	view_;

  	public:
  		IndexAccessor(ViewType v):
  		view_(v){}

  		INLINE_FUNCTION_HD
  		IndexType operator()(int32 i)const
  		{
  			return view_[i];
  		}
  	};

protected:

	int32 		min_  	= 0;
	int32 		max_ 	= 0;
	size_t 		size_	= 0; 	

	DualViewType 	views_;

public:

	indexContainer(){}

	// half open [begin,end)
	indexContainer(IndexType begin, IndexType end)
	:
		min_(begin),
		max_(end-1),
		size_(end-begin),
		views_("indexContainer", size_)
	{
		pFlow::fillSequence(views_.d_view, 0, size_, min_);
		copy(views_.h_view, views_.d_view);
	}


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

	indexContainer(const indexContainer&) = default;

	indexContainer& operator = (const indexContainer&) = default;	

	~indexContainer() = default;

	INLINE_FUNCTION_HD
	size_t size()const
	{
		return size_;
	}

	INLINE_FUNCTION_HD
	size_t empty()const
	{
		return size_==0;
	}
	
	INLINE_FUNCTION_HD
	IndexType min()const
	{
		return min_;
	}

	INLINE_FUNCTION_HD
	IndexType max()const
	{
		return max_;
	}

	template<typename executionSpace>
	INLINE_FUNCTION_HD
	IndexType operator()(selectSide<executionSpace>,int32 i)const
	{	
		if constexpr (isHostAccessible<executionSpace>())
		{
			return views_.h_view(i);
		}else
		{
			return views_.d_view(i);
		}
	}

	const HostViewType& hostView()const
	{
		return views_.h_view;
	}

	const DeviceViewType& deviceView()const
	{
		return views_.d_view;
	}

	auto indicesHost()const
	{
		return IndexAccessor<HostViewType>(views_.h_view);
	}

	auto indicesDevice()const
	{
		return IndexAccessor<DeviceViewType>(views_.d_veiw);
	}
};


using int32IndexContainer = indexContainer<int32>;
using int64IndexContainer = indexContainer<int64>;


}


#endif 
