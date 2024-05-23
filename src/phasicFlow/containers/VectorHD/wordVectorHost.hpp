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
#ifndef __wordVectorHost_hpp__
#define __wordVectorHost_hpp__

#include "VectorSingle.hpp"
#include "Vector.hpp"

namespace pFlow
{


template<>
class VectorSingle<word, HostSpace>
{
public:
	
	//- typedefs for accessing data

		using VectorType		= VectorSingle<word, HostSpace>;

		using VectorTypeHost 	= VectorSingle<word, HostSpace>; 		

		using iterator        = word*;

		using const_iterator   = const word*;
	  	
		using reference       = word&;
	  	
	 	using const_reference  = const word&;

		using value_type       = word;
	  	
		using pointer         = word*;
	  	
		using const_pointer    = const word*;
  	
	//- typedefs related to memory management 

		using viewType 			= ViewType1D<word, HostSpace>;

		using device_type 		= typename viewType::device_type;

		using memory_space 		= typename viewType::memory_space;

		using execution_space 	= typename viewType::execution_space;

private:
	
	// - Data members

		Vector<word>              container_;

        mutable viewType          unManagedView_;

	// - protected members and methods 		

		/// Is the memory of this vector accessible from Host
		static constexpr 
		bool  isHostAccessible_ = true; 

		/// Is the memory of this vector accessiple from Divce
		static constexpr
		bool isDeviceAccessible_ = false; 

		static constexpr
		bool isTriviallyCopyable_ = false; 

	
public:

	/// Type info
	TypeInfoTemplateNV111("VectorSingle", word , memoerySpaceName());

	//// - Constructors

		/// Empty vector
		VectorSingle() = default;
		
		/// Empty vector with a name (capacity = 2) 
		explicit VectorSingle(const word& name)
        :
            container_(name)
        {}
		
		/// Vector with name and size n
		VectorSingle(const word& name, uint32 n)
        :
            container_(name, n)
        {}
		
		/// Vector with name, size and value
		VectorSingle(const word& name, uint32 n, const word& val)
        :
            container_(name, n, val)
        {}

		/// Vector with name, size (n) and reserved capacity 
		VectorSingle(const word& name, uint32 cap, uint32 n, const RESERVE& r )
        :
            container_(name, cap, n, r)
        {}
		
		/// Construct with a name and form std::vector (host memory)
		VectorSingle(const word& name, const std::vector<word> & src)
        :
            container_(name, src)
        {} 

		/// Construct with a name and form std::vector (host memory) and with a desired capacity. 
		VectorSingle(const word& name, const std::vector<word> & src, uint32 cap)
        :
            container_(name, src, cap)
        {}
		
		/// Copy construct (performs deep copy)
		VectorSingle(const VectorSingle& src)=default;
		
    	/// Copy construct with a new name (perform deep copy)
		VectorSingle(const word& name, const VectorSingle& src)
        :
            container_(name, src.container_)
        {}

				
		/// Copy assignment (perform deep copy from rhs to *this)
		VectorSingle& operator = (const VectorSingle& rhs)=default;
		
		/// Move construct
		VectorSingle(VectorSingle&&) = default;   

		/// Move assignment
		VectorSingle& operator= (VectorSingle&&) = default; 

		/// @brief Descructor
        /// This may not destroy the underlying memory, sice view is 
		/// shared_ptr and maybe referenced by another object too 
		~VectorSingle() = default;

		/// Clone as a uniquePtr (perform deep copy)
		INLINE_FUNCTION_H 
		uniquePtr<VectorSingle> clone() const
        {
            return makeUnique<VectorSingle>(*this);
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

		/// Device view range [0,capcity)
		INLINE_FUNCTION_H 
		auto& deviceViewAll()
        {
            // return un-managed view 
            unManagedView_ = viewType(container_.data(), container_.capacity());
            return unManagedView_;
        }

		/// Device view range [0,capcity)
		INLINE_FUNCTION_H 
		const auto& deviceViewAll() const
        {
            // return un-managed view 
            unManagedView_ = viewType(const_cast<word*>(container_.data()), container_.capacity());
            return unManagedView_;
        }

		///  Device view range [0, size)
		INLINE_FUNCTION_H 
		auto deviceView()const
        {
            return viewType(const_cast<word*>(container_.data()), container_.size());
        }

		/// Return a view accessible on Host in range [0,capacity)
		INLINE_FUNCTION_H 
		auto hostViewAll()const
        {
            return viewType(const_cast<word*>(container_.data()), container_.capacity());
        }
		

		/// Return a view accessible on Host in range [0,size)
		INLINE_FUNCTION_H 
		auto hostView()const
        {
            return viewType(const_cast<word*>(container_.data()), container_.size());
        }

		/// Name of the vector 
		INLINE_FUNCTION_H 
		word name()const
        {
            return container_.name();
        }
		
		
		/// Size of the vector 
		INLINE_FUNCTION_H 
		uint32 size()const
        {
            return container_.size();
        }
		

		// Capcity of the vector 
		INLINE_FUNCTION_H 
		uint32 capacity()const
        {
            return container_.capacity();
        }
		
		/// If vector is empty 
		INLINE_FUNCTION_H 
		bool empty()const
        {
            return container_.size()==0uL;
        }

		
		/// Reserve capacity for vector 
		/// Preserve the content.
		INLINE_FUNCTION_H 
		void reserve(uint32 cap)
        {
            container_.reserve(cap);
        } 

		/// Reallocate memory to new cap and set size to 0. 
		/*INLINE_FUNCTION_H 
		void reallocate(uint32 cap); 
		
		/// Reallocate memory to new cap and set size to newSize. 
		/// Do not preserve the content
		INLINE_FUNCTION_H 
		void reallocate(uint32 cap, uint32 newSize);*/
		
		/// Resize the vector and preserve the content
		INLINE_FUNCTION_H   
		void resize(uint32 n)
        {
            container_.resize(n);
        }

		/// Resize the vector and assign the value to it. 
		INLINE_FUNCTION_H  
		void resize(uint32 n, const word& val)
        {
            container_.resize(n, val);
        }

		/// Clear the vector, but keep the allocated memory unchanged 
		INLINE_FUNCTION_H 
		void clear()
        {
            container_.clear();
        }
		
		/// Fill the range [0,size) with val
		INLINE_FUNCTION_H 
		void fill(const word& val)
        {
            container_.fill(val);
        }

		INLINE_FUNCTION_H
		void fill(rangeU32 r,  const word& val)
        {
            container_.fill(r.start(), r.end(), val);
        }
		
		/// Change size of the vector and assign val to vector and 
		INLINE_FUNCTION_H
		void assign(size_t n, const word& val)
        {
            container_.assign(n, val);
        }
		
		/// Assign source vector with specified capacity.
		/// The size of *this becomes the size of src. 
		INLINE_FUNCTION_H 
		void assign(const std::vector<word>& src, uint32 cap)
        {
            container_.reserve(cap);
            this->assign(src);
        }
		

		/// Assign source vector.
		/// The size of *this becomes the size of src. 
		/// The capacity of *this becomes the capacity of src.
		INLINE_FUNCTION_H 
		void assign(const std::vector<word>& src)
        {
            container_.assign(src.begin(), src.end());
        }

		/// Assign source vector from host side.
		/// The size of *this becomes the size of src. 
		/// The capacity of *this becomes the capacity of src.
		INLINE_FUNCTION_H
		void assignFromHost(const VectorTypeHost& src)
        {
            notImplementedFunction;
            fatalExit;
        }

		INLINE_FUNCTION_H
		void assign(const VectorType& src, bool srcCapacity = true)
        {
            notImplementedFunction;
            fatalExit;
        }

		template<typename MSpace>
		INLINE_FUNCTION_H
		void assignFromDevice(const VectorSingle<word, MSpace>& src, bool srcCapacity = true)
		{
			notImplementedFunction;
            fatalExit;
		}

		/*INLINE_FUNCTION_H
		void append(const ViewType1D<T,MemorySpace>& appVec);

		INLINE_FUNCTION_H
		void append(const std::vector<T>& appVec);

		INLINE_FUNCTION_H
		void append(const VectorType& appVec);*/
		
		INLINE_FUNCTION_H
		auto getSpan()
        {
            return span<word>(container_.data(), container_.size());
        }
				
		INLINE_FUNCTION_H 
		auto getSpan()const
        {
            return span<word>(const_cast<word*>(container_.data()), container_.size());
        }
		
		
		bool insertSetElement(const uint32IndexContainer& indices, const word& val);
        
		
		
		bool insertSetElement(const uint32IndexContainer& indices, const std::vector<word>& vals);
        
		
		
		bool insertSetElement(
			const uint32IndexContainer& indices, 
			const ViewType1D<word, memory_space> vals
        );
        

		INLINE_FUNCTION_H
		bool reorderItems(const uint32IndexContainer& indices)
        {
            notImplementedFunction;
            return false;
        }

		/// @brief push a new element at the end (host call only)
		///  resize if necessary and works on host accessible vector.
		
		void push_back(const word& val)
		{
			container_.push_back(val);
		}

		INLINE_FUNCTION_H 
		pointer data(){
			return container_.data();
		}

		INLINE_FUNCTION_H 
		const_pointer data()const{
			return container_.data();
		}

		/// Return begin iterator. It works when devices is host accessible.
		auto
		begin(){
			return container_.begin();
		}

		/// Return begin iterator. it works when host is accessible.
		const auto
		begin()const {
			return container_.begin();
		}
		
		auto end(){
			return container_.end();
		}

		/// Return end iterator. it works when host is accessible.
		const auto end()const{
			return container_.end();
		}
				
		/// Return reference to element i. it works when host is accessible.		
		word& operator[](size_t i){
			return container_[i];
		}

		const word& operator[](size_t i)const{
			return container_[i];
		}

	//// - IO operations

		/// Read vector from stream
		FUNCTION_H
		bool read(iIstream& is)
		{
			return container_.read(is);
		}

		/// Read vector from stream
		FUNCTION_H
		bool read(iIstream& is, const IOPattern& iop)
		{
			return container_.read(is, iop);
		}

		/// Write the vector to os
		FUNCTION_H
		bool write(iOstream& os, const IOPattern& iop)const
		{
			return container_.write(os, iop);
		}

		FUNCTION_H
		bool write(iOstream& os)const
		{
			return container_.write(os);	
		}

		template<typename HostMask>
		FUNCTION_H
		bool write(iOstream& os, const IOPattern& iop, const HostMask& mask)const
		{
			notImplementedFunction;
			return false;
		}


		/// Name of the memory space
		static  
		constexpr const char* memoerySpaceName()
		{
			return memory_space::name();
		}

}; // class wordVectorHost

inline iIstream& operator >> (iIstream & is, VectorSingle<word, HostSpace> & ivec )
{
	if( !ivec.read(is) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

inline iOstream& operator << (iOstream& os, const VectorSingle<word, HostSpace>& ovec )
{
	
	if( !ovec.write(os) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}

	return os; 
}


} // - pFlow



#endif


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