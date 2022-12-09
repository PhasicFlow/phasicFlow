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


#ifndef __bitsetHD_hpp__ 
#define __bitsetHD_hpp__


#include "KokkosTypes.hpp"
#include "types.hpp"

// a lightweight container for holding bits on host or device 
// it is NOT concurrent and it does not used atommic operation on memory

namespace pFlow
{


template<typename blockType, typename MemorySpace=void>
class bitsetHD
{
public:

	using BlockType 		= blockType;

	using blockViewType 	= Kokkos::View<BlockType*, Kokkos::LayoutLeft, MemorySpace>;

  	using deviceType 		= typename blockViewType::device_type;

  	using memory_space 		= typename blockViewType::memory_space;

  	using execution_space 	= typename blockViewType::execution_space;  

protected:

	int32 			numBlocks_;

	int32 			numBits_;

	blockViewType 	blocks_;

	const static inline 
	int32 bitsPerBlock_ = std::numeric_limits<BlockType>::digits;

	const static inline
	int32 blockMask_   = bitsPerBlock_ - 1;

	static INLINE_FUNCTION_HD
	int32 blockIndex(int32 pos)
	{
		return pos/bitsPerBlock_;
	}
	
	static INLINE_FUNCTION_HD
	BlockType bitIndex(int32 pos)
	{
		return static_cast<BlockType>(pos%bitsPerBlock_);
	}

	static INLINE_FUNCTION_HD
	BlockType blockMask(int32 pos)
	{
		return static_cast<BlockType> (1 <<(pos & blockMask_));
	}

	static INLINE_FUNCTION_HD
	int32 calculateBlockSize(int32 numBits)
	{
		return numBits/bitsPerBlock_ + 1;
	}


public:

	
	bitsetHD(const word& name, int32 numBits)
	:
		numBlocks_(calculateBlockSize(numBits)),
		numBits_(numBits),
		blocks_(name, numBlocks_)
	{
		
	}

	bitsetHD(const bitsetHD&) = default;

	bitsetHD(bitsetHD&&) = default;

	bitsetHD& operator=(const bitsetHD&) = default;

	bitsetHD& operator=(bitsetHD&&) = default;

	void set()
	{
		Kokkos::deep_copy(blocks_, ~0);
	}

	void reset()
	{ 
		Kokkos::deep_copy( blocks_ , static_cast<BlockType>(0));
	}
   
    void clear() 
    {
    	Kokkos::deep_copy( blocks_ , static_cast<BlockType>(0)); 
    }

  	INLINE_FUNCTION_HD
  	void set(int32 pos) const 
  	{
		BlockType& block = blocks_[blockIndex(pos)];
		block |= blockMask(pos);
	}

	INLINE_FUNCTION_HD
	void unset(int32 pos)const
	{
		BlockType& block = blocks_[blockIndex(pos)];
		block &= (~blockMask(pos));
	}

	INLINE_FUNCTION_HD
	void reset(int32 pos)const
	{
		unset(pos);
	}

	INLINE_FUNCTION_HD
	void flip(int32 pos)const
	{
		BlockType& block = blocks_[blockIndex(pos)];
		block ^= blockMask(pos);
	}

	INLINE_FUNCTION_HD
	bool isSet(int32 pos)const
	{
		BlockType& block = blocks_[blockIndex(pos)];
		return block & blockMask(pos); 
	}

	INLINE_FUNCTION_HD
	bool isUnset(int32 pos)const
	{
		return !isSet(pos);
	}

	INLINE_FUNCTION_HD
	bool isSetReset(int32 pos)const
	{
		BlockType& block = blocks_[blockIndex(pos)];
		auto mask = blockMask(pos);
		bool is_set = block & mask;
		block &= (~mask);
		return is_set;
	}

	INLINE_FUNCTION_HD
	int32 numBlocks()const
	{
		return numBlocks_;
	}
	
	INLINE_FUNCTION_HD
	int32 numBits()const
	{
		return numBits_;
	}
	
	INLINE_FUNCTION_HD
	int32 size()const
	{
		return numBits_;
	}

	INLINE_FUNCTION_HD
	int32 capacity()const
	{
		return numBlocks_*bitsPerBlock_;
	}

	INLINE_FUNCTION_H
	void realloc(int32 numBits)
	{
		numBlocks_ = calculateBlockSize(numBits);
		numBits_ = numBits;
		Kokkos::realloc(blocks_, numBlocks_);
	}
  

};


using bitset32_D = bitsetHD<unsigned>;

using bitset32_H = bitsetHD<unsigned, HostSpace>;

using bitset64_D = bitsetHD<unsigned long>;

using bitset64_H = bitsetHD<unsigned long, HostSpace>;


} // namespace pFlow


#endif //__bitsetHD_hpp__
