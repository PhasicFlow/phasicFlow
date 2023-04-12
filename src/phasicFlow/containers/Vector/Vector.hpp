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
#include "indexContainer.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"

#ifndef __RESERVE__
#define __RESERVE__
	struct RESERVE{};
#endif

namespace pFlow
{


template<typename T, typename Allocator>
class Vector;

#include "VectorFwd.hpp"


template <class T>
class noConstructAllocator
    : public std::allocator<T>
{
public:
    using std::allocator<T>::allocator;

    template <class U, class... Args> void construct(U*, Args&&...) {}
};

template<typename T>
using vecAllocator = std::allocator<T>;

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

	static inline size_t getVectorStride(const size_t& len)
	{
		size_t stride = 1;
		if( len < 6 ) 		stride = len;
		else if( len <16 )  stride = 3;
		else if( len < 31)  stride = 2;
		else				stride = 1;

		return stride;
	}

	static constexpr bool  isHostAccessible_ = true;

	constexpr static inline const char* memoerySpaceName()
  	{
  		return "std";
  	}
  	

public:

	// - Type info
	TypeInfoTemplateNV2("Vector", T, memoerySpaceName());

	//// - Constructors

		// - empty Vector
		inline Vector()
		:
			Vector("Vector")
		{}

		inline Vector(const word& name)
		:
			name_(name)
		{}
		// - with sepcified length
		inline Vector(const size_t len)
		:
			Vector("Vector",len)
		{}

		// - with specified length and name
		inline Vector(const word& name, size_t len)
		:
			vectorType(len),
			name_(name)
		{
			
		}

		// - with length and value
		inline Vector(size_t len, const T& val)
		:
			Vector("Vector", len, val)
		{}

		inline Vector(const word& name, size_t len, const T& val)
		:
			Vector(name, len)
		{
			this->assign(len, val);
		}

		// - zero length with specified capacity, use Logical
		//   to make it different from previous constructor.
		inline Vector(const size_t cap, RESERVE ):
			Vector("Vector", cap, 0, RESERVE())
		{
		}

		inline Vector(const size_t cap, const size_t len, RESERVE )
		:
			Vector("Vector", cap, len, RESERVE())
		{
			
		}

		Vector(const word& name, size_t cap, size_t len, RESERVE ):
			name_(name)
		{
			this->reserve(cap);
			this->resize(len);
		}

		inline Vector(const size_t cap, const size_t len, const T& val, RESERVE )
		{
			name_ = "Vector";
			reserve(cap);
			this->assign(len, val);
		}


		// from initializer list 
		inline Vector(const initList &l)
		:
			vectorType(l)
		{}

		// - from src and a new name
		inline Vector(const word name, const Vector<T>& src):
			vectorType(src),
			name_(name)
		{}

		// copy construct 
		inline Vector(const VectorType& src) = default;
		
		// move construct 
		inline Vector( VectorType && mv) = default;

		inline Vector(const vectorType& src)
		:
			vectorType(src),
			name_("Vector")
		{
			
		}
		
		// copy assignment
		inline VectorType& operator=( const VectorType& rhs ) = default;

		inline VectorType& operator=(const vectorType& rhs)
		{
			Vector::assign(rhs.begin(), rhs.end());
			return *this;
		}
		
		// move assignment 
		inline VectorType& operator=( VectorType && mv) = default;
	
		// scalar assignment 
		inline void operator=(const T& val)
		{
			fill(val);
		}

		inline ~Vector()
		{
			vectorType::clear();
		}

		inline uniquePtr<VectorType> clone() const
		{
			return makeUnique<VectorType>(*this);
		}

		inline VectorType* clonePtr()const
		{
			return new VectorType(*this);
		}

	inline auto clear()
	{
		return vectorType::clear();
	}

	// access to this, mostly used by derived classes 
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

	const word& name()const
	{
		return name_;
	}

	inline auto size()const
	{
		return vectorType::size();
	}

	inline auto capacity()const
	{
		return vectorType::capacity();
	}

	inline auto reserve(label len)
	{
		return vectorType::reserve(len);
	}

	// - delete elemens of vector based on sorted indices 
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
	inline bool insertSetElement(int32 idx, const T& val);

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

	// from iIstream and specified size 
	//Vector(iIstream & is, size_t len);

	// from iIstream and free size
	Vector(iIstream& is);

	//bool readVector(iIstream & is, size_t len);

	bool readVector(iIstream& is);

	bool writeVector(iOstream& os) const;

	bool read(iIstream& is)
	{
		return readVector(is);
	}

	bool write(iOstream& os)const
	{
		return writeVector(os);
	}

};


template<typename T, typename Allocator>
inline iIstream& operator >> (iIstream & is, Vector<T, Allocator> & ivec )
{
	if( !ivec.readVector(is) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

template<typename T, typename Allocator> 
inline iOstream& operator << (iOstream& os, const Vector<T, Allocator>& ovec )
{
	
	if( !ovec.writeVector(os) )
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
