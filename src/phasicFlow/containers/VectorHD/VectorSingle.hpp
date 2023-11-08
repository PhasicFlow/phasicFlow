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

#include <vector>


#include "globalSettings.hpp"
#include "types.hpp"
#include "error.hpp"
#include "indexContainer.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"
#include "span.hpp"
#include "Vector.hpp"
#include "phasicFlowKokkos.hpp"
#include "dataIO.hpp"


#ifndef __RESERVE__
#define __RESERVE__
	struct RESERVE{};
#endif

namespace pFlow
{

//- Forward 
template<typename T, typename MemorySpace>
class VectorSingle;



template<typename T, typename MemorySpace=void>
class VectorSingle
{
public:
	
	//- typedefs for accessing data

		using VectorType		= VectorSingle<T, MemorySpace>;

		using iterator        = T*;

		using constIterator   = const T*;
	  	
		using reference       = T&;
	  	
	 	using constReference  = const T&;

		using valueType       = T;
	  	
		using pointer         = T*;
	  	
		using constPointer    = const T*;
  	
	//- typedefs related to memory management 

		using viewType 			= ViewType1D<T, MemorySpace>;

		using deviceType 		= typename viewType::device_type;

		using memorySpace 		= typename viewType::memory_space;

		using executionSpace 	= typename viewType::execution_space;

protected:
	
	// - Data members

		/// Size of the vector 
		uint32 			size_ = 0;

		/// view of the vector 
		viewType   		view_;

	// - protected members and methods 

		/// growth factor for vector 
		static const inline 
		real 	growthFactor_ = vectorGrowthFactor__;

		/// Is the memory of this vector accessible from Host
		static constexpr 
		bool  isHostAccessible_ = isHostAccessible<executionSpace>();
			//Kokkos::SpaceAccessibility<executionSpace,HostSpace>::accessible;

		/// Is the memory of this vector accessiple from Divce
		static constexpr
		bool isDeviceAccessible_ = isDeviceAccessible<executionSpace>();


		/// Name of the memory space
	  	constexpr static inline 
	  	const char* memoerySpaceName()
	  	{
	  		return memorySpace::name();
	  	}

	  	/// Evaluate capacity based on the input size 
		static INLINE_FUNCTION_H uint32 evalCapacity(uint32 n)
		{
			return static_cast<uint32>(n*growthFactor_+1);
		}

	/// Change size to n and preserve the conetent if realloc 
	/// occurs 
	INLINE_FUNCTION_H 
	uint32 changeSize(uint32 n, bool withInit= false)
	{
		if(n > this->capacity() )
		{
			uint32 newCap = evalCapacity(n);
			changeCapacity(newCap, withInit);
		}
		return setSize(n);
	}

	INLINE_FUNCTION_H
	uint32 changeCapacitySize(uint32 actualCap, uint32 n, bool withInit= false)
	{
		changeCapacity(actualCap, withInit);
		return setSize(n);
	}

	INLINE_FUNCTION_H
	void changeCapacity(uint32 actualCap, bool withInit= false)
	{
		if(withInit)
		{
			resizeInit(view_, actualCap);
		}
		else
		{
			resizeNoInit(view_, actualCap);
		}
	}

	INLINE_FUNCTION_H
	uint32 reallocateCapacitySize(uint32 cap, uint32 s)
	{
		reallocNoInit(view_, cap);
		return setSize(s);
	}

	INLINE_FUNCTION_H 
	uint32 setSize(uint32 n)
	{
		auto os = size_;
		size_ = n;
		return os;
	}
	
public:

	// - type info
	TypeInfoTemplateNV2("VectorSingle", T, memoerySpaceName());

	//// - Constructors

		/// Empty vector
		VectorSingle()
		:
			VectorSingle("VectorSingle")
		{}

		/// Empty vector with a name 
		VectorSingle(const word& name)
		:
			size_(0),
			view_(name,2)
		{}

		
		/// Vector with name and size n
		VectorSingle(const word& name, uint32 n)
		:
			size_(n),
			view_(name, evalCapacity(n))
		{}

		

		/// Vector with name, size and value
		VectorSingle(const word& name, uint32 n, const T& val)
		:
			VectorSingle(name, n)
		{
			assign(n, val); 
		}


		/// Vector with name, size (n) and reserved capacity 
		VectorSingle(const word& name, uint32 cap, uint32 n, RESERVE )
		:
			size_(n),
			view_(name, cap)
		{}

		/// Construct with a name and form std::vector (host memory)
		VectorSingle(const word& name, const std::vector<T> & src) 
		:
			VectorSingle(name)
		{
			assign(src);
		}

		/// Construct with a name and form std::vector (host memory)
		/// and with a desired capacity. 
		VectorSingle(const word& name, const std::vector<T> & src, uint32 cap) 
		:
			VectorSingle(name)
		{
			assign(src, cap);
		}

		/// Copy construct (performs deep copy)
		VectorSingle(const VectorSingle& src)
		:
			VectorSingle(src.name(), src)
		{	
			//copy(deviceVector(), src.deviceVector());
		}

		/// Copy construct with a new name (perform deep copy)
		VectorSingle(const word& name, const VectorSingle& src)
		:
			VectorSingle(name, src.capacity(), src.size(), RESERVE())
		{
			copy(deviceVector(), src.deviceVector());	
		}

		/// Copy assignment (perform deep copy from rhs to *this)
		VectorSingle& operator = (const VectorSingle& rhs) 
		{
			if(&rhs == this) return *this;
			VectorSingle temp(rhs);

			size_ 	= temp.size();	
			view_   = temp.view_;

			return *this;
		}

		/// Move construct
		VectorSingle(VectorSingle&&) = default;   

		/// Move assignment
		VectorSingle& operator= (VectorSingle&&) = default; 

		/// This may not destroy the underlying memory, sice view is 
		/// shared_ptr and maybe referenced by another object too 
		~VectorSingle()
		{
			view_ = viewType();
			size_ = 0;
		}

		/// Clone as a uniquePtr (perform deep copy)
		INLINE_FUNCTION_H 
		uniquePtr<VectorSingle> clone() const  
		{
			return makeUnique<VectorSingle>( this->name(), *this);
		}

		/// Clone as a pointer (perform deep copy)
		INLINE_FUNCTION_H
		VectorSingle* clonePtr()const   
		{
			return new VectorSingle(this->name(), *this);
		}

	//// - Methods

		/// Return *this
		INLINE_FUNCTION_H
		VectorType& VectorField()
		{
			return *this;
		}

		/// Return *this
		INLINE_FUNCTION_H
		const VectorType& VectorField()const
		{
			return *this;
		}

		/// Device vector range [0,capcity)
		INLINE_FUNCTION_H 
		viewType deviceVectorAll() const{
			return view_;
		}

		///  Device vector range [0, size)
		INLINE_FUNCTION_H 
		viewType deviceVector()const
		{
			return Kokkos::subview(view_, Kokkos::make_pair(0,int(size_)));
		}

		/// Return a vector accessible on Host in range [0,capacity)
		INLINE_FUNCTION_H 
		auto hostVectorAll()const
		{
			auto hView = Kokkos::create_mirror_view(view_);
			copy(hView, view_);
			return hView;
		}

		/// Return a vector accessible on Host in range [0,size)
		INLINE_FUNCTION_H 
		auto hostVector()const
		{
			auto hView = Kokkos::create_mirror_view(deviceVector());
			copy(hView, deviceVector());
			return hView;
		}

		/// Name of the vector 
		INLINE_FUNCTION_H 
		const word name()const
		{
			return view_.label();
		}
		
		/// Size of the vector 
		INLINE_FUNCTION_H 
		uint32 size()const
		{
			return size_;
		}

		// Capcity of the vector 
		INLINE_FUNCTION_H 
		uint32 capacity()const
		{
			return view_.extent(0);
		}

		/// If vector is empty 
		INLINE_FUNCTION_H 
		bool empty()const
		{
			return size_==0;
		}

		/// Reserve capacity for vector 
		/// Preserve the content.
		INLINE_FUNCTION_H 
		void reserve(uint32 cap) 
		{
			changeCapacity(cap);
		}

		/// Reallocate memory to new cap and set size to 0. 
		INLINE_FUNCTION_H 
		void reallocate(uint32 cap) 
		{
			reallocateCapacitySize(cap, 0);			
		}

		/// Reallocate memory to new cap and set size to newSize. 
		/// Do not preserve the content
		INLINE_FUNCTION_H 
		void reallocate(uint32 cap, uint32 newSize) 
		{
			reallocateCapacitySize(cap, newSize);			
		}

		/// Resize the vector and preserve the content
		INLINE_FUNCTION_H   
		void resize(uint32 n){
			changeSize(n);
		}

		/// Resize the vector and assign the value to it. 
		INLINE_FUNCTION_H  
		void resize(uint32 n, const T& val) {
			assign(n, val);
		}

		/// Clear the vector, but keep the allocated memory unchanged 
		INLINE_FUNCTION_H 
		void clear() 
		{
			 size_ = 0;
		}

		/// Fill the range [0,size) with val
		INLINE_FUNCTION_H 
		void fill(const T& val)
		{
			if(empty())return;
			pFlow::fill(view_, rangeU32(0 ,size_) ,val);
		}
		
		/// Change size of the vector and assign val to vector and 
		INLINE_FUNCTION_H
		void assign(size_t n, const T& val)
		{
			if( n > capacity() )
			{
				reallocateCapacitySize(evalCapacity(n), n);
			}
			else
			{
				setSize(n);	
			}
			this->fill(val);
		}

		
		/// Assign source vector with specified capacity.
		/// The size of *this becomes the size of src. 
		INLINE_FUNCTION_H 
		void assign(const std::vector<T>& src, uint32 cap) 
		{
			uint32 srcSize = src.size();
			
			if(cap != capacity())
			{
				reallocateCapacitySize(cap, srcSize);
			}
			else
			{
				setSize(srcSize);
			}
			
			// - unmanaged view in the host
			hostViewType1D<const T> temp(src.data(), srcSize );
			copy(deviceVector(), temp);
		}

		/// Assign source vector.
		/// The size of *this becomes the size of src. 
		/// The capacity of *this becomes the capacity of src.
		INLINE_FUNCTION_H 
		void assign(const std::vector<T>& src) 
		{
			assign(src, src.capacity());
		}


		INLINE_FUNCTION_H
		bool insertSetElement(uint32IndexContainer indices, const T& val);
		
		INLINE_FUNCTION_H
		bool insertSetElement(uint32IndexContainer indices, const std::vector<T>& vals);
		
		INLINE_FUNCTION_H
		bool reorderItems(uint32IndexContainer indices);


		/*INLINE_FUNCTION_H
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
		}*/

		// - host calls only
		//   push a new element at the end
		//   resize if necessary
		//   works on host accessible vector 
		template<bool Enable = true>
		INLINE_FUNCTION_H
		typename std::enable_if<isHostAccessible_ && Enable, void>::type
		push_back(const T& val)
		{
			auto n = changeSize(size_+1);
			data()[n] = val;	
		}

		INLINE_FUNCTION_H 
		pointer data(){
			return view_.data();
		}

		INLINE_FUNCTION_H 
		constPointer data()const{
			return view_.data();
		}

		/// Return begin iterator. it works when host is accessible.
		template<bool Enable = true>
		INLINE_FUNCTION_H 
		typename std::enable_if_t<isHostAccessible_ && Enable,iterator>
		begin(){
			return data();
		}

		/// Return begin iterator. it works when host is accessible.
		template<bool Enable = true>
		INLINE_FUNCTION_H 
		typename std::enable_if_t<isHostAccessible_ && Enable,constIterator>
		begin()const {
			return data();
		}

		
		/// Return end iterator. it works when host is accessible.
		template<bool Enable = true> 
		INLINE_FUNCTION_H
		typename std::enable_if_t<isHostAccessible_ && Enable,iterator>
		end(){
			return size_ > 0 ? data() + size_: data();
		}

		
		/// Return end iterator. it works when host is accessible.
		template<bool Enable = true>
		INLINE_FUNCTION_H
		typename std::enable_if_t<isHostAccessible_ && Enable,constIterator>
		end()const{
			return size_ > 0 ? data() + size_: data();
		}
				
		/// Return reference to element i. it works when host is accessible.
		template<bool Enable = true>
		INLINE_FUNCTION_H 
		typename std::enable_if_t<isHostAccessible_ && Enable,reference>
		operator[](size_t i){
			return view_[i];
		}

		/// Return reference to element i. it works when host is accessible.
		template<bool Enable = true>
		INLINE_FUNCTION_H 
		typename std::enable_if_t<isHostAccessible_ && Enable,constReference>
		 operator[](size_t i)const{
			return view_[i];
		}

	//// - IO operations

		/// Read vector from stream (ASCII)
		FUNCTION_H
		bool read(iIstream& is, IOPattern::IOType iotype)
		{
			std::vector<T> vecFromFile;
			if(! readStdVector(is, vecFromFile, iotype)) return false;

			this->assign(vecFromFile);

			return true;
		}

		/// Write the vector to os
		FUNCTION_H
		bool write(iOstream& os, IOPattern::IOType iotype)const
		{
			auto hVec = hostVector();
			auto sp = span<T>( const_cast<T*>(hVec.data()), hVec.size());
			
			return writeSpan(os, sp, iotype);
			
		}

		template<typename HostMask>
		FUNCTION_H
		bool write(iOstream& os, IOPattern::IOType iotype, const HostMask& mask)const
		{
			auto hVec = hostVector();
			
			auto numActive = mask.numActive();
			std::vector<T> finalField;
			finalField.clear();
			finalField.reserve(numActive);
			
			uint32 start 	= mask.activeRange().start();
			uint32 end 		= mask.activeRange().end();
			
			for(uint32 i=start; i<end; i++ )
			{
				if( mask() )
					finalField.push_back(hVec[i]);
			}

			auto sp = span<T>( finalField.data(), finalField.size());

			return writeSpan(os, sp, iotype);

		}

}; // class VectorSingle

template<typename T, typename MemorySpace>
inline iIstream& operator >> (iIstream & is, VectorSingle<T, MemorySpace> & ivec )
{
	if( !ivec.read(is, IOPattern::MasterProcessorOnly ) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

template<typename T, typename MemorySpace>
inline iOstream& operator << (iOstream& os, const VectorSingle<T, MemorySpace>& ovec )
{
	
	if( !ovec.write(os, IOPattern::AllProcessorsDifferent) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}

	return os; 
}


} // - pFlow

#include "VectorSingleAlgorithms.hpp"
#include "VectorSingleI.hpp"


#endif //__VectorSingle_hpp__

