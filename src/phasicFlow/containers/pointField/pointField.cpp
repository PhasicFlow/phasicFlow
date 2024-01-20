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
pFlow::pointField<VectorField, T, MemorySpace>::pointField
(
    const objectFile& objf, 
	pointStructure& pStruct, 
	const T& defVal
)
:
	IOobject
	(
		objf,
		pStruct.ioPattern(),
		pStruct.owner()
	),
	InternalFieldType
	(
		objf.name(),
		pStruct
	),
	boundaryFieldList_(pStruct.boundaries(), *this),
	pStruct_(pStruct),
	defaultValue_(defVal)
{
	
}

template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::pointField<VectorField, T, MemorySpace>::pointField
(
	const objectFile &objf, 
	pointStructure &pStruct, 
	const T& defVal,
	const T& val
)
:
	IOobject
	(
		objectFile
		(
			objf.name(),
			objf.localPath(),
			objectFile::READ_NEVER,
			objf.wFlag()
		),
		pStruct.ioPattern(),
		pStruct.owner()
	),
	InternalFieldType
	(
		objf.name(),
		pStruct,
		val
	),
	boundaryFieldList_(pStruct.boundaries(), *this),
	pStruct_(pStruct),
	defaultValue_(defVal)
{}


 /*



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

     }
     else if( msg.isInsert())
     {
         const auto newElems = pStruct().insertedPointIndex();
         //Vector<T> vals( newElems.size(), defaultValue_);
         return this->insertSetElement(newElems, defaultValue_);
     }

     if(msg.isRearranged())
     {
         auto sortedIndex = pStruct().mortonSortedIndex();
         this->sortItems(sortedIndex);
         return true;
     }

     return true;
 }*/

template <template <class, class> class VectorField, class T, class MemorySpace>
bool pFlow::pointField<VectorField, T, MemorySpace>::readPointField(
    iIstream &is,
    const IOPattern &iop)
{
	Field<Vector, T , vecAllocator<T>> 
        fRead("file_read"+InternalFieldType::name(), InternalFieldType::fieldKey());   

	if( !fRead.read(is, iop))
	{
		fatalErrorInFunction<<
		"Error in reading pointPosition from stream "<< is.name()<<endl;
		return false;
	}

	auto thisN = pStruct_.simDomain().initialNumberInThis();

	Field<Vector, T , vecAllocator<T>> internal
    (
        "internalField"+InternalFieldType::name(), 
        InternalFieldType::fieldKey(), 
        thisN, 
        thisN, 
        RESERVE()
    );

	auto pSpan  = fRead.getSpan(); 
    auto iSpan  = internal.getSpan();
    
    if(!pStruct_.simDomain().initialTransferBlockData(pSpan, iSpan))
    {
        fatalErrorInFunction<<
        "Error in transfering the block data for field "<<
		InternalFieldType::name()<<endl;
        return false;
    }

	this->field_.assign(internal);

	return true;
}

template<template<class, class> class VectorField, class T, class MemorySpace>
bool pFlow::pointField<VectorField, T, MemorySpace>::writePointField
(
	iOstream& os,
	const IOPattern& iop
)const
{
	
    hostViewType1D<T> valuesH;

    if(this->isAllActive())
    {
        valuesH = this->fieldHost();
    }
    else
    {
        valuesH = this->activeValuesHost();
    }

    auto data = span<T>(valuesH.data(), valuesH.size());
    
    if( !writeSpan(os, data, iop) )
    {
        fatalErrorInFunction<<
        "Error in writing pointStructure in stream "<<
        os.name()<<endl;
        return false;
    }

    return true;
	
}