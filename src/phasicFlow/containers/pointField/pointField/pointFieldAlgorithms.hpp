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


namespace pFlow
{


const inline auto ActivePoint = pointStructure::ACTIVE;

template<class T, typename... properties>
T maxActive_serial(
	const Kokkos::View<T*, properties...>& view,
	const Kokkos::View<int8*, Kokkos::LayoutLeft, Kokkos::HostSpace>& flag,
	size_t start,
	size_t end
	)
{
	T maxValue = largestNegative<T>();
	for(label i=start; i<end; ++i)
	{
		if(flag[i] == ActivePoint )maxValue = max(maxValue, view[i]);
	}
	return maxValue;
}

// to be executed on host 
template<typename T, typename... properties>
INLINE_FUNCTION_H
T maxActiveH(
	const Kokkos::View<T*, properties...>& view,
	const Kokkos::View<int8*, Kokkos::LayoutLeft, Kokkos::HostSpace>& flag,
 	size_t start,
 	size_t end
 	) 
{

	T maxValue = largestNegative<T>();

	auto RP = Kokkos::RangePolicy<
	Kokkos::IndexType<size_t>,
	typename Kokkos::View<T, properties...>::execution_space >(start, end);

	Kokkos::parallel_reduce("pointFieldAlgorithms-maxActive",
							RP, 
							LAMBDA_HD(label i, T& valueToUpdate){
								if(flag[i] == ActivePoint) valueToUpdate = max(view[i],valueToUpdate);
							},
							Kokkos::Max<T>( maxValue )
							);
	return maxValue;
}


// to be executed on device 
template<typename T, typename... properties>
INLINE_FUNCTION_H
T maxActiveD(
	const Kokkos::View<T*, properties...>& view,
	const Kokkos::View<int8*, Kokkos::LayoutLeft>& flag,
 	size_t start,
 	size_t end
 	) 
{

	T maxValue = largestNegative<T>();

	auto RP = Kokkos::RangePolicy<
	Kokkos::IndexType<size_t>,
	typename Kokkos::View<T, properties...>::execution_space >(start, end);

	Kokkos::parallel_reduce("pointFieldAlgorithms-maxActive",
							RP, 
							LAMBDA_HD(label i, T& valueToUpdate){
								if(flag[i] == ActivePoint) valueToUpdate = max(view[i],valueToUpdate);
							},
							Kokkos::Max<T>( maxValue )
							);
	return maxValue;
}

template<class T, class MemorySpace>
T maxActive(const pointField<VectorSingle, T, MemorySpace>& pField)
{
	
	// if all points are active, perfrom a simple max
	if( pField.allActive() )
		return max(pField.VectorField());

	const auto len = pField.size();
	if constexpr ( pField.isHostAccessible())
	{
		if(len < sizeToSerial__ ) // serial execution instead of lunching parallel execution 
			return maxActive_serial(
				pField.deviceViewAll(),
				pField.pointFlag().hostViewAll(),
				static_cast<size_t>(0),
				len
				);
		else
			return maxActiveH(
				pField.deviceViewAll(),
				pField.pointFlag().hostViewAll(),
				static_cast<size_t>(0),
				len
				);
	}
	else
	{
		return maxActiveD(
			pField.deviceViewAll(),
			pField.pointFlag().deviceViewAll(),
			static_cast<size_t>(0),
			len
			);
	}

	// to remove the warning of CUDAC++ compiler when dealing with constexpr
	return 0;
}

template<class side, class T, class MemorySpace=void>
T maxActive(const pointField<VectorDual, T, MemorySpace>& pField)
{
	if( pField.allActive() )
		return max(pField.VectorField());

	auto len = pField.size();

	if constexpr (std::is_same<side,HostSide>::value)
	{
		if( len < sizeToSerial__)
			return maxActive_serial(
				pField.hostViewAll(),
				pField.pointFlag().hostViewAll(),
				static_cast<size_t>(0),
				len
				);
		else
			return maxActiveH(
				pField.hostViewAll(),
				pField.pointFlag().hostViewAll(),
				static_cast<size_t>(0),
				len
				);
	}else
	{
		return maxActiveD(
				pField.deviceViewAll(),
				pField.pointFlag().deviceViewAll(),
				static_cast<size_t>(0),
				len
				);
	}

	return 0;
}

/*template<typename T>
	void inline fillActive(pointField<T>& field, const T& val)
{
	if(field.pStruct().allActive)
	{
		fill_n(field, field.size(), val);
		return;
	}
	ForAll(i, field)
	{
		if(field.pStruct().isActive(i)) field[i] = val;
	}
}

template<typename T>
	void inline fillMarkedDelete( pointField<T>& field, const T& val)
{
	if(field.pStruct().allActive())return;
	ForAll(i,field)
	{
		if(!field.pStruct().isActive(i)) field[i] = val;
	}
}

template<typename T>
inline auto countActive(const pointField<T>& field, const T& val)
{
	if(field.pStruct().allActive())
	{
		return count(field, val);
	}
	else
	{
		return count(field, val, [&](label i){return field.pointField().isActive(i);});
	}
}

template<typename T, typename UnaryPredicate>
inline auto for_eachActive(pointField<T>& field, UnaryPredicate func)
{
	if(field.pStruct().allActive())
	{
		ForAll(i,field)
		{
			func(i);
		}
	}
	else
	{
		ForAll(i, field)
		{
			if(field.pStruct().isActive(i)) func(i);
		}
	}
	return func;
}

template<typename T, typename UnaryPredicate>
inline bool for_eachActiveBreak(pointField<T>& field, UnaryPredicate func)
{
	if(field.pStruct().allActive())
	{
		ForAll(i,field)
		{
			if(!func(i))return false;
		}
	}
	else
	{
		ForAll(i, field)
		{
			if(field.pStruct().isActive(i))
			{
			 	if(!func(i)) return false;
			}
		}
	}
	return true;
}

template<typename T, typename UnaryPredicate>
inline auto for_eachMarkedDelete(pointField<T>& field, UnaryPredicate func)
{
	if(field.pStruct().allActive()) return func;
		
	ForAll(i, field)
	{
		if(!field.pStruct().isActive(i)) func(i);
	}
	return func;
}*/


}