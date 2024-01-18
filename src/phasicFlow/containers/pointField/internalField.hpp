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
#ifndef __internalField_hpp__
#define __internalField_hpp__

#include "internalPoints.hpp"
#include "Field.hpp"
#include "observer.hpp"


namespace pFlow
{

template<template<class, class> class VectorField, class T, class MemorySpace=void>
class internalField
:
    public observer
{

public:

	using FieldType         = Field<VectorField, T, MemorySpace>;
  	
	using VectorType  		= typename FieldType::VectorType;

	using memory_space 		= typename FieldType::memory_space;

	using execution_space 	= typename FieldType::execution_space;

protected:

    FieldType           	field_;

    const internalPoints&   internalPoints_;

public:

	internalField(
		const word& name, 
		const internalPoints& internal);
	

	auto fieldDevice()const
	{
		return field_.deviceVector();
	}

	auto fieldHost()const
	{
		return field_.hostVector();
	}

	hostViewType1D<T> activeValuesHost()const;

	inline
	auto size()const
	{
		return field_.size();
	}

	inline
	auto capacity()const
	{
		return field_.capacity();
	}

	inline 
	word name()const
	{
		return field_.name();
	}

	inline 
	word fieldKey()const
	{
		return field_.fieldKey();
	}

	inline
	auto activeRange()const
	{
		return internalPoints_.activeRange();
	}

	inline
	auto isAllActive()const
	{
		return internalPoints_.isAllActive();
	}

	//// - IO

	bool write(iOstream& os, const IOPattern& iop)const;

};

/*template<template<class, class> class VectorField, class T, class MemorySpace>
inline
iOstream& operator<<
(
	iOstream& os, 
	const internalField<VectorField, T, MemorySpace>& if
)
{
	if( !if.write(os, IOPattern::AllProcessorsDifferent) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}

	return os;
}*/

} // pFlow

#include "internalField.cpp"

#endif // __internalField_hpp__