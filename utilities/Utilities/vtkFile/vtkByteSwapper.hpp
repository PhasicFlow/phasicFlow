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

#ifndef __vtkByteSwapper_h__
#define __vtkByteSwapper_h__

namespace pFlow
{

/*
This utility is used to re-order bytes when writing data in binary format to vtk file
These lines of code are exactly copied from the source code of https://github.com/Kitware/VTK/
*/


template <size_t s>
struct vtkByteSwapper;

/// @brief with char, no re-ordering is required 
template <>
struct vtkByteSwapper<1>
{
	static inline void Swap(char*) {}
};

/// @brief re-order 2-byte data 
template <>
struct vtkByteSwapper<2>
{
	static inline void Swap(char* data)
	{
		const uint16_t& ref16 = *reinterpret_cast<uint16_t*>(data);
		*reinterpret_cast<uint16_t*>(data) = (ref16 >> 8) | (ref16 << 8);
	}
};

/// @brief re-order 4-byte data 
template <>
struct vtkByteSwapper<4>
{
	static inline void Swap(char* data)
	{
		const uint32_t& ref32 = *reinterpret_cast<uint32_t*>(data);
		*reinterpret_cast<uint32_t*>(data) =
			(ref32 >> 24) | (ref32 << 24) | ((ref32 & 0x00ff0000) >> 8) | ((ref32 & 0x0000ff00) << 8);
	}
};

/// @brief re-order of 8-byte data 
template <>
struct vtkByteSwapper<8>
{
	static inline void Swap(char* data)
	{
		const uint64_t& ref64 = *reinterpret_cast<uint64_t*>(data);
		*reinterpret_cast<uint64_t*>(data) = (ref64 >> 56) | (ref64 << 56) |
			((ref64 & 0x00ff000000000000) >> 40) | ((ref64 & 0x000000000000ff00) << 40) |
			((ref64 & 0x0000ff0000000000) >> 24) | ((ref64 & 0x0000000000ff0000) << 24) |
			((ref64 & 0x000000ff00000000) >> 8) | ((ref64 & 0x00000000ff000000) << 8);
	}
};

template<typename T>
inline T byteSwaper(const T& p)
{
    union
    {
      T value;
      char data[sizeof(T)];
    } temp = { p };
    vtkByteSwapper<sizeof(T)>::Swap(temp.data);
    return temp.value;
}



} // pFlow

#endif //__vtkByteSwapper_h__
