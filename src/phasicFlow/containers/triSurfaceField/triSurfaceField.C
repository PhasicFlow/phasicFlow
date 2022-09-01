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
}