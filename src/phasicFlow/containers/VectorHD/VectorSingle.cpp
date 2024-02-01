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

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
pFlow::uint32 pFlow::VectorSingle<T,MemorySpace>::changeSize
(
    uint32 n, 
    bool withInit
)
{
    if(n > this->capacity() )
    {
        uint32 newCap = evalCapacity(n);
        changeCapacity(newCap, withInit);
    }
    return setSize(n);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
pFlow::uint32 pFlow::VectorSingle<T,MemorySpace>::changeCapacitySize
(
    uint32 actualCap, 
    uint32 n, 
    bool withInit
)
{
    changeCapacity(actualCap, withInit);
    return setSize(n);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
void pFlow::VectorSingle<T,MemorySpace>::changeCapacity
(
    uint32 actualCap, 
    bool withInit
)
{
    if(withInit)
    {
        resizeInit(view_, actualCap);
    }
    else
    {
        resizeNoInit(view_, actualCap);
    }
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
pFlow::uint32 pFlow::VectorSingle<T,MemorySpace>::reallocateCapacitySize
(
    uint32 cap, 
    uint32 s
)
{
    reallocNoInit(view_, cap);
    return setSize(s);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
pFlow::uint32 pFlow::VectorSingle<T,MemorySpace>::setSize(uint32 n)
{
    auto os = size_;
    size_ = n;
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>::VectorSingle()
:
    VectorSingle("VectorSingle")
{}

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>::VectorSingle(const word& name)
:
    size_(0),
    view_(name,2)
{}

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>::VectorSingle
(
    const word& name, 
    uint32 n
)
:
    size_(n),
    view_(name, evalCapacity(n))
{}

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>::VectorSingle
(
    const word& name, 
    uint32 n, 
    const T& val
)
:
    VectorSingle(name, n)
{
    assign(n, val); 
}

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>::VectorSingle
(
    const word& name, 
    uint32 cap, 
    uint32 n, 
    RESERVE
)
:
    size_(n),
    view_(name, cap)
{}

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>::VectorSingle
(
    const word& name, 
    const std::vector<T> & src
) 
:
    VectorSingle(name)
{
    assign(src);
}

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>::VectorSingle
(
    const word& name, 
    const std::vector<T> & src, 
    uint32 cap
) 
:
    VectorSingle(name)
{
    assign(src, cap);
}

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>::VectorSingle
(
    const VectorSingle& src
)
:
    VectorSingle(src.name(), src)
{}

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>::VectorSingle
(
    const word& name, 
    const VectorSingle& src
)
:
    VectorSingle(name, src.capacity(), src.size(), RESERVE())
{
    copy(deviceView(), src.deviceView());	
}

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>& 
    pFlow::VectorSingle<T,MemorySpace>::operator = (const VectorSingle& rhs) 
{
    if(&rhs == this) return *this;
    VectorSingle temp(rhs);

    size_ 	= temp.size();	
    view_   = temp.view_;

    return *this;
}

template<typename T, typename MemorySpace>
pFlow::VectorSingle<T,MemorySpace>::~VectorSingle()
{
    view_ = viewType();
    size_ = 0;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
pFlow::uniquePtr<pFlow::VectorSingle<T,MemorySpace>>
 pFlow::VectorSingle<T,MemorySpace>::clone() const  
{
    return makeUnique<VectorSingle>( this->name(), *this);
}


template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
pFlow::VectorSingle<T,MemorySpace>& 
    pFlow::VectorSingle<T,MemorySpace>::VectorField()
{
    return *this;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
const pFlow::VectorSingle<T,MemorySpace>& 
pFlow::VectorSingle<T,MemorySpace>::VectorField()const
{
    return *this;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
const auto& pFlow::VectorSingle<T,MemorySpace>::deviceViewAll() const
{
    return view_;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
auto& pFlow::VectorSingle<T,MemorySpace>::deviceViewAll()
{
    return view_;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
auto pFlow::VectorSingle<T,MemorySpace>::deviceView()const
{
    return Kokkos::subview(
        view_, 
        Kokkos::make_pair(0u,static_cast<uint32>(size_)));
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
auto pFlow::VectorSingle<T,MemorySpace>::hostViewAll()const
{
    auto hView = Kokkos::create_mirror_view(view_);
    copy(hView, view_);
    return hView;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
auto pFlow::VectorSingle<T,MemorySpace>::hostView()const
{
    auto hView = Kokkos::create_mirror_view(deviceView());
    copy(hView, deviceView());
    return hView;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
pFlow::word pFlow::VectorSingle<T,MemorySpace>::name()const
{
    return view_.label();
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
pFlow::uint32 pFlow::VectorSingle<T,MemorySpace>::size()const
{
    return size_;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
pFlow::uint32 pFlow::VectorSingle<T,MemorySpace>::capacity()const
{
    return view_.extent(0);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
bool pFlow::VectorSingle<T,MemorySpace>::empty()const
{
    return size_==0;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
void pFlow::VectorSingle<T,MemorySpace>::reserve(uint32 cap) 
{
    changeCapacity(cap);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
void pFlow::VectorSingle<T,MemorySpace>::reallocate(uint32 cap) 
{
    reallocateCapacitySize(cap, 0);			
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
void pFlow::VectorSingle<T,MemorySpace>::reallocate
(
    uint32 cap, 
    uint32 newSize
) 
{
    reallocateCapacitySize(cap, newSize);			
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H   
void pFlow::VectorSingle<T,MemorySpace>::resize(uint32 n)
{
    changeSize(n);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H  
void pFlow::VectorSingle<T,MemorySpace>::resize(uint32 n, const T& val) 
{
    assign(n, val);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
void pFlow::VectorSingle<T,MemorySpace>::clear() 
{
    size_ = 0;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
void pFlow::VectorSingle<T,MemorySpace>::fill(const T& val)
{
    if(empty())return;
    pFlow::fill(view_, rangeU32(0 ,size_) ,val);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
void pFlow::VectorSingle<T,MemorySpace>::fill
(
    rangeU32 r, 
    const T& val
)
{
    pFlow::fill(view_, r, val);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
void pFlow::VectorSingle<T,MemorySpace>::assign
(
    size_t n, 
    const T& val
)
{
    if( n > capacity() )
    {
        reallocateCapacitySize(evalCapacity(n), n);
    }
    else
    {
        setSize(n);	
    }
    this->fill(val);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
void pFlow::VectorSingle<T,MemorySpace>::assign
(
    const std::vector<T>& src, 
    uint32 cap
) 
{
    uint32 srcSize = src.size();
    
    if(cap != capacity())
    {
        reallocateCapacitySize(cap, srcSize);
    }
    else
    {
        setSize(srcSize);
    }
    
    // - unmanaged view in the host
    hostViewType1D<const T> temp(src.data(), srcSize );
    copy(deviceView(), temp);
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
void pFlow::VectorSingle<T,MemorySpace>::assign
(
    const std::vector<T>& src
) 
{
    assign(src, src.capacity());
}
template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
void pFlow::VectorSingle<T,MemorySpace>::assign(const VectorTypeHost& src)
{
	uint32 srcSize = src.size();
	uint32 srcCap = src.capacity();

    if(srcCap != capacity())
    {
        reallocateCapacitySize(srcCap, srcSize);
    }
    else
    {
        setSize(srcSize);
    }
	copy(deviceView(), src.hostView());
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
auto pFlow::VectorSingle<T,MemorySpace>::getSpan()
{
    return span<T>(view_.data(), size());
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
auto pFlow::VectorSingle<T,MemorySpace>::getSpan()const
{
    return span<T>(const_cast<T*>(view_.data()), this->size());
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
bool pFlow::VectorSingle<T,MemorySpace>::insertSetElement(uint32IndexContainer indices, const T& val)
{
	if(indices.empty()) return true;
	
	auto maxInd = indices.max();

	if(this->empty() || maxInd > size()-1 )
	{
		resize(maxInd+1);
	}

	using policy = Kokkos::RangePolicy<execution_space,Kokkos::IndexType<uint32>>;

	if constexpr( isDeviceAccessible_ )
	{
		auto v = view_;
		auto ind = indices.deviceView();

		Kokkos::parallel_for(
			"VectorSingle::insertSetElement",
			policy(0,indices.size()),
			LAMBDA_HD(uint32 i){
				v[ind[i]]= val;
			});

		Kokkos::fence();
	}
	else
	{

		auto v = view_;
		auto ind = indices.hostView();
		
		Kokkos::parallel_for(
			"VectorSingle::insertSetElement",
			policy(0,indices.size()),
			LAMBDA_HD(uint32 i){
				v[ind[i]]= val;
			});

		Kokkos::fence();

	}

	return true;
}


template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
bool pFlow::VectorSingle<T,MemorySpace>::insertSetElement
(
	const uint32IndexContainer indices, 
	const std::vector<T>& vals
)
{

	if(indices.empty())return true;
	if(indices.size() != vals.size())return false;

	auto maxInd = indices.max(); 
	
	if(this->empty() || maxInd > size()-1 )
	{
		resize(maxInd+1);
	} 
	
	using policy = Kokkos::RangePolicy<execution_space,Kokkos::IndexType<uint32>>;

	hostViewType1D<const T> hVals( vals.data(), vals.size());

	if constexpr( isDeviceAccessible_ )
	{
		deviceViewType1D<T> dVals("dVals", indices.size());
		copy(dVals, hVals);
		auto dVec = view_;
		auto ind = indices.deviceView();

		Kokkos::parallel_for(
			"VectorSingle::insertSetElement",
			policy(0,indices.size()), LAMBDA_HD(uint32 i){	
				dVec(ind(i))= dVals(i);});
		Kokkos::fence();

	}
	else
	{
		auto dVec = view_;
		auto ind = indices.hostView();

		Kokkos::parallel_for(
			"VectorSingle::insertSetElement",
			policy(0,indices.size()), LAMBDA_HD(uint32 i){	
				dVec(ind(i))= hVals(i);});
		Kokkos::fence();

	}
		
	return true;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
bool pFlow::VectorSingle<T,MemorySpace>::insertSetElement
(
	uint32IndexContainer indices, 
	const ViewType1D<T, memory_space> vals
)
{
    if(indices.empty())return true;
	if(indices.size() != vals.size())return false;

	auto maxInd = indices.max(); 
	
	if(this->empty() || maxInd > size()-1 )
	{
		resize(maxInd+1);
	} 

    using policy = Kokkos::RangePolicy<
                    execution_space,
                    Kokkos::IndexType<uint32>>;

	if constexpr( isDeviceAccessible_ )
	{
		auto dVec = view_;
        auto dVals = vals;
		auto ind = indices.deviceView();

		Kokkos::parallel_for(
			"VectorSingle::insertSetElement",
			policy(0,indices.size()), LAMBDA_HD(uint32 i){	
				dVec(ind(i))= dVals(i);});
		Kokkos::fence();

	}
	else
	{
		auto dVec = view_;
        auto hVals = vals;
		auto ind = indices.hostView();

		Kokkos::parallel_for(
			"VectorSingle::insertSetElement",
			policy(0,indices.size()), LAMBDA_HD(uint32 i){	
				dVec(ind(i))= hVals(i);});
		Kokkos::fence();

	}
		
	return true;
}

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H
bool pFlow::VectorSingle<T,MemorySpace>::reorderItems(uint32IndexContainer indices)
{
	if(indices.size() == 0)
	{
		setSize(0);
		return true;
	}
	
	auto maxInd = indices.max();

	if(maxInd >= this->size())
	{
		fatalErrorInFunction<<"In reordering the VectorSingle ("
		<< this->name()<< ") maximum index ("<< maxInd << 
		") exceeds the size of the vector ("	<< this->size()<<")"<<endl;
		return false;
	}

	uint32 		newSize = indices.size();
	
	setSize(newSize);

	viewType 	sortedView(this->name(), newSize);

	using policy = Kokkos::RangePolicy< execution_space,Kokkos::IndexType<uint32>>;

	if constexpr( isDeviceAccessible_)
	{
		auto d_indices = indices.deviceView();
		auto d_view = view_;
		
		Kokkos::parallel_for
		(
			"VectorSingle::sortItems",
			policy(0,newSize), 
			LAMBDA_HD(uint32 i)
			{	
				sortedView(i) = d_view(d_indices(i));
			}
		);

		Kokkos::fence();

	}
	else
	{
		auto h_indices = indices.hostView();
		auto d_view = view_;
		
		Kokkos::parallel_for
		(
			"VectorSingle::sortItems",
			policy(0,newSize), 
			LAMBDA_HD(uint32 i)
			{	
				sortedView(i) = d_view(h_indices(i));
			}
		);
		
		Kokkos::fence();
	}

	copy(deviceView(), sortedView);
	
	return true;
}