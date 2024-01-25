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

#ifndef __internalFieldAlgorithms_hpp__
#define __internalFieldAlgorithms_hpp__

namespace pFlow
{

template<class T, class MemorySpace>
inline 
T min(const internalField<T,MemorySpace>& iField)
{   
    using exeSpace = typename internalField<T,MemorySpace>::execution_space;

    using policy = Kokkos::RangePolicy<
			exeSpace,
			Kokkos::IndexType<uint32> >;

    if constexpr(isDeviceAccessible<exeSpace>())
    {
        // this is a device view 
        auto maskD = iField.activePointsMaskDevice();
        auto aRange = maskD.activeRange();
        auto filed = iField.field().deviceVectorAll();

        T minElement;
        
        Kokkos::parallel_reduce(
		"min(internalField)",
		policy(aRange.start(), aRange.end() ),
		LAMBDA_HD(uint32 i, T& minUpdate)
        {
			if( maskD(i) )
                if(filed[i] < minUpdate) minUpdate = filed[i];
		},
		Kokkos::Min<T>(minElement));
	    return minElement;
    }
    else
    {
        // this is a host view 
        auto maskH = iField.activePointsMaskHost();
        auto aRange = maskH.activeRange();
        auto filed = iField.field().deviceVectorAll();
        T minElement;
        Kokkos::parallel_reduce(
            "min(internalField)",
            policy(aRange.start(), aRange.end()),
            LAMBDA_HD(uint32 i, T& minUpdate)
            {
                if( maskH(i) )
                    if(filed[i] < minUpdate) minUpdate = filed[i];
            },
            Kokkos::Min<T>(minElement));

	    return minElement;
    }
}

template<class T, class MemorySpace>
inline 
T max(const internalField<T,MemorySpace>& iField)
{   
    using exeSpace = typename internalField<T,MemorySpace>::execution_space;

    using policy = Kokkos::RangePolicy<
			exeSpace,
			Kokkos::IndexType<uint32> >;

    if constexpr(isDeviceAccessible<exeSpace>())
    {
        // this is a device view 
        auto maskD = iField.activePointsMaskDevice();
        auto aRange = maskD.activeRange();
        auto filed = iField.field().deviceVectorAll();

        T maxElement;
        
        Kokkos::parallel_reduce(
		"max(internalField)",
		policy(aRange.start(), aRange.end() ),
		LAMBDA_HD(uint32 i, T& maxUpdate)
        {
			if( maskD(i) )
                if( maxUpdate <filed[i]) maxUpdate = filed[i];
		},
		Kokkos::Max<T>(maxElement));
	    return maxElement;
    }
    else
    {
        // this is a host view 
        auto maskH = iField.activePointsMaskHost();
        auto aRange = maskH.activeRange();
        auto filed = iField.field().deviceVectorAll();

        T maxElement;
        
        Kokkos::parallel_reduce(
		"max(internalField)",
		policy(aRange.start(), aRange.end() ),
		LAMBDA_HD(uint32 i, T& maxUpdate)
        {
			if( maskH(i) )
                if( maxUpdate <filed[i]) maxUpdate = filed[i];
		},
		Kokkos::Max<T>(maxElement));
	    return maxElement;
    }
}


template<class T, class MemorySpace>
inline 
Pair<T,T> minMax(const internalField<T,MemorySpace>& iField)
{   
    using exeSpace = typename internalField<T,MemorySpace>::execution_space;

    using policy = Kokkos::RangePolicy<
			exeSpace,
			Kokkos::IndexType<uint32> >;

    if constexpr(isDeviceAccessible<exeSpace>())
    {
        // this is a device view 
        auto maskD = iField.activePointsMaskDevice();
        auto aRange = maskD.activeRange();
        auto filed = iField.field().deviceVectorAll();

        T minElement;
        T maxElement;
        
        Kokkos::parallel_reduce(
		"minMax(internalField)",
		policy(aRange.start(), aRange.end() ),
		LAMBDA_HD(uint32 i, T& minUpdate, T& maxUpdate)
        {
			if( maskD(i) )
            {
                if(maxUpdate < filed[i]) maxUpdate = filed[i];
                if(filed[i] < minUpdate) minUpdate = filed[i];
            }
                
		},
		Kokkos::Min<T>(minElement),
        Kokkos::Max<T>(maxElement)
        );
	    return {minElement, maxElement};
    }
    else
    {
        // this is a host view 
        auto maskH = iField.activePointsMaskHost();
        auto aRange = maskH.activeRange();
        auto filed = iField.field().deviceVectorAll();

        T minElement;
        T maxElement;
        
        Kokkos::parallel_reduce(
		"minMax(internalField)",
		policy(aRange.start(), aRange.end() ),
		LAMBDA_HD(uint32 i, T& minUpdate, T& maxUpdate)
        {
			if( maskH(i) )
            {
                if(maxUpdate < filed[i]) maxUpdate = filed[i];
                if(filed[i] < minUpdate) minUpdate = filed[i];
            }
                
		},
		Kokkos::Min<T>(minElement),
        Kokkos::Max<T>(maxElement)
        );
	    return {minElement, maxElement};
    }
}



}





#endif