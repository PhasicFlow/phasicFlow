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


#ifndef __Vector_hpp__
#define __Vector_hpp__

#include <vector>
#include <algorithm>

#include "typeInfo.hpp"
#include "error.hpp"
#include "uniquePtr.hpp"
#include "stdAlgorithms.hpp"
#include "span.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"

#include "stdVectorHelper.hpp"

#ifndef __RESERVE__
#define __RESERVE__
	struct RESERVE{};
#endif

namespace pFlow
{
 

template<typename T, typename Allocator>
class Vector;

#include "VectorFwd.hpp"




template<typename T, typename Allocator = vecAllocator<T> >
class Vector
: 
	public std::vector<T, Allocator>
{
public:
	
	using VectorType 		= Vector<T, Allocator>;

	using vectorType 		= typename std::vector<T, Allocator>;

	using iterator 			= typename vectorType::iterator;
	
	using const_iterator 	= typename vectorType::const_iterator;
	
	using reference 		= typename vectorType::reference;
	
	using const_reference 	= typename vectorType::const_reference;
	
	using value_type 		= T; 		

	using pointer 			= T *;

	using const_pointer 	= const T *;

	using init_list 		= typename std::initializer_list<T>;

private:

	// - name of the vector  
	word name_;


	static constexpr bool  isHostAccessible_ = true;

	
  	

public:

	// - Type info
	TypeInfoTemplateNV111("Vector", T, memoerySpaceName());

	//// - Constructors

		/// Empty Vector
		inline Vector()
		:
			Vector("Vector")
		{}


		/// Empty Vector with a name 
		inline 
		explicit Vector(const word& name)
		:
			name_(name)
		{}

		
		/// Vector with specified length and name
		inline 
		Vector(const word& name, size_t len)
		:
			vectorType(len),
			name_(name)
		{}

		/// Vector with name, length and value
		inline 
		Vector(const word& name, size_t len, const T& val)
		:
			Vector(name, len)
		{
			this->assign(len, val);
		}

		/// Vector with name, size and reserved capacity 
		Vector(const word& name, size_t cap, size_t len, RESERVE ):
			name_(name)
		{
			this->reserve(cap);
			this->resize(len);
		}


		/// Vector from name and initializer list 
		inline 
		Vector(const word& name, const init_list &l)
		:
			vectorType(l),
			name_(name)
		{}

		/// Construct with a name and form std::vector (host memory)
		inline Vector(const word& name, const vectorType& src)
		:
			vectorType(src),
			name_(name)
		{}

		/// Construct with a name and form std::vector (host memory)
		/// and with a desired capacity. 
		inline Vector(const word& name, const vectorType& src, size_t cap)
		:
			Vector(name, cap, src.size(), RESERVE())
		{
			this->assign(src.begin(), src.end());
		}

		/// Copy construct 
		inline Vector(const VectorType& src) = default;

		/// Copy from src with a new name 
		inline Vector(const word& name, const Vector<T>& src):
			vectorType(src),
			name_(name)
		{}

		/// Copy assignment
		inline VectorType& operator=( const VectorType& rhs ) = default;

		/// Copy assignment from std::vector 
		inline VectorType& operator=(const vectorType& rhs)
		{
			Vector::assign(rhs.begin(), rhs.end());
			return *this;
		}

		/// Move construct 
		Vector( VectorType && mv) noexcept= default;
	
		/// Move assignment 
		VectorType& operator=( VectorType && mv)noexcept = default;
	
		/// Scalar assignment 
		inline void operator=(const T& val)
		{
			fill(val);
		}

		/// Destructor 
		~Vector() = default;
		

		/// Clone as a uniquePtr
		inline uniquePtr<VectorType> clone() const
		{
			return makeUnique<VectorType>(*this);
		}

		/// Clone as a pointer 
		inline VectorType* clonePtr()const
		{
			uniquePtr<VectorType> Ptr = makeUnique<VectorType>(*this);
			return Ptr.release();
		}

	
	//// - Methods 

		/// Access to this, mostly used by derived classes 
		const VectorType& VectorField() const
		{
			return *this;
		}
	 
		VectorType& VectorField()
		{
			return *this;
		}

		const vectorType& vectorField()const
		{
			return *this;
		}

		vectorType& vectorField()
		{
			return *this;
		}

		auto& deviceVectorAll()
		{
			return *this;
		}

		const auto& deviceVectorAll()const
		{
			return *this;
		}

		auto& deviceVector()
		{
			return *this;
		}

		const auto& deviceVector()const
		{
			return *this;
		}


		/// Name of the vector 
		const word& name()const
		{
			return name_;
		}

		/// Size of the vector 
		inline auto size()const
		{
			return vectorType::size();
		}

		/// Capacity of the vector 
		inline auto capacity()const
		{
			return vectorType::capacity();
		}

		/// If vector is empty 
		inline bool empty()const
		{
			return vectorType::empty();
		}

		/// Reserve capacity for vector 
		/// Preserve the content.
		inline void reserve(size_t cap)
		{
			vectorType::reserve(cap);
		}

		// - fill the whole content of vector, [begin, end), with val 
		void fill( const T& val);

		inline
		auto getSpan()
		{
			return span<T>(this->data(), this->size());
		}
		
		inline 
		auto getSpan()const
		{
			return span<T>(const_cast<T*>(this->data()), this->size());
		}

		static constexpr bool isHostAccessible()
		{
			return isHostAccessible_;
		}

	void operator +=( const T& val);
	void operator -=( const T& val);
	void operator *=( const T& val);
	void operator /=( const T& val);

	void operator +=( const VectorType& v );
	void operator -=( const VectorType& v );
	void operator /=( const VectorType& v );
	void operator *=( const VectorType& v );

	VectorType operator -()const;

	bool read(iIstream& is)
	{
		return readStdVector(is, vectorField());
	}
	bool write(iOstream& os)const
	{
		return writeStdVector(os, vectorField());
	}

	bool read(iIstream& is, const IOPattern& iop)
	{
		return readStdVector(is, vectorField(), iop);
	}

	bool write(iOstream& os, const IOPattern& iop)const
	{
		return writeStdVector(os, vectorField(), iop);
	}

	static
	constexpr const char* memoerySpaceName()
  	{
  		return "std";
  	}
};


template<typename T, typename Allocator>
inline iIstream& operator >> (iIstream & is, Vector<T, Allocator> & ivec )
{
	if( !ivec.read(is) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

template<typename T, typename Allocator> 
inline iOstream& operator << (iOstream& os, const Vector<T, Allocator>& ovec )
{	
	if( !ovec.write(os) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}
	return os; 
}


} // pFlow


#include "VectorI.hpp"
#include "Vector.cpp"
#include "VectorMath.hpp"
#include "VectorAlgorithm.hpp"

#endif



	/*// - delete elemens of vector based on sorted indices 
	//   return false if out of range
	bool deleteElement_sorted(const Vector<label>& indices );

	// - delete elemens of vector based on indices 
	//   return false if out of range
	bool deleteElement(const Vector<label>& indices );

	// - delete elment with index 
	//   return false if out of range
	bool deleteElement(label index);

	/// Sort elements based on the indices
	void sortItems(const int32IndexContainer& indices);

	// - set or insert new elements into the vector 
	//   return false if it fails
	bool insertSetElement(const  int32IndexContainer& indices, const T& val);

	// - set or insert new elements into the vector 
	//   return false if it fails
	bool insertSetElement(const  int32IndexContainer& indices, const Vector<T>& vals);
	
	// - set or insert new elements into the vector 
	//   return false if it fails
	bool insertSetElement(const  Vector<int32>& indices, const T& val);

	// - set or insert new elements into the vector 
	//   return false if it fails
	bool insertSetElement(const  Vector<int32>& indices, const Vector<T>& vals);

	// - set or insert a new element into the vecor 
	//   return false if it fails 
	inline bool insertSetElement(int32 idx, const T& val);*/