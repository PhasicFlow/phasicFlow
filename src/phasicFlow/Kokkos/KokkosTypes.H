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

#ifndef __KokkosTypes_H__
#define __KokkosTypes_H__


#include <Kokkos_Core.hpp>
#include <Kokkos_DualView.hpp>
#include <Kokkos_UnorderedMap.hpp>


namespace pFlow
{

class DeviceSide{};
class HostSide{};

template<typename side>
struct selectSide{};

using HostSpace 					= Kokkos::HostSpace;
using Serial 						= Kokkos::Serial;

#ifdef _OPENMP
using OpenMP 						= Kokkos::OpenMP;
#endif

using DefaultHostExecutionSpace 	= Kokkos::DefaultHostExecutionSpace;
using DefaultExecutionSpace 		= Kokkos::DefaultExecutionSpace;



template<typename T1, typename T2>
	using kPair = Kokkos::pair<T1,T2>;

using range = kPair<int,int>;

using range64 = kPair<int long,int long>;

template<typename T, typename... properties>
	using ViewTypeScalar = Kokkos::View<T,properties...>;

template<typename T, typename... properties>
	using ViewType1D = Kokkos::View<T*,properties...>;

template<typename T, typename... properties>
	using ViewType3D = Kokkos::View<T***,properties...>;

template<typename Key, typename Value, typename... properties>
	using unorderedMap = Kokkos::UnorderedMap<Key, Value, properties...>;

template<typename Key, typename... properties>
	using unorderedSet = Kokkos::UnorderedMap<Key, void, properties...>;

template<typename Key, typename Value>
	using deviceHashMap= Kokkos::UnorderedMap<Key, Value>;

template<typename Key, typename Value>
	using hostHashMap= Kokkos::UnorderedMap<Key, Value, Kokkos::HostSpace>;

template<typename Key>
	using deviceHashSet= Kokkos::UnorderedMap<Key, void>;

template<typename Key>
	using hostHashSet  = Kokkos::UnorderedMap<Key,void, Kokkos::HostSpace>;

// a 1D array (vector) with default device (memory space and execution space)
template<typename T>
	using deviceViewTypeScalar = Kokkos::View<T>;

template<typename T>
	using deviceViewType1D = Kokkos::View<T*>;

template<typename T, typename Layout=void>
	using deviceViewType2D = Kokkos::View<T**,Layout, void>;


// a 1D array (vector with host memeory space)
template<typename T>
	using hostViewTypeScalar = Kokkos::View<T, Kokkos::HostSpace>;

template<typename T>
	using hostViewType1D = Kokkos::View<T*, Kokkos::HostSpace>;

template<typename T, typename Layout=void>
	using hostViewType2D = Kokkos::View<T**,Layout, Kokkos::HostSpace>;


#ifdef __CUDACC__
using Cuda 							= Kokkos::Cuda;
template<typename T>
	using cudaViewTypeScalar = Kokkos::View<T, Kokkos::CudaSpace>;

template<typename T>
	using cudaViewType1D = Kokkos::View<T*, Kokkos::CudaSpace>;

template<typename T, typename Layout=void>
	using cudaViewType2D = Kokkos::View<T*,Layout, Kokkos::CudaSpace>;
#endif


template<typename T>
using deviceAtomicViewType1D = 
    Kokkos::View<
		T*,
		Kokkos::MemoryTraits<std::is_same<DefaultExecutionSpace,Serial>::value?0:Kokkos::Atomic>>;

template<typename T>
using deviceAtomicViewType3D =
	Kokkos::View<
	T***,
	Kokkos::MemoryTraits<std::is_same<DefaultExecutionSpace,Serial>::value?0:Kokkos::Atomic>>;


} // pFlow

#endif  //__KokkosTypes_H__
