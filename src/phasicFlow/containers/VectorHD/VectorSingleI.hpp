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
bool pFlow::VectorSingle<T,MemorySpace>::insertSetElement(uint32IndexContainer indices, const T& val)
{
	if(indices.empty()) return true;
	
	auto maxInd = indices.max();

	if(this->empty() || maxInd > size()-1 )
	{
		resize(maxInd+1);
	}

	using policy = Kokkos::RangePolicy<executionSpace,Kokkos::IndexType<uint32>>;

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

	if(indices.size() == 0)return true;
	if(indices.size() != vals.size())return false;

	auto maxInd = indices.max(); 
	
	if(this->empty() || maxInd > size()-1 )
	{
		resize(maxInd+1);
	} 
	
	using policy = Kokkos::RangePolicy<executionSpace,Kokkos::IndexType<uint32>>;

	hostViewType1D<const T> hVals( vals.data(), vals.size());

	if constexpr( isDeviceAccessible_ )
	{
		deviceViewType1D<T> dVals("dVals", indices.size());
		copy(dVals, hVals);
		auto dVec = view_;
		auto ind = indices.deviceView();

		Kokkos::parallel_for(
			"VectorSingle::insertSetElement",
			policy(0,indices.size()), LAMBDA_HD(int32 i){	
				dVec(ind(i))= dVals(i);}
			);

		Kokkos::fence();

	}
	else
	{
		auto dVec = view_;
		auto ind = indices.hostView();

		Kokkos::parallel_for(
			"VectorSingle::insertSetElement",
			policy(0,indices.size()), LAMBDA_HD(int32 i){	
				dVec(ind(i))= hVals(i);}
			);

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

	using policy = Kokkos::RangePolicy< executionSpace,Kokkos::IndexType<uint32>>;

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

	copy(deviceVector(), sortedView);
	
	return true;
}