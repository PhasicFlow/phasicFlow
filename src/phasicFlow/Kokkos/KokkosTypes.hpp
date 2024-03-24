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
#ifndef __KokkosTypes_hpp__
#define __KokkosTypes_hpp__

/**
 *  \file KokkosType.hpp
 * 
 * 	\brief name aliases and typedesf for Kokkos entities that are 
 * frequently used in PhasicFlow.
 * 
 */


#include <Kokkos_Core.hpp>
#include <Kokkos_DualView.hpp>
#include <Kokkos_UnorderedMap.hpp>

#include "builtinTypes.hpp"


namespace pFlow
{


/// Host memory space 
using HostSpace 	= Kokkos::HostSpace;

/// Serial execution space 
using Serial 		= Kokkos::Serial;

#ifdef _OPENMP
/// OpenMp execution space 
using OpenMP 		= Kokkos::OpenMP;
#endif

#ifdef __CUDACC__
/// Cuda execution space 
using Cuda 			= Kokkos::Cuda;
#endif

/// Default Host execution space, on top of all host execution spaces 
using DefaultHostExecutionSpace 	= Kokkos::DefaultHostExecutionSpace;

/// Default execution space, it can be device exe. space, if a device space is 
/// activated. 
using DefaultExecutionSpace 		= Kokkos::DefaultExecutionSpace;


using deviceRPolicyStatic = 
	Kokkos::RangePolicy<
			Kokkos::DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<pFlow::uint32> >;


using hostRPolicyStatic = 
	Kokkos::RangePolicy<
			Kokkos::DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<pFlow::uint32> >;


/// Pair of two variables
template<typename T1, typename T2>
using Pair 	= Kokkos::pair<T1,T2>;

/// View for a scalar
template<typename T, typename... properties>
	using ViewTypeScalar = Kokkos::View<T,properties...>;

/// 1D veiw as a vector 
template<typename T, typename... properties>
	using ViewType1D = Kokkos::View<T*,properties...>;

/// 2D view as an array
template<typename T, typename... properties>
	using ViewType2D = Kokkos::View<T**,properties...>;

/// 3D view as an array
template<typename T, typename... properties>
	using ViewType3D = Kokkos::View<T***,properties...>;

/// 1D dual view as a vector 
template<typename T, typename... properties>
	using DualViewType1D = Kokkos::DualView<T*,properties...>;

/// unordered map
template<typename Key, typename Value, typename... properties>
	using unorderedMap = Kokkos::UnorderedMap<Key, Value, properties...>;

/// unordered set 
template<typename Key, typename... properties>
	using unorderedSet = Kokkos::UnorderedMap<Key, void, properties...>;

/// Scalar on device 
template<typename T>
	using deviceViewTypeScalar = Kokkos::View<T>;

/// 1D array (vector) with default device (memory space and execution space)
template<typename T>
	using deviceViewType1D = Kokkos::View<T*>;

/// 2D view on device as an array on device 
template<typename T, typename Layout=void>
	using deviceViewType2D = Kokkos::View<T**,Layout, void>;

/// 3D view on device as an array on device 
template<typename T, typename Layout=void>
	using deviceViewType3D = Kokkos::View<T***,Layout, void>;



template<typename T>
	using hostViewTypeScalar = Kokkos::View<T, Kokkos::HostSpace>;

/// 1D array (vector with host memeory space)
template<typename T>
	using hostViewType1D = Kokkos::View<T*, Kokkos::HostSpace>;

/// 2D array on host 
template<typename T, typename Layout=void>
	using hostViewType2D = Kokkos::View<T**,Layout, Kokkos::HostSpace>;

/// 3D array on host
template<typename T, typename Layout=void>
	using hostViewType3D = Kokkos::View<T***,Layout, Kokkos::HostSpace>;

/// 1D vector on device with atomic capabilities
template<typename T>
using deviceAtomicViewType1D = 
    Kokkos::View<
		T*,
		Kokkos::MemoryTraits<std::is_same<DefaultExecutionSpace,Serial>::value?0:Kokkos::Atomic>>;

/// 3D array on device with atomic capabilities
template<typename T>
using deviceAtomicViewType3D =
	Kokkos::View<
	T***,
	Kokkos::MemoryTraits<std::is_same<DefaultExecutionSpace,Serial>::value?0:Kokkos::Atomic>>;


} // pFlow

#endif  //__KokkosTypes_hpp__
