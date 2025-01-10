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
	const pointStructure& pStruct,
	InternalFieldType& internal
)
:
	generalBoundary
	(
		boundary, 
		pStruct,
		getTypeName<T>(), 
		memory_space::name()
	),
	internal_(internal)
{}

template<class T, class MemorySpace>
typename pFlow::boundaryField<T, MemorySpace>::ProcVectorType& 
pFlow::boundaryField<T, MemorySpace>::neighborProcField()
{
	static ProcVectorType dummyVector{"dummyVector"};
	//notImplementedFunction;
	//fatalExit;
	return dummyVector;
}

template<class T, class MemorySpace>
const typename pFlow::boundaryField<T, MemorySpace>::ProcVectorType& 
pFlow::boundaryField<T, MemorySpace>::neighborProcField() const
{
	static ProcVectorType dummyVector{"dummyVector"};
	//notImplementedFunction;
	//fatalExit;
	return dummyVector;
}

template<class T, class MemorySpace>
pFlow::uniquePtr<pFlow::boundaryField<T, MemorySpace>>
	pFlow::boundaryField<T, MemorySpace>::create
(
	const boundaryBase& boundary, 
	const pointStructure& pStruct,
	InternalFieldType& internal
)
{

	word bType = angleBracketsNames3(
		"boundaryField", 
		boundary.type(),
		getTypeName<T>(),
		memory_space::name());

	word bTypeAlter = angleBracketsNames3(
		"boundaryField", 
		"none",
		getTypeName<T>(),
		memory_space::name());

	if(boundaryBasevCtorSelector_.search(bType))
	{
		return boundaryBasevCtorSelector_[bType](boundary, pStruct, internal);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< bType << "for type "<< 
			Yellow_Text(getTypeName<T>())	<< " does not exist.\n"
			<<"Avaiable ones are: \n\n"
			,
			boundaryBasevCtorSelector_
		);
		fatalExit;

	}
	return nullptr;
}