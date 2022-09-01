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


template<template<class, class> class VectorField, class T, class MemorySpace>
bool pFlow::pointField<VectorField, T, MemorySpace>::readPointField
(
	iIstream& is
)
{
	
	return FieldType::readField(is, pStruct_.size(), false);
}

template<template<class, class> class VectorField, class T, class MemorySpace>
bool pFlow::pointField<VectorField, T, MemorySpace>::writePointField
(
	iOstream& os
)const
{
	return FieldType::write(os);
}



template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::pointField<VectorField, T, MemorySpace>::pointField
(
	const pointStructure& pStruct,
	const T& defVal,
	bool subscribe
)
:
	eventObserver(pStruct, subscribe),
	FieldType(pStruct.capacity(), pStruct.size(), RESERVE()),
	pStruct_(pStruct),
	defaultValue_(defVal)
{
	// set value 
	this->fill(defVal);
}

template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::pointField<VectorField, T, MemorySpace>::pointField
(
	const pointStructure& pStruct,
	const T& val,
	const T& defVal,
	bool subscribe
)
:
	eventObserver(pStruct, subscribe),
	FieldType(pStruct.capacity(), pStruct.size(), RESERVE()),
	pStruct_(pStruct),
	defaultValue_(defVal)
{
	this->fill(val);
}

template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::pointField<VectorField, T, MemorySpace>::pointField
(
	const pointField& src,
	bool subscribe
)
:
	eventObserver(src.pStruct(), subscribe),
	FieldType(src),
	pStruct_(src.pStruct()),
	defaultValue_(src.defaultValue_)
{}

template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::pointField<VectorField, T, MemorySpace>::pointField(const pointField& src)
:
	pointField<VectorField, T, MemorySpace>(src, src.subscribed())
{}


template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::pointField<VectorField, T, MemorySpace>& pFlow::pointField<VectorField, T, MemorySpace>::operator = 
(
	const pointField& rhs
)
{
	if(this == &rhs) return *this;
	
	this->VectorField() = rhs.VectorField();
	return *this;
}


template<template<class, class> class VectorField, class T, class MemorySpace>
bool pFlow::pointField<VectorField, T, MemorySpace>::update(const eventMessage& msg)
{
 
	if( msg.isDeleted() )
	{
		/*const auto& dp = pStruct_.markedDeletePoints();
		return this->deleteElement(dp);
		notImplementedFunction;*/
	}
	else if( msg.isInsert())
	{
		const auto newElems = pStruct().insertedPointIndex();
		//Vector<T> vals( newElems.size(), defaultValue_);
		return this->insertSetElement(newElems, defaultValue_);
	}
	
	return true;
}

