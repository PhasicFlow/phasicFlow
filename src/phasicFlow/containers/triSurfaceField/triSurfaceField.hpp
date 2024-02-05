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

#ifndef __trieSurfaceField_hpp__ 
#define __trieSurfaceField_hpp__

#include "Field.hpp"
#include "multiTriSurface.hpp"
#include "observer.hpp"
#include "error.hpp"


namespace pFlow
{


template<class T, class MemorySpace=void>
class triSurfaceField
:
	public IOobject,
	public observer
{
public:
	
	using triSurfaceFieldType 	= triSurfaceField<T, MemorySpace>;
	
	using FieldType         	= Field<T, MemorySpace>;
  	
  	using VectorType  			= typename FieldType::VectorType;

	using memory_space 			= typename FieldType::memory_space;

	using execution_space 		= typename FieldType::execution_space;

private:

	////- data members

		FieldType 				field_;

		const multiTriSurface& 	surface_;

		// - value when a new item is added to field
		T 						defaultValue_;


public:
	
	// - type info
	TypeInfoTemplate111("triSurfaceField", T, VectorType::memoerySpaceName());

	
	//// CONSTRUCTORS

		// - construct a field from tirSurface and set defaultValue_ and field value to defVal
		triSurfaceField( 
			const objectFile& objf,
			multiTriSurface& surface, 
			const T& defVal);

		triSurfaceField( 
			const objectFile& objf,
			repository* owner,
			multiTriSurface& surface, 
			const T& defVal);

		// - construct from iIOEntity, tirSurface and a value
		triSurfaceField( 
			const objectFile& objf,
			multiTriSurface& surface, 
			const T& val, 
			const T& defVal);

		triSurfaceField( 
			const objectFile& objf,
			repository* owner,
			multiTriSurface& surface, 
			const T& val, 
			const T& defVal);

		~triSurfaceField()override = default;

			

	//// - Methods

		inline 
		const auto& surface()const 
		{
			return surface_;
		}

		

	//// -  IO operations
		bool write(iOstream& is, const IOPattern& iop)const override;

    
        bool read(iIstream& is, const IOPattern& iop) override;

};

/*template<template<class, class> class VectorField, class T, class MemorySpace>
iIstream& operator >> (iIstream & is, triSurfaceField<VectorField, T, MemorySpace> & tsF )
{
	if( !tsF.read(is))
	{
		ioErrorInFile( is.name(), is.lineNumber() ) <<
		"error in reading triSurfaceField from file. \n"<<
		"field name: "<< tsF.name()<<endl;
		fatalExit;
	}

	return is;
}

template<template<class, class> class VectorField, class T, class MemorySpace>
iOstream& operator << (iOstream& os, const triSurfaceField<VectorField, T, MemorySpace>& tsF )
{
	if(! tsF.write(os) )
	{
		ioErrorInFile( os.name(), os.lineNumber() )<<
		"error in writing triSurfaceField into file. \n"<<
		"field name: "<< tsF.name()<<endl;
		fatalExit;
	}

	return os;
}*/

}

#include "triSurfaceField.cpp"

#endif //__trieSurfaceField_hpp__
