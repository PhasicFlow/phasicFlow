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

template<class T, class MemorySpace>
pFlow::pointField<T, MemorySpace>::pointField
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
    
    if(IOobject::implyRead())
    {
        REPORT(1)<< "Reading field "<< Green_Text(IOobject::name())<<
        " from "<<IOobject::path()<<END_REPORT;
    }
    else
    {
        REPORT(1)<< "Creating field "<< Green_Text(IOobject::name())<<END_REPORT;
    }
    if( !IOobject::readObject() )
    {
        fatalErrorInFunction<<
        "Error in reading from file "<<IOobject::path()<<endl;
        fatalExit;
    }    

}

template<class T, class MemorySpace>
pFlow::pointField<T, MemorySpace>::pointField
(
    const objectFile& objf,
    repository* owner,
    pointStructure& pStruct,
    const T& defVal
)
:
	IOobject
	(
		objf,
		pStruct.ioPattern(),
		owner
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
    if(IOobject::implyRead())
    {
        REPORT(1)<< "Reading field "<< Green_Text(IOobject::name())<<
        " from "<<IOobject::path()<<END_REPORT;
    }
    else
    {
        REPORT(1)<< "Creating field "<< Green_Text(IOobject::name())<<END_REPORT;
    }
    if( !IOobject::readObject() )
    {
        fatalErrorInFunction<<
        "Error in reading from file "<<IOobject::path()<<endl;
        fatalExit;
    }
}

template<class T, class MemorySpace>
pFlow::pointField<T, MemorySpace>::pointField
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
{

}


 /*



 template<class T, class MemorySpace>
 pFlow::pointField<T, MemorySpace>::pointField
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

 template<class T, class MemorySpace>
 pFlow::pointField<T, MemorySpace>::pointField
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

 template<class T, class MemorySpace>
 pFlow::pointField<T, MemorySpace>::pointField
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

 template<class T, class MemorySpace>
 pFlow::pointField<T, MemorySpace>::pointField(const pointField& src)
 :
     pointField<T, MemorySpace>(src, src.subscribed())
 {}


 template<class T, class MemorySpace>
 pFlow::pointField<T, MemorySpace>& pFlow::pointField<T, MemorySpace>::operator =
 (
     const pointField& rhs
 )
 {
     if(this == &rhs) return *this;

     this->VectorField() = rhs.VectorField();
     return *this;
 }


 template<class T, class MemorySpace>
 bool pFlow::pointField<T, MemorySpace>::update(const eventMessage& msg)
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

template <class T, class MemorySpace>
bool pFlow::pointField<T, MemorySpace>::readPointField
(
    iIstream &is,
    const IOPattern &iop
)
{
	typename InternalFieldType::FieldTypeHost fRead
    (
        InternalFieldType::name(),
        InternalFieldType::fieldKey()
    );
    
	if( !fRead.read(is, iop))
	{
		fatalErrorInFunction<<
		"Error in reading pointPosition from stream "<< is.name()<<endl;
		return false;
	}

	auto thisN = pStruct_.simDomain().initialNumberInThis();

	typename InternalFieldType::FieldTypeHost internal
    (
        InternalFieldType::name(),
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

	this->field_.assignFromHost(internal);

	return true;
}

template<class T, class MemorySpace>
bool pFlow::pointField<T, MemorySpace>::writePointField
(
	iOstream& os,
	const IOPattern& iop
)const
{
    return InternalFieldType::write(os, iop);
}