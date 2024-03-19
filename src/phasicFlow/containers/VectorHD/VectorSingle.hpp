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
#include "phasicFlowKokkos.hpp"
#include "stdVectorHelper.hpp"
#include "error.hpp"
#include "indexContainer.hpp"
#include "streams.hpp"



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

		using VectorTypeHost 	= VectorSingle<T, HostSpace>; 		

		using iterator        = T*;

		using const_iterator   = const T*;
	  	
		using reference       = T&;
	  	
	 	using const_reference  = const T&;

		using value_type       = T;
	  	
		using pointer         = T*;
	  	
		using const_pointer    = const T*;
  	
	//- typedefs related to memory management 

		using viewType 			= ViewType1D<T, MemorySpace>;

		using device_type 		= typename viewType::device_type;

		using memory_space 		= typename viewType::memory_space;

		using execution_space 	= typename viewType::execution_space;

private:
	
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
		bool  isHostAccessible_ = isHostAccessible<execution_space>();

		/// Is the memory of this vector accessiple from Divce
		static constexpr
		bool isDeviceAccessible_ = isDeviceAccessible<execution_space>();

		static constexpr
		bool isTriviallyCopyable_ = std::is_trivially_copyable_v<T>;

	  	/// Evaluate capacity based on the input size 
		static INLINE_FUNCTION_H uint32 evalCapacity(uint32 n)
		{
			return static_cast<uint32>(n*growthFactor_+1);
		}

	/// @brief Change size to n and preserve the conetent if realloc 
	/// occurs 
	INLINE_FUNCTION_H 
	uint32 changeSize(uint32 n, bool withInit= false);

    /// @brief Change the size and capacity of Vector
	INLINE_FUNCTION_H
	uint32 changeCapacitySize(uint32 actualCap, uint32 n, bool withInit= false);
	
	INLINE_FUNCTION_H
	void changeCapacity(uint32 actualCap, bool withInit= false);
	
	INLINE_FUNCTION_H
	uint32 reallocateCapacitySize(uint32 cap, uint32 s);
	
	INLINE_FUNCTION_H 
	uint32 setSize(uint32 n);
	
public:

	/// Type info
	TypeInfoTemplateNV111("VectorSingle", T, memoerySpaceName());

	//// - Constructors

		/// Empty vector
		VectorSingle();
		
		/// Empty vector with a name (capacity = 2) 
		explicit VectorSingle(const word& name);
		
		/// Vector with name and size n
		VectorSingle(const word& name, uint32 n);
		
		/// Vector with name, size and value
		VectorSingle(const word& name, uint32 n, const T& val);

		/// Vector with name, size (n) and reserved capacity 
		VectorSingle(const word& name, uint32 cap, uint32 n, RESERVE );
		
		/// Construct with a name and form std::vector (host memory)
		VectorSingle(const word& name, const std::vector<T> & src); 

		/// Construct with a name and form std::vector (host memory) and with a desired capacity. 
		VectorSingle(const word& name, const std::vector<T> & src, uint32 cap);
		
		/// Copy construct (performs deep copy)
		VectorSingle(const VectorSingle& src);
		
    	/// Copy construct with a new name (perform deep copy)
		VectorSingle(const word& name, const VectorSingle& src);
		
		/// Copy assignment (perform deep copy from rhs to *this)
		VectorSingle& operator = (const VectorSingle& rhs) ;
		
		/// Move construct
		VectorSingle(VectorSingle&&) = default;   

		/// Move assignment
		VectorSingle& operator= (VectorSingle&&) = default; 

		/// @brief Descructor
        /// This may not destroy the underlying memory, sice view is 
		/// shared_ptr and maybe referenced by another object too 
		~VectorSingle();

		/// Clone as a uniquePtr (perform deep copy)
		INLINE_FUNCTION_H 
		uniquePtr<VectorSingle> clone() const;

			
	//// - Methods

		/// Return *this
		INLINE_FUNCTION_H
		VectorType& VectorField();

		/// Return *this
		INLINE_FUNCTION_H
		const VectorType& VectorField()const;

		/// Device view range [0,capcity)
		INLINE_FUNCTION_H 
		auto& deviceViewAll();

		/// Device view range [0,capcity)
		INLINE_FUNCTION_H 
		const auto& deviceViewAll() const;

		///  Device view range [0, size)
		INLINE_FUNCTION_H 
		auto deviceView()const;

		/// Return a view accessible on Host in range [0,capacity)
		INLINE_FUNCTION_H 
		auto hostViewAll()const;
		

		/// Return a view accessible on Host in range [0,size)
		INLINE_FUNCTION_H 
		auto hostView()const;

		/// Name of the vector 
		INLINE_FUNCTION_H 
		word name()const;
		
		
		/// Size of the vector 
		INLINE_FUNCTION_H 
		uint32 size()const;
		

		// Capcity of the vector 
		INLINE_FUNCTION_H 
		uint32 capacity()const;
		
		/// If vector is empty 
		INLINE_FUNCTION_H 
		bool empty()const;
		
		/// Reserve capacity for vector 
		/// Preserve the content.
		INLINE_FUNCTION_H 
		void reserve(uint32 cap); 

		/// Reallocate memory to new cap and set size to 0. 
		INLINE_FUNCTION_H 
		void reallocate(uint32 cap); 
		
		/// Reallocate memory to new cap and set size to newSize. 
		/// Do not preserve the content
		INLINE_FUNCTION_H 
		void reallocate(uint32 cap, uint32 newSize); 
		
		/// Resize the vector and preserve the content
		INLINE_FUNCTION_H   
		void resize(uint32 n);

		/// Resize the vector and assign the value to it. 
		INLINE_FUNCTION_H  
		void resize(uint32 n, const T& val);

		/// Clear the vector, but keep the allocated memory unchanged 
		INLINE_FUNCTION_H 
		void clear();
		
		/// Fill the range [0,size) with val
		INLINE_FUNCTION_H 
		void fill(const T& val);

		INLINE_FUNCTION_H
		void fill(rangeU32 r,  const T& val);
		
		/// Change size of the vector and assign val to vector and 
		INLINE_FUNCTION_H
		void assign(size_t n, const T& val);
		
		/// Assign source vector with specified capacity.
		/// The size of *this becomes the size of src. 
		INLINE_FUNCTION_H 
		void assign(const std::vector<T>& src, uint32 cap); 
		

		/// Assign source vector.
		/// The size of *this becomes the size of src. 
		/// The capacity of *this becomes the capacity of src.
		INLINE_FUNCTION_H 
		void assign(const std::vector<T>& src);

		/// Assign source vector from host side.
		/// The size of *this becomes the size of src. 
		/// The capacity of *this becomes the capacity of src.
		INLINE_FUNCTION_H
		void assign(const VectorTypeHost& src);

		INLINE_FUNCTION_H
		void append(const std::vector<T>& appVec);
		
		INLINE_FUNCTION_H
		auto getSpan();
				
		INLINE_FUNCTION_H 
		auto getSpan()const;
		
		INLINE_FUNCTION_H
		bool insertSetElement(uint32IndexContainer indices, const T& val);
		
		INLINE_FUNCTION_H
		bool insertSetElement(uint32IndexContainer indices, const std::vector<T>& vals);
		
		INLINE_FUNCTION_H
		bool insertSetElement(
			uint32IndexContainer indices, 
			const ViewType1D<T, memory_space> vals);

		INLINE_FUNCTION_H
		bool reorderItems(uint32IndexContainer indices);

		/// @brief push a new element at the end (host call only)
		///  resize if necessary and works on host accessible vector.
		template<bool Enable = true>
		INLINE_FUNCTION_H
		typename std::enable_if_t<isHostAccessible_ && Enable, void>
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
		const_pointer data()const{
			return view_.data();
		}

		/// Return begin iterator. It works when devices is host accessible.
		template<bool Enable = true>
		INLINE_FUNCTION_H 
		typename std::enable_if_t<isHostAccessible_ && Enable,iterator>
		begin(){
			return data();
		}

		/// Return begin iterator. it works when host is accessible.
		template<bool Enable = true>
		INLINE_FUNCTION_H 
		typename std::enable_if_t<isHostAccessible_ && Enable,const_iterator>
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
		typename std::enable_if_t<isHostAccessible_ && Enable,const_iterator>
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
		typename std::enable_if_t<isHostAccessible_ && Enable,const_reference>
		 operator[](size_t i)const{
			return view_[i];
		}

	//// - IO operations

		/// Read vector from stream
		FUNCTION_H
		bool read(iIstream& is)
		{
			std::vector<T> vecFromFile;
			if(! readStdVector(is, vecFromFile)) return false;

			this->assign(vecFromFile);

			return true;
		}

		/// Read vector from stream
		FUNCTION_H
		bool read(iIstream& is, const IOPattern& iop)
		{
			std::vector<T> vecFromFile;
			if(! readStdVector(is, vecFromFile, iop)) return false;
			this->assign(vecFromFile);

			return true;
		}

		/// Write the vector to os
		FUNCTION_H
		bool write(iOstream& os, const IOPattern& iop)const
		{
			auto hVec = hostView();
			auto sp = span<T>( const_cast<T*>(hVec.data()), hVec.size());
			
			return writeSpan(os, sp, iop);
			
		}

		FUNCTION_H
		bool write(iOstream& os)const
		{
			auto hVec = hostView();
			auto sp = span<T>( const_cast<T*>(hVec.data()), hVec.size());
			
			return writeSpan(os, sp);
			
		}

		template<typename HostMask>
		FUNCTION_H
		bool write(iOstream& os, const IOPattern& iop, const HostMask& mask)const
		{
			auto hVec = hostView();
			
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

			return writeSpan(os, sp, iop);

		}


		/// Name of the memory space
		static  
		constexpr const char* memoerySpaceName()
		{
			return memory_space::name();
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
#include "VectorSingle.cpp"




#endif //__VectorSingle_hpp__


/*
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
			return append(Vec.deviceView(), Vec.size());
		}*/