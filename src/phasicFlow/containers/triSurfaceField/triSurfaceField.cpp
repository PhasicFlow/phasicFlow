#include "triSurfaceField.hpp"
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
pFlow::triSurfaceField<T,MemorySpace>::triSurfaceField
( 
	const objectFile& objf,
	multiTriSurface& surface, 
	const T& defVal
)
:
	triSurfaceField
	(
		objf,
		surface.owner(),
		surface,
		defVal
	)
{

}

template<class T, class MemorySpace>
pFlow::triSurfaceField<T,MemorySpace>::triSurfaceField
( 
	const objectFile& objf,
	repository* owner,
	multiTriSurface& surface, 
	const T& defVal
)
:
	IOobject
	(
		objf,
		IOPattern::AllProcessorsSimilar,
		owner
	),
	observer
	(
		&surface,
		message::Empty()
	),
	field_
	(
		objf.name(),
		"value",
		surface.capacity(),
		surface.size(),
		RESERVE()
	),
	surface_(surface),
	defaultValue_(defVal)
{
	if(IOobject::implyRead())
    {
        REPORT(1)<< "Reading triSurfaceField "<< Green_Text(IOobject::name())<<
        " from "<<IOobject::path()<<END_REPORT;
    }
    else
    {
        REPORT(1)<< "Creating triSurfaceField "<< Green_Text(IOobject::name())<<END_REPORT;
    }
    if( !IOobject::readObject() )
    {
        fatalErrorInFunction<<
        "Error in reading from file "<<IOobject::path()<<endl;
        fatalExit;
    }
} 

template <class T, class MemorySpace>
pFlow::triSurfaceField<T, MemorySpace>::triSurfaceField
(
	const objectFile &objf, 
	multiTriSurface &surface, 
	const T &val, 
	const T &defVal
)
:
	triSurfaceField
	(
		objf,
		surface.owner(),
		surface,
		val, 
		defVal
	)	
{}

template <class T, class MemorySpace>
pFlow::triSurfaceField<T, MemorySpace>::triSurfaceField
(
	const objectFile &objf, 
	repository *owner, 
	multiTriSurface &surface, 
	const T &val, 
	const T &defVal
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
		IOPattern::AllProcessorsSimilar,
		owner
	),
	observer
	(
		&surface,
		message::Empty()
	),
	field_
	(
		objf.name(),
		"value",
		surface.capacity(),
		surface.size(),
		val
	),
	surface_(surface),
	defaultValue_(defVal)
{
}

template <class T, class MemorySpace>
bool pFlow::triSurfaceField<T, MemorySpace>::write
(
	iOstream &is, 
	const IOPattern &iop
) const
{
	return field_.write(is, iop);
}

template <class T, class MemorySpace>
bool pFlow::triSurfaceField<T, MemorySpace>::read
(
	iIstream &is, 
	const IOPattern &iop
)
{
    return field_.read(is, iop);
}

/*template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::triSurfaceField<VectorField, T, MemorySpace>::triSurfaceField
(
	const triSurface& surface,
	const T& defVal,
	bool subscribe
)
:
	eventObserver(surface, subscribe),
	FieldType(surface.capacity(), surface.size(), RESERVE()),
	surface_(surface),
	defaultValue_(defVal)
{
	this->fill(defVal);
}

template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::triSurfaceField<VectorField, T, MemorySpace>::triSurfaceField
(
	const triSurface& surface,
	const T& val,
	const T& defVal,
	bool subscribe
)
:
	eventObserver(surface, subscribe),
	FieldType(surface.capacity(), surface.size(), RESERVE()),
	surface_(surface),
	defaultValue_(defVal)
{
	this->fill(val);
}


template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::triSurfaceField<VectorField, T, MemorySpace>::triSurfaceField
(
	const triSurfaceField& src,
	bool subscribe
)
:
	eventObserver(src.surface(), subscribe),
	FieldType(src),
	surface_(src.surface()),
	defaultValue_(src.defaultValue_)
{

}

template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::triSurfaceField<VectorField, T, MemorySpace>::triSurfaceField
(
	const triSurfaceField& src
)
:
triSurfaceField<VectorField, T, MemorySpace>(src, src.subscribed())
{}

template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::triSurfaceField<VectorField, T, MemorySpace>&
	pFlow::triSurfaceField<VectorField, T, MemorySpace>::operator= 
(
	const triSurfaceField& rhs
)
{
	if(this == &rhs) return *this;
	
	this->VectorField() = rhs.VectorField();
	return *this;
}

template<template<class, class> class VectorField, class T, class MemorySpace>
bool pFlow::triSurfaceField<VectorField, T, MemorySpace>::update
(
	const eventMessage& msg
)
{
	notImplementedFunction;
	return true;
}


template<template<class, class> class VectorField, class T, class MemorySpace>
bool pFlow::triSurfaceField<VectorField, T, MemorySpace>::readTriSurfacceField
(
	iIstream& is
)
{
	return FieldType::readField(is, surface_.size(), false);
}

template<template<class, class> class VectorField, class T, class MemorySpace>
bool pFlow::triSurfaceField<VectorField, T, MemorySpace>::writeTriSurfaceField
(
	iOstream& os
)const
{
	return FieldType::write(os);
}*/