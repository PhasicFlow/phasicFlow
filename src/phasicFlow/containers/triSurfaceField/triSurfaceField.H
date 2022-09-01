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

#ifndef __trieSurfaceField_H__ 
#define __trieSurfaceField_H__

#include "Field.H"
#include "triSurface.H"
#include "error.H"


namespace pFlow
{


template<template<class, class> class VectorField, class T, class MemorySpace=void>
class triSurfaceField
:
	public eventObserver,	
	public Field<VectorField, T, MemorySpace>
{
public:
	
	using triSurfaceFieldType 	= triSurfaceField<VectorField, T, MemorySpace>;
	
	using FieldType         	= Field<VectorField, T, MemorySpace>;
  	
  	using VectorType  			= typename FieldType::VectorType;

	using iterator        = typename FieldType::iterator;

  	using constIterator   = typename FieldType::constIterator;

	using reference       = typename FieldType::reference;
  	
  	using constReference  = typename FieldType::constReference;

	using valueType       = typename FieldType::valueType;
  	
  	using pointer         = typename FieldType::pointer;
  	
  	using constPointer    = typename FieldType::constPointer;

protected:

	////- data members
		const triSurface& surface_;

		// - value when a new item is added to field
		T 	defaultValue_;


public:
	
	// - type info
	TypeNameTemplateNV2("triSurfaceField", T, VectorType::memoerySpaceName());

	
	//// CONSTRUCTORS

		// - construct a field from tirSurface and set defaultValue_ and field value to defVal
		triSurfaceField( const triSurface& surface, const T& defVal, bool subscribe = true);

		// - construct from iIOEntity, tirSurface and a value
		triSurfaceField( const triSurface& surface, const T& val, const T& defVal, bool subscribe = true);

		// - construct from another triSurfaceField
		//   subscribe to events if true
		triSurfaceField( const triSurfaceField& src, bool subscribe);


		// - copy construct 
		triSurfaceField(const triSurfaceField& src);

		// - no move construct
		triSurfaceField(triSurfaceField&& src) = delete;


		// assignment, only assign the VectorField and preserve other parts of this 
		triSurfaceField& operator = (const triSurfaceField& rhs);

		// no move assignment 
		triSurfaceField& operator = (triSurfaceField&&) = delete;		


		inline uniquePtr<triSurfaceFieldType> clone() const
		{
			return makeUnique<triSurfaceFieldType>(*this);
		}

		inline triSurfaceFieldType* clonePtr()const
		{
			return new triSurfaceFieldType(*this);
		}

	//// - Methods

		inline const triSurface& surface()const {
			return surface_;
		}

		auto getTriangleAccessor()const
		{
			return surface_.getTriangleAccessor();
		}

		bool update(const eventMessage& msg);


	//// -  IO operations
		bool readTriSurfacceField(iIstream& is);

		bool writeTriSurfaceField(iOstream& os)const;


		bool read(iIstream& is)
		{
			return readTriSurfacceField(is);
		}

		bool write(iOstream& os)const
		{
			return writeTriSurfaceField(os);
		}

};

template<template<class, class> class VectorField, class T, class MemorySpace>
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
}

}

#include "triSurfaceField.C"

#endif //__trieSurfaceField_H__
