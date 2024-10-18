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

#ifndef __Field_hpp__
#define __Field_hpp__


#include "types.hpp"
#include "VectorSingle.hpp"
#include "wordVectorHost.hpp"
#include "Vector.hpp"
#include "streams.hpp"

namespace pFlow
{


template<class T, class MemorySpace = void>
class Field
:
	public VectorSingle<T, MemorySpace>
{
public:

	using VectorType  		= VectorSingle<T,MemorySpace>;

	using FieldType         = Field<T, MemorySpace>;

	using FieldTypeHost 	= Field<T, HostSpace>;

	using memory_space 		= typename VectorType::memory_space;

	using execution_space 	= typename VectorType::execution_space;

	using iterator        	= typename VectorType::iterator;

	using const_iterator   	= typename VectorType::const_iterator;

	using reference       	= typename VectorType::reference;
  	
	using const_reference  	= typename VectorType::const_reference;

	using value_type       	= typename VectorType::value_type;
  	
	using pointer         	= typename VectorType::pointer;
  	
	using const_pointer    	= typename VectorType::const_pointer;
  	

private:
	
	static const inline word FKey = "value";

	const word fieldKey_ = FKey;

public:

	/// type info
	TypeInfoTemplateNV111("Field", T, VectorType::memoerySpaceName());

	//// - Constructors

		/// construct an empty Filed with default fieldKey
		Field()
		:
			VectorType()
		{}

		Field(const word& name)
		:
			VectorType(name)
		{}

		/// Construct an empty field with name and fieldKey 
		Field(const word& name, const word& fieldKey)
		:
			VectorType(name),
			fieldKey_(fieldKey)
		{}
		
		/// Construct a field with name and fieldKey and specified len
		Field(const word& name, const word& fieldKey, size_t len)
		:
			VectorType(name, len),
			fieldKey_(fieldKey)
		{}

		
		/// Construct a field with name, fieldKey and 
		/// set length to len and value to val
		Field(const word& name, const word& fieldKey, size_t len, const T& val)
		:
			VectorType(name, len, val),
			fieldKey_(fieldKey)
		{}

		Field(const word& name, const word& fieldKey, size_t capacity, size_t len, const T& val)
		:
			VectorType(name, len, len, RESERVE()),
			fieldKey_(fieldKey)
		{
			VectorType::fill(val);
		}

		/// Construct a field with name, fieldKey, capacity and len
		Field(const word& name, const word& fieldKey, size_t capacity, size_t len, RESERVE)
		:
			VectorType(name, capacity, len, RESERVE()),
			fieldKey_(fieldKey)
		{}

		/// Construct a field with fieldKey and Vector vec
		Field(const word& fieldKey, const Vector<T>& vec)
		:
			VectorType(vec.name(), vec.vectorField()),
			fieldKey_(fieldKey)
		{}

		/// Construct a field with name, fieldKey and Vector vec
		Field(const word& name, const word& fieldKey, const Vector<T>& vec)
		:
			VectorType(name, vec.vectorField()),
			fieldKey_(fieldKey)
		{}

		/// Construct a field with name and fieldKey and std::vector 
		Field(const word& name, const word& fieldKey, const std::vector<T>& vec)
		:
			VectorType(name, vec),
			fieldKey_(fieldKey)
		{}

		/// Copy construct with new name and fieldkey
		Field(const word& name, const word& fieldKey, const FieldType& src):
			VectorType(name, src),
			fieldKey_(fieldKey)
		{}

		/// Default copy constructor
		Field(const FieldType&) = default;

		/// Copy assignment, name and fieldKey
		/// on the left hand side are preserved
		FieldType& operator = (const FieldType& rhs)
		{
			if(&rhs == this) return *this;
			VectorType::operator=(rhs);
			return *this;
		}

		/// Move constructor 
		Field(FieldType&&) = default;

		/// Move assignment 
		FieldType& operator = (FieldType&&) = default;

	//// - Methods

		/// return field key
		word fieldKey()const
		{
			return fieldKey_;
		}

		word name()const
		{
			return VectorType::name();
		}


		void fillField(rangeU32 span, const T& val)
		{
			this->fill(span, val);
		}

		void fillField(const T& val)
		{
			this->fill(val);
		}
		
	//// - IO operations 
		
		bool read(iIstream& is);
		
		bool write(iOstream& os)const;			


		bool read(iIstream& is, const IOPattern& iop, bool resume = false);
		

		bool write(iOstream& os, const IOPattern& iop )const;			

};


template<class T, class MemorySpace>
inline iIstream& operator >> (iIstream & is, Field<T, MemorySpace> & ifld )
{
	if( !ifld.read(is, IOPattern::MasterProcessorOnly) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

template<typename T, typename MemorySpace>
inline iOstream& operator << (iOstream& os, const Field<T, MemorySpace>& ofld )
{
	
	if( !ofld.write(os, IOPattern::AllProcessorsDifferent) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}

	return os; 
}


}

#include "Field.cpp"


#endif  //__Field_hpp__
