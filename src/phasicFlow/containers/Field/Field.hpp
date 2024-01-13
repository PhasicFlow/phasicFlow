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

//#include "VectorSingle.hpp"
//#include "vocabs.hpp"


#include "types.hpp"
#include "Vector.hpp"
#include "streams.hpp"

namespace pFlow
{



template<template<class, class> class VectorField, class T, class PropType=void>
class Field
:
	public VectorField<T, PropType>
{
public:
	

	using VectorType  		= VectorField<T,PropType>;

	using FieldType         = Field<VectorField, T, PropType>;

	using iterator        = typename VectorType::iterator;

	using const_iterator   = typename VectorType::const_iterator;

	using reference       = typename VectorType::reference;
  	
	using const_reference  = typename VectorType::const_reference;

	using value_type       = typename VectorType::value_type;
  	
	using pointer         = typename VectorType::pointer;
  	
	using const_pointer    = typename VectorType::const_pointer;
  	

protected:
	
	static const inline word FKey = "value";

	const word fieldKey_ = FKey;

	/*bool readUniform( iIstream& is, size_t len, bool readLength = true);

	bool readNonUniform( iIstream& is, size_t len);*/

public:

	/// type info
	TypeInfoTemplateNV2("Field", T, VectorType::memoerySpaceName());

	//// - Constructors

		/// construct an empty Filed with default fieldKey
		Field()
		:
			VectorType()
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

		/// clone as a uniquePtr
		INLINE_FUNCTION_H
		uniquePtr<FieldType> clone() const
		{
			return makeUnique<FieldType>(*this);
		}

		/// clone as a raw pointer 
		INLINE_FUNCTION_H
		FieldType* clonePtr()const
		{
			return new FieldType(*this);
		}

	//// - Methods

		/// return field key
		const word& fieldKey()const
		{
			return fieldKey_;
		}
	
	//// - IO operations 
		/*bool readField(iIstream& is, const size_t len, bool resume, bool readLength = true);
		
		
		bool readField(iIstream& is, bool resume );

		
		bool writeField(iOstream& os)const;*/

		
		bool read(iIstream& is, const IOPattern& iop);
		

		bool write(iOstream& os, const IOPattern& iop )const;

		template<typename HostMask>
		bool write(
			iOstream& os, 
			const IOPattern& iop, 
			const HostMask& mask)const
		{
			
			os.writeWordKeyword(fieldKey_)<<endl;

			if(!os.check(FUNCTION_NAME))return false;

			if(!VectorType::write(os, iop)) return false;

			os.endEntry();
			if(!os.check(FUNCTION_NAME))return false;

			return true;
		}
		
			

};


template<template<class, class> class VectorField, class T, class PropType>
inline iIstream& operator >> (iIstream & is, Field<VectorField, T, PropType> & ifld )
{
	if( !ifld.read(is, IOPattern::MasterProcessorOnly) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

template<template<class, class> class VectorField, class T, class PropType>
inline iOstream& operator << (iOstream& os, const Field<VectorField, T, PropType>& ofld )
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
