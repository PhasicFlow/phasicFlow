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
pFlow::boundaryField<VectorField, T, MemorySpace>::boundaryField
(
	const boundaryBase& boundary, 
	InternalFieldType& internal
)
:
	observer(),
	boundary_(boundary),
	internal_(internal)
{}


template<template<class, class> class VectorField, class T, class MemorySpace>
pFlow::uniquePtr<pFlow::boundaryField<VectorField, T, MemorySpace>>
	pFlow::boundaryField<VectorField, T, MemorySpace>::create
(
	const boundaryBase& boundary, 
	InternalFieldType& internal
)
{

	word bType = angleBracketsNames("boundaryField", boundary.type());

	if(boundaryBasevCtorSelector_.search(bType))
	{
		return boundaryBasevCtorSelector_[bType](boundary, internal);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< bType << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			boundaryBasevCtorSelector_
		);
		fatalExit;

	}
	return nullptr;
}