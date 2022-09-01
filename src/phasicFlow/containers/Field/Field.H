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

#ifndef __Field_H__
#define __Field_H__

#include "VectorSingle.H"
#include "vocabs.H"

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

  	using constIterator   = typename VectorType::constIterator;

	using reference       = typename VectorType::reference;
  	
  	using constReference  = typename VectorType::constReference;

	using valueType       = typename VectorType::valueType;
  	
  	using pointer         = typename VectorType::pointer;
  	
  	using constPointer    = typename VectorType::constPointer;
  	

protected:
	
	static const inline word FKey = "value";

	const word fieldKey_ = FKey;

	bool readUniform( iIstream& is, size_t len, bool readLength = true);

	bool readNonUniform( iIstream& is, size_t len);

public:

	// - type info
	TypeNameTemplateNV2("Field", T, VectorType::memoerySpaceName());

	//// - Constructors

		// construct an empty Filed with default fieldKey
		Field()
		:
			VectorType()
		{}

		// construct an empty Field with fieldKey
		Field(const word& fieldKey)
		:
			VectorType(),
			fieldKey_(fieldKey)
		{}

		// construct an empty field with name and fieldKey 
		Field(const word& name, const word& fieldKey)
		:
			VectorType(name),
			fieldKey_(fieldKey)
		{}

		// construct an empty Filed with default fieldKey
		Field(size_t len)
		:
			VectorType(len)
		{}

		// construct an empty Field with fieldKey
		Field(const word& fieldKey, size_t len)
		:
			VectorType(len),
			fieldKey_(fieldKey)
		{}

		// construct an empty field with name and fieldKey 
		Field(const word& name, const word& fieldKey, size_t len)
		:
			VectorType(name, len),
			fieldKey_(fieldKey)
		{}

		// construct an empty Filed with default fieldKey and set vector to val
		Field(size_t len, const T& val)
		:
			VectorType(len, val)
		{}

		// construct an empty Field with fieldKey and set vector to val
		Field(const word& fieldKey, size_t len, const T& val)
		:
			VectorType(len, val),
			fieldKey_(fieldKey)
		{}

		// construct an empty field with name and fieldKey and set vector to val
		Field(const word& name, const word& fieldKey, size_t len, const T& val)
		:
			VectorType(name, len, val),
			fieldKey_(fieldKey)
		{}

		// construct a field with capacity and len and default fieldKey
		Field(size_t capacity, size_t len, RESERVE)
		:
			VectorType(capacity, len, RESERVE())
		{}

		// construct an empty Field with fieldKey
		Field(const word& fieldKey, size_t capacity, size_t len, RESERVE)
		:
			VectorType(capacity, len, RESERVE()),
			fieldKey_(fieldKey)
		{}

		// construct an empty field with name and fieldKey 
		Field(const word& name, const word& fieldKey, size_t capacity, size_t len, RESERVE)
		:
			VectorType(name, capacity, len, RESERVE()),
			fieldKey_(fieldKey)
		{}

		// construct with vec and default fieldKey
		Field(const Vector<T>& vec)
		:
			VectorType(vec)
		{}

		// construct an empty Field with fieldKey
		Field(const word& fieldKey, const Vector<T>& vec)
		:
			VectorType(vec),
			fieldKey_(fieldKey)
		{}

		// construct an empty field with name and fieldKey 
		Field(const word& name, const word& fieldKey, const Vector<T>& vec)
		:
			VectorType(name, vec),
			fieldKey_(fieldKey)
		{}


		// - copy construct with new name and fieldkey
		Field(const word& name, const word& fieldKey, const FieldType& src):
			VectorType(name, src),
			fieldKey_(fieldKey)
		{}

		// - default copy constructor
		Field(const FieldType&) = default;

		// - default copy assignment 
		FieldType& operator = (const FieldType&) = default;

		// - no move constructor 
		Field(FieldType&&) = delete;

		// - no move assignment 
		FieldType& operator = (FieldType&&) = delete;

		// - clone as a uniquePtr
		INLINE_FUNCTION_H
		uniquePtr<FieldType> clone() const
		{
			return makeUnique<FieldType>(*this);
		}

		// - clone as a raw pointer 
		INLINE_FUNCTION_H
		FieldType* clonePtr()const
		{
			return new FieldType(*this);
		}

	//// - Methods

		const word& fieldKey()const
		{
			return fieldKey_;
		}
	
	//// - IO operations 
		bool readField(iIstream& is, const size_t len, bool readLength = true);
		
		
		bool readField(iIstream& is );

		
		bool writeField(iOstream& os)const;

		
		bool read(iIstream& is)
		{
			return readField(is);
		}

		bool write(iOstream& os)const
		{
			return writeField(os);
		}
	

};


template<template<class, class> class VectorField, class T, class PropType>
inline iIstream& operator >> (iIstream & is, Field<VectorField, T, PropType> & ifld )
{
	if( !ifld.readField(is) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

template<template<class, class> class VectorField, class T, class PropType>
inline iOstream& operator << (iOstream& os, const Field<VectorField, T, PropType>& ofld )
{
	
	if( !ofld.writeField(os) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}

	return os; 
}


}

#include "Field.C"


#endif  //__Field_H__
