
#ifndef __scatterFieldAccess_hpp__
#define __scatterFieldAccess_hpp__


#include "phasicFlowKokkos.hpp"

namespace pFlow
{


template<typename T, typename MemorySpace>
class scatterFieldAccess
{
public:
	
	using viewType 			= ViewType1D<T, MemorySpace>;

    using device_type 		= typename viewType::device_type;

    using memory_space 		= typename viewType::memory_space;

    using execution_space 	= typename viewType::execution_space;

protected:

	uint32 								size_ = 0;

	ViewType1D<uint32, memory_space> 	indices_;

	viewType 		fieldVals_;

public:

    scatterFieldAccess():
        indices_(),
        fieldVals_()    
    {}

	scatterFieldAccess(
		uint32 	sz,
		ViewType1D<uint32, memory_space> ind,
		ViewType1D<T, memory_space> fVals)
	:
		size_(sz),
		indices_(ind),
		fieldVals_(fVals)
	{}

	scatterFieldAccess(const scatterFieldAccess&) = default;

	scatterFieldAccess(scatterFieldAccess&&) = default;

	scatterFieldAccess& operator=(const scatterFieldAccess&) = default;

	scatterFieldAccess& operator=(scatterFieldAccess&&) = default;

	~scatterFieldAccess() = default;

	// - Methods

    INLINE_FUNCTION_HD
	T& operator()(uint32 i)
	{
		return fieldVals_(indices_(i));
	}

    INLINE_FUNCTION_HD
	const T& operator()(uint32 i)const
	{
		return fieldVals_(indices_(i));
	}

    INLINE_FUNCTION_HD
	T& operator[](uint32 i)
	{
		return fieldVals_(indices_(i));
	}

    INLINE_FUNCTION_HD
	const T& operator[](uint32 i)const
	{
		return fieldVals_(indices_(i));
	}

    INLINE_FUNCTION_HD
	uint32 	size()const
	{
		return size_;
	}

    INLINE_FUNCTION_HD
	bool empty()const
	{
		return size_ == 0;
	}

};

}


#endif //__scatterFieldAccess_hpp__

