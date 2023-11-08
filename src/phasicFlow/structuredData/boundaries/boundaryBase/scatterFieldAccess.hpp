
#ifndef __scatterFieldAccess_hpp__
#define __scatterFieldAccess_hpp__


#include "phasicFlowKokkos.hpp"

namespace pFlow
{


template<typename T, typename ExecutionSpace>
class scatterFieldAccess
{
public:
	
	using execution_space 	= ExecutionSpace;

	using memory_space 		= typename execution_space::memory_space;

protected:

	uint32 								size_ = 0;

	ViewType1D<uint32, memory_space> 	indices_;

	ViewType1D<T, memory_space> 		fieldVals_;

public:

	scatterFieldAccess(
		uint32 	sz
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

	T& operator()(uint32 i)
	{
		return fieldVals_(indices_(i));
	}

	const T& operator()(uint32 i)const
	{
		return fieldVals_(indices_(i));
	}

	T& operator[](uint32 i)
	{
		return fieldVals_(indices_(i));
	}

	const T& operator[](uint32 i)const
	{
		return fieldVals_(indices_(i));
	}

	uint32 	size()const
	{
		return size_;
	}

	bool empty()const
	{
		return size_ == 0;
	}

};

}


#endif //__scatterFieldAccess_hpp__

