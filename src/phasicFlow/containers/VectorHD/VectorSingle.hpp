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


#ifndef __VectorSingle_hpp__
#define __VectorSingle_hpp__

#include "globalSettings.hpp"
#include "types.hpp"
#include "typeInfo.hpp"
#include "Vector.hpp"
#include "indexContainer.hpp"

#include "streams.hpp"

#include "KokkosTypes.hpp"
#include "ViewAlgorithms.hpp"


#ifndef __RESERVE__
#define __RESERVE__
	struct RESERVE{};
#endif

namespace pFlow
{


template<typename T, typename MemorySpace>
class VectorSingle;


template<typename T, typename MemorySpace=void>
class VectorSingle
{
public:
	
	// viewType (view of data host and device)
  	using VectorType		= VectorSingle<T, MemorySpace>;

  	using iterator        = T*;

  	using constIterator   = const T*;
  	
	using reference       = T&;
  	
  	using constReference  = const T&;

	using valueType       = T;
  	
  	using pointer         = T*;
  	
  	using constPointer    = const T*;
  	
  	// type defs related to Kokkos 
  	using viewType 			= ViewType1D<T, MemorySpace>;

  	using deviceType 		= typename viewType::device_type;

  	using memory_space 		= typename viewType::memory_space;

  	using execution_space 	= typename viewType::execution_space;

protected:
	
	size_t 			size_ = 0;

	size_t 			capacity_ = 0;

	viewType   		view_;

	mutable viewType	subView_;

	mutable bool		subViewUpdated_ = false;

	static const inline real 	growthFactor_ = vectorGrowthFactor__;

	static constexpr bool  isHostAccessible_ =
  	Kokkos::SpaceAccessibility<execution_space,Kokkos::HostSpace>::accessible;

  	constexpr static inline const char* memoerySpaceName()
  	{
  		return memory_space::name();
  	}

	static INLINE_FUNCTION_H size_t evalCapacity(size_t n)
	{
		return static_cast<size_t>(n*growthFactor_+1);
	}

	// use actualCap = true only for reserve
	INLINE_FUNCTION_H void changeSize(size_t n, bool actualCap=false)
	{
		if(n > capacity_ )
		{
			if(actualCap)
				capacity_ = n;
			else
				capacity_ = evalCapacity(n);

			Kokkos::resize(view_, capacity_);
			subViewUpdated_ = false;
		}
		if(!actualCap) 
		{
			setSize(n);
		}
	}

	INLINE_FUNCTION_H void setSize(size_t n)
	{
		size_ = n;
		subViewUpdated_ = false;
	}

	// - update subview 
	INLINE_FUNCTION_H void updateSubView()const
	{
		if(subViewUpdated_) return;

		subView_ 	= Kokkos::subview(view_, Kokkos::make_pair(0,int(size_)));
		subViewUpdated_ = true;
	}
	
	
public:

	// - type info
	TypeInfoTemplateNV2("VectorSingle", T, memoerySpaceName());

	//// - Constructors

		// - empty constructor
		VectorSingle()
		:
			VectorSingle("VectorSingle")
		{}

		// empty vector with a name 
		VectorSingle(const word& name)
		:
			size_(0),
			capacity_(2),
			view_(name,capacity_)
		{
			changeSize(size_);
		}

		// - a vector with size n
		VectorSingle(size_t n)
		:
			VectorSingle("VectorSingle",n)
		{}

		// - a vector with name and size n
		VectorSingle(const word& name, size_t n)
		:
			size_(n),
			capacity_(evalCapacity(n)),
			view_(name, capacity_)
		{
			changeSize(size_);
		}

		// a vector with size and value 
		VectorSingle(size_t n, const T& val)
		:
			VectorSingle("VectorSingle", n , val)
		{}

		// a vector with name, size and value
		VectorSingle(const word& name, size_t n, const T& val)
		:
			VectorSingle(name, n)
		{
			assign(n, val);
		}

		// a vector with name and reserved capacity 
		VectorSingle(size_t cap, size_t n, RESERVE )
		:
			VectorSingle("VectorSingle", cap, n, RESERVE())
		{}


		// a vector with name and reserved capacity 
		VectorSingle(const word& name, size_t cap, size_t n, RESERVE )
		:
			VectorSingle(name)
		{
			reallocate(cap);
			size_ = n;
		}

		// - construct from pFlow::Vector (host memory)
		VectorSingle(const Vector<T> & src)
		:
			VectorSingle("VectorSingle", src)
		{}

		// - construct from pFlow::Vector and name 
		VectorSingle(const word& name, const Vector<T> & src)
		:
			VectorSingle(name)
		{
			assign(src);
		}

		// - copy construct (perform deep copy)
		VectorSingle(const VectorSingle& src)
		:
			VectorSingle(src.name(), src.capacity(), src.size(), RESERVE())
		{	
			copy(deviceVectorAll(), src.deviceVectorAll());
			////Kokkos::deep_copy(deviceVectorAll(), src.deviceVectorAll());
		}

		// - copy construct with a new name 
		VectorSingle(const word& name, const VectorSingle& src)
		:
			VectorSingle(name, src.capacity(), src.size(), RESERVE())
		{
			copy(deviceVectorAll(), src.deviceVectorAll());	
			////Kokkos::deep_copy(deviceVectorAll(), src.deviceVectorAll());
		}

		// - copy assignment 
		VectorSingle& operator = (const VectorSingle& rhs)
		{
			if(&rhs == this) return *this;
			VectorSingle temp(rhs);
			capacity_   = temp.capacity();
			size_ 		= temp.size();	
			view_   = temp.view_;
			subViewUpdated_ = false;

			return *this;
		}

		// no move construct
		VectorSingle(VectorSingle&&) = delete;

		// no move assignment
		VectorSingle& operator= (VectorSingle&&) = delete;


		// - clone as a uniquePtr
		INLINE_FUNCTION_H 
		uniquePtr<VectorSingle> clone() const
		{
			return makeUnique<VectorSingle>(*this);
		}

		// - clone as a pointer
		INLINE_FUNCTION_H
		VectorSingle* clonePtr()const
		{
			return new VectorSingle(*this);
		}

	//// - Methods

		// - return *this
		INLINE_FUNCTION_H
		VectorType& VectorField()
		{
			return *this;
		}

		// - return *this
		INLINE_FUNCTION_H
		const VectorType& VectorField()const
		{
			return *this;
		}


		// - Device vector range [0,capcity)
		INLINE_FUNCTION_H 
		viewType& deviceVectorAll(){
			return view_;
		}

		// - Device vector range [0,capacity)
		INLINE_FUNCTION_H 
		const viewType& deviceVectorAll() const {
			return view_;
		}

		//  - Device vector range [0, size)
		INLINE_FUNCTION_H 
		viewType& deviceVector(){
			updateSubView();
			return subView_;
		}

		//  - Device vector range [0, size)
		INLINE_FUNCTION_H 
		const viewType& deviceVector()const{
			updateSubView();
			return subView_;
		}

		INLINE_FUNCTION_H 
		const auto hostVectorAll()const
		{
			auto hView = Kokkos::create_mirror_view(view_);
			copy(hView, view_);
			return hView;
		}

		INLINE_FUNCTION_H 
		auto hostVectorAll()
		{
			auto hView = Kokkos::create_mirror_view(view_);
			copy(hView, view_);
			return hView;
		}

		INLINE_FUNCTION_H 
		const auto hostVector()const
		{
			auto hView = Kokkos::create_mirror_view(deviceVector());
			copy(hView, deviceVector());
			return hView;
		}

		INLINE_FUNCTION_H 
		auto hostVector()
		{
			auto hView = Kokkos::create_mirror_view(deviceVector());
			copy(hView, deviceVector());
			return hView;
		}

		// - name of vector 
		INLINE_FUNCTION_H 
		const word name()const
		{
			return view_.label();
		}
		
		// - size of vector 
		INLINE_FUNCTION_H 
		size_t size()const
		{
			return size_;
		}

		// - capcity of vector 
		INLINE_FUNCTION_H 
		size_t capacity()const
		{
			return capacity_;
		}

		// - if vector is empty 
		INLINE_FUNCTION_H 
		bool empty()const
		{
			return size_==0;
		}

		// - reserve capacity for vector 
		//   preserve the content
		INLINE_FUNCTION_H 
		void reserve(size_t cap)
		{
			changeSize(cap, true);
		}

		// - reallocate memory
		INLINE_FUNCTION_H void reallocate(size_t cap)
		{
			capacity_ = cap;
			size_ = 0;
			reallocNoInit(view_, capacity_);
			subViewUpdated_ = false;
		}

		INLINE_FUNCTION_H void reallocate(size_t cap, size_t size)
		{
			capacity_ = cap;
			size_ = size;
			reallocNoInit(view_, capacity_);
			subViewUpdated_ = false;
		}

		// resize the vector  
		INLINE_FUNCTION_H 
		void resize(size_t n){
			changeSize(n);
		}

		// resize the view and assign value to the most recent view (most updated)
		INLINE_FUNCTION_H 
		void resize(size_t n, const T& val) {
			assign(n, val);
		}

		// - clear the vector 
		INLINE_FUNCTION_H 
		void clear() {
			 size_ = 0;
			 subViewUpdated_ = false;

		}

		// - fill the range [0,size) with val
		INLINE_FUNCTION_H 
		void fill(const T& val)
		{
			if(empty())return;
			pFlow::fill(deviceVectorAll(),0 ,size_ ,val);
		}
		
		// - host calls only
		// - assign n first elements to val
		//   resize view
		//   assign value to either side (device/host)
		INLINE_FUNCTION_H
		void assign(size_t n, const T& val)
		{
			if(capacity()<n)
			{
				this->reallocate(evalCapacity(n));
			}
			size_ = n;
			this->fill(val);
		}

		// - host calls only
		// - assign source vector
		//   resize views
		//   assign to both sides (device&host)
		INLINE_FUNCTION_H void assign(const Vector<T>& src)
		{
			auto srcSize = src.size();
			if( capacity() < srcSize ) 
			{
				this->reallocate( src.capacity() );
			}
			size_ = srcSize;
			
			// - unmanaged view in the host
			hostViewType1D<const T> temp(src.data(), srcSize );
			copy(deviceVector(), temp);
		}


		//TODO: change it to parallel version 
		// - delete elements from vector 
		//   similar memory spaces 
		/*template<class indT, class MSpace>
		INLINE_FUNCTION_H 
			typename std::enable_if<
				Kokkos::SpaceAccessibility<
					execution_space, typename VectorSingle<indT,MSpace>::memory_space>::accessible,
					bool>::type
		deleteElement
		(
			const VectorSingle<indT,MSpace>& sortedIndices
		)
		{
			
			auto& indices = sortedIndices.deviceVectorAll();
			auto& dVec = deviceVectorAll();
			indT  numInd = sortedIndices.size();
			indT  oldSize = this->size();

			if( numInd == 0 )return true;
			
			// an scalalr
			
			Kokkos::parallel_for(1, LAMBDA_HD(int nn)
			 	{
			 		(void)nn;	
			 		indT n = 0;
					indT nextInd = indices[0];
					indT j = indices[0];
					for(label i=indices[0]; i < oldSize; ++i)
					{
						if( n < numInd && i == nextInd )
						{
							++n;
							nextInd = indices[n];
						}
						else
						{
							dVec[j] = dVec[i];
							++j;	
						}				
					}

			 	});
			typename viewType::execution_space().fence();
			size_ = oldSize - indices.size();
			subViewUpdated_ = false;

			return true;
		}

		// different memory spaces 
		template<class indT, class MSpace>
		INLINE_FUNCTION_H 
		typename std::enable_if<
				! Kokkos::SpaceAccessibility<
					execution_space, typename VectorSingle<indT,MSpace>::memory_space>::accessible,
					bool>::type
		deleteElement
		(
			const VectorSingle<indT,MSpace>& sortedIndices
		)
		{
			
			notImplementedFunction;
		}*/

		INLINE_FUNCTION_H
		bool insertSetElement(const int32IndexContainer& indices, const T& val)
		{
			if(indices.empty()) return true;
			auto maxInd = indices.max();

			if(this->empty() || maxInd > size()-1 )
			{
				resize(maxInd+1);
			}

			
			using policy = Kokkos::RangePolicy<
			execution_space,
			Kokkos::IndexType<int32> >;
			auto dVec = deviceVectorAll();
			auto dIndex = indices.deviceView();
			
			Kokkos::parallel_for(
				"insertSetElement",
				policy(0,indices.size()), LAMBDA_HD(int32 i){	
				dVec(dIndex(i))= val;
				});
			Kokkos::fence();

			return true;
		}

		INLINE_FUNCTION_H
		void sortItems(const int32IndexContainer& indices)
		{
			if(indices.size() == 0)
			{
				setSize(0);
				return;
			}
			
			size_t 		newSize = indices.size();
			viewType 	sortedView("sortedView", newSize);

			using policy = Kokkos::RangePolicy<
			execution_space,
			Kokkos::IndexType<int32> >;

			auto d_indices = indices.deviceView();
			auto d_view = view_;
			Kokkos::parallel_for(
				"sortItems",
				newSize, 
				LAMBDA_HD(int32 i){	
					sortedView(i) = d_view(d_indices(i));
				});

			Kokkos::fence();

			setSize(newSize);

			copy(deviceVector(), sortedView);
			
			return;

		}

		INLINE_FUNCTION_H
		bool insertSetElement(const int32IndexContainer& indices, const Vector<T>& vals)
		{

			if(indices.size() == 0)return true;
			if(indices.size() != vals.size())return false;

			auto maxInd = indices.max(); 
			
			if(this->empty() || maxInd > size()-1 )
			{
				resize(maxInd+1);
			} 

				
			hostViewType1D<const T> hVecVals( vals.data(), vals.size());
			deviceViewType1D<T> dVecVals("dVecVals", indices.size());

			copy(dVecVals, hVecVals);

			using policy = Kokkos::RangePolicy<
			execution_space,
			Kokkos::IndexType<int32> >;
			auto dVec = deviceVectorAll();
			auto dIndex = indices.deviceView();
		
			Kokkos::parallel_for(
				"insertSetElement",
				policy(0,indices.size()), LAMBDA_HD(int32 i){	
				dVec(dIndex(i))= dVecVals(i);
				});
			Kokkos::fence();

			return true;
			
		}

		INLINE_FUNCTION_H
		bool insertSetElement(const Vector<int32>& indices, const T& val)
		{
			if(indices.empty()) return true;

			auto maxInd = max(indices);
			
			if(this->empty() || maxInd > size()-1 )
			{
				resize(maxInd+1);
			}

			if constexpr (isHostAccessible_)
			{
				hostViewType1D<int32> hostView(const_cast<int32*>(indices.data()), indices.size());
				fillSelected(deviceVectorAll(), hostView, indices.size(), val);
				return true;
			
			}else
			{

				// TODO: remove the const_cast 
				hostViewType1D<int32> hostView(const_cast<int32*>(indices.data()), indices.size());
				deviceViewType1D<int32> dView("dView", indices.size());
				copy(dView, hostView);
				fillSelected(deviceVectorAll(), dView, indices.size(), val);
				return true;
			}

			return false;
		}

		INLINE_FUNCTION_H
		bool insertSetElement(const Vector<int32>& indices, const Vector<T>& vals)
		{
			if(indices.size() == 0)return true;
			if(indices.size() != vals.size())return false;

			auto maxInd = max(indices); 
			
			if(this->empty() || maxInd > size()-1 )
			{
				resize(maxInd+1);
			} 

			if constexpr (isHostAccessible_)
			{
				// TODO: remove const_cast
				hostViewType1D<int32> hVecInd( const_cast<int32*>(indices.data()), indices.size());
				hostViewType1D<T> hVecVals( const_cast<T*>(vals.data()), vals.size());

				fillSelected(deviceVectorAll(), hVecInd, hVecVals, indices.size());
				return true;
			
			}else
			{
				
				// TODO: remove const_cast
				hostViewType1D<int32> hVecInd( const_cast<int32*>(indices.data()), indices.size());
				deviceViewType1D<int32> dVecInd("dVecInd", indices.size());

				hostViewType1D<T> hVecVals( const_cast<T*>(vals.data()), vals.size());
				deviceViewType1D<T> dVecVals("dVecVals", indices.size());
				
				copy(dVecVals, hVecVals);
				copy(dVecInd, hVecInd);

				fillSelected(deviceVectorAll(), dVecInd, dVecVals, indices.size());
				return true;
			}

			return false;
		}

		INLINE_FUNCTION_H
		bool append(const deviceViewType1D<T>& dVec, size_t numElems)
		{

			if(numElems == 0 )return true;
			auto oldSize = size_;
			auto newSize = size_ + numElems;

			if(this->empty() || newSize > capacity() )
			{
				resize(newSize);
			}
			else
			{
				size_ = size_+numElems;
			}
			
			auto dSubView = Kokkos::subview(view_, Kokkos::make_pair(oldSize, newSize)); 
			copy(dSubView, dVec);

			return true;
		}

		INLINE_FUNCTION_H
		bool append(const VectorSingle& Vec)
		{
			return append(Vec.deviceVector(), Vec.size());
		}

		// - host calls only
		//   push a new element at the end
		//   resize if necessary
		//   works on host accessible vector 
		template<bool Enable = true>
		typename std::enable_if<
			isHostAccessible_ && Enable,
			void>::type
		push_back(const T& val)
		{
			if(size_ == capacity_) 
			{
				changeSize(evalCapacity(capacity_), true);
			}
			data()[size_++] = val;
			subViewUpdated_ = false;
		}

		INLINE_FUNCTION_H pointer data(){
			return view_.data();
		}

		INLINE_FUNCTION_H constPointer data()const{
			return view_.data();
		}

		// - host calls only
		//   works on host accessible vector 
		// 	 returns begin iterator 
		template<bool Enable = true>
		INLINE_FUNCTION_H 
		typename std::enable_if_t<
			isHostAccessible_ && Enable,
			iterator>
		begin(){
			return data();
		}

		// - host calls only
		//   works on host accessible vector 
		//   returns begin iterator 
		template<bool Enable = true>
		INLINE_FUNCTION_H 
		typename std::enable_if<
			isHostAccessible_ && Enable,
			constIterator>::type
		begin()const {
			return data();
		}

		// - host calls only
		//   works on host accessible vector 
		//   returns end iterator
		template<bool Enable = true> 
		INLINE_FUNCTION_H
		typename std::enable_if<
			isHostAccessible_ && Enable,
			iterator>::type
		end(){
			return size_ > 0 ? data() + size_: data();
		}

		// host call
		// returns end iterator 
		template<bool Enable = true>
		INLINE_FUNCTION_H
		typename std::enable_if<
			isHostAccessible_ && Enable,
			constIterator>::type
		end()const{
			return size_ > 0 ? data() + size_: data();
		}
				
		// operator to be used on host side vectors
		template<bool Enable = true>
		INLINE_FUNCTION_H 
		typename std::enable_if<
			isHostAccessible_ && Enable,
			reference>::type
		operator[](label i){
			return view_[i];
		}

		template<bool Enable = true>
		INLINE_FUNCTION_H 
		typename std::enable_if<
			isHostAccessible_ && Enable,
			constReference>::type
		 operator[](label i)const{
			return view_[i];
		}

	//// - IO operations

		FUNCTION_H
		bool readVector(
			iIstream& is,
			size_t len=0)
		{
			Vector<T> vecFromFile;
			if( !vecFromFile.readVector(is,len) ) return false;

			this->assign(vecFromFile);

			return true;
		}

		FUNCTION_H
		bool read(iIstream& is)
		{
			return readVector(is);
		}

		FUNCTION_H
		bool write(iOstream& os)const
		{
			
			Vector<T, noConstructAllocator<T>> vecToFile(this->size());
			
			const auto dVec = Kokkos::subview(view_, Kokkos::make_pair(0,int(size_)));
			hostViewType1D<T> mirror(vecToFile.data(), vecToFile.size());
			copy(mirror,dVec);

			return vecToFile.write(os);
		}

}; // class VectorSingle

template<typename T, typename MemorySpace>
inline iIstream& operator >> (iIstream & is, VectorSingle<T, MemorySpace> & ivec )
{
	if( !ivec.read(is) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

template<typename T, typename MemorySpace>
inline iOstream& operator << (iOstream& os, const VectorSingle<T, MemorySpace>& ovec )
{
	
	if( !ovec.write(os) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}

	return os; 
}




} // - pFlow

#include "VectorSingleAlgorithms.hpp"


#endif //__VectorSingle_hpp__

