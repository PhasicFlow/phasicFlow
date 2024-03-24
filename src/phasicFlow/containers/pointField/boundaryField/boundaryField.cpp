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
pFlow::boundaryField<T, MemorySpace>::boundaryField
(
	const boundaryBase& boundary, 
	InternalFieldType& internal
)
:
	observer(&boundary, defaultMessage_),
	boundary_(boundary),
	internal_(internal)
{}


template<class T, class MemorySpace>
pFlow::uniquePtr<pFlow::boundaryField<T, MemorySpace>>
	pFlow::boundaryField<T, MemorySpace>::create
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
			fatalError << "Ctor Selector "<< bType << "for type "<< 
			Yellow_Text(getTypeName<T>())	<< " dose not exist.\n"
			<<"Avaiable ones are: \n\n"
			,
			boundaryBasevCtorSelector_
		);
		fatalExit;

	}
	return nullptr;
}