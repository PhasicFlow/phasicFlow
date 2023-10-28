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
	
	typedef Vector<T, Allocator>					VectorType;

	typedef typename std::vector<T, Allocator>		vectorType;

	typedef typename vectorType::iterator 			iterator;
	
	typedef typename vectorType::const_iterator 	constIterator;
	
	typedef typename vectorType::reference 			reference;
	
	typedef typename vectorType::const_reference 	constReference;
	
	typedef  T 										valueType; 		

	typedef  T* 									pointer;

	typedef const T*  								constPointer;

	typedef typename std::initializer_list<T> 		initList;

protected:

	// - name of the vector  
	word name_;


	static constexpr bool  isHostAccessible_ = true;

	constexpr static inline const char* memoerySpaceName()
  	{
  		return "std";
  	}
  	

public:

	// - Type info
	TypeInfoTemplateNV2("Vector", T, memoerySpaceName());

	//// - Constructors

		/// Empty Vector
		inline Vector()
		:
			Vector("Vector")
		{}


		/// Empty Vector with a name 
		inline Vector(const word& name)
		:
			name_(name)
		{}

		
		/// Vector with specified length and name
		inline Vector(const word& name, size_t len)
		:
			vectorType(len),
			name_(name)
		{}

		/// Vector with name, length and value
		inline Vector(const word& name, size_t len, const T& val)
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
		inline Vector(const word& name, const initList &l)
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
		inline Vector(const word name, const Vector<T>& src):
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
		inline Vector( VectorType && mv) = default;
	
		/// Move assignment 
		inline VectorType& operator=( VectorType && mv) = default;
	
		/// Scalar assignment 
		inline void operator=(const T& val)
		{
			fill(val);
		}

		/// Destructor 
		inline ~Vector()
		{
			vectorType::clear();
		}

		/// Clone as a uniquePtr
		inline uniquePtr<VectorType> clone() const
		{
			return makeUnique<VectorType>(*this);
		}

		/// Clone as a pointer 
		inline VectorType* clonePtr()const
		{
			return new VectorType(*this);
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

	// - fill the whole content of vector, [begin, end), with val 
	inline void fill( const T& val);

	static constexpr bool isHostAccessible()
	{
		return isHostAccessible_;
	}

	inline void operator +=( const T& val);
	inline void operator -=( const T& val);
	inline void operator *=( const T& val);
	inline void operator /=( const T& val);

	inline void operator +=( const VectorType& v );
	inline void operator -=( const VectorType& v );
	inline void operator /=( const VectorType& v );
	inline void operator *=( const VectorType& v );

	inline VectorType operator -()const;

	/// Read vector (assume ASCII in input)
	bool readVector(iIstream& is, IOPattern::IOType iotype);

	/// write vector 
	bool writeVector(iOstream& os, IOPattern::IOType iotype) const;

	bool read(iIstream& is, IOPattern::IOType iotype)
	{
		return readVector(is, iotype);
	}

	bool write(iOstream& os, IOPattern::IOType iotype)const
	{
		return writeVector(os, iotype);
	}

};


template<typename T, typename Allocator>
inline iIstream& operator >> (iIstream & is, Vector<T, Allocator> & ivec )
{
	if( !ivec.readVector(is, IOPattern::MasterProcessor) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

template<typename T, typename Allocator> 
inline iOstream& operator << (iOstream& os, const Vector<T, Allocator>& ovec )
{	
	if( !ovec.writeVector(os, IOPattern::AllProcessorsDifferent) )
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
