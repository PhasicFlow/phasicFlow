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

#ifndef __internalField_hpp__
#define __internalField_hpp__

#include "pointStructure.hpp"
#include "Field.hpp"
#include "observer.hpp"


namespace pFlow
{

template<template<class, class> class VectorField, class T, class MemorySpace=void>
class internalField
:
	public observer,
	public Field<VectorField, T, MemorySpace>
{
public:
	
	using pointFieldType 	= pointField<VectorField, T, MemorySpace>;
	
	using FieldType         = Field<VectorField, T, MemorySpace>;

    using boundaryFieldListType = boundaryFieldList<VectorField, T, MemorySpace>;
  	
  	using VectorType  		= typename FieldType::VectorType;

	using iterator        	= typename FieldType::iterator;

  	using const_iterator   	= typename FieldType::const_iterator;

	using reference       	= typename FieldType::reference;
  	
  	using const_reference  	= typename FieldType::const_reference;

	using value_type       	= typename FieldType::value_type;
  	
  	using pointer         	= typename FieldType::pointer;
  	
  	using const_pointer    	= typename FieldType::const_pointer;

protected:

    //// - data members 

        /// @brief list of boundaries
        boundaryFieldListType       boundaryFieldList_;
        
		/// @brief  refrence to point structure 
		const pointStructure&       pStruct_;

		/// @brief value when a new item is added to field
		T 	defaultValue_;


public:
 
	// - type info
	TypeInfoTemplateNV2("pointField", T, VectorType::memoerySpaceName());

	
	//// - Constructors

		// - construct a field from pointStructure and set defaultValue_ and field value to defVal
		pointField(
			const objectFile& objf, 
            pointStructure& pStruct, 
            const T& defVal);

		// - construct from iIOEntity, pointStructure and a value
		/*pointField( const pointStructure& pStruct, const T& val, const T& defVal, bool subscribe = true);

		// - construct from another pointField
		//   subscribe to events if true
		pointField( const pointField& src, bool subscribe);


		// - copy construct 
		pointField(const pointField& src);

		// - no move construct
		pointField(pointField&& src) = delete;


		// assignment, only assign the VectorField and preserve other parts of this 
		pointField& operator = (const pointField& rhs);

		// no move assignment 
		pointField& operator = (pointField&&) = delete;		


		inline uniquePtr<pointFieldType> clone() const
		{
			return makeUnique<pointFieldType>(*this);
		}

		inline pointFieldType* clonePtr()const
		{
			return new pointFieldType(*this);
		}

	//// - Methods

		// - reference to pointStructure
		inline const pointStructure& pStruct()const {
			return pStruct_;
		}

		// if all points are active 
		INLINE_FUNCTION_H
		bool allActive()const {	
			return pStruct_.allActive();
		}
		

		INLINE_FUNCTION_H
		bool isActive(label i)const {
			return pStruct_.isActive(i);
		}

		const auto& pointFlag()const
		{
			return pStruct_.pointFlag();
		}

		range activeRange()const
		{
			return pStruct_.activeRange();
		}

		// - update the field if any changes occure in pStruct 
		//   for now it checks for deleted points
		bool update(const eventMessage& msg);


	//// -  IO operations
		bool readPointField(iIstream& is);

		bool writePointField(iOstream& os)const;


		bool read(iIstream& is)
		{
			return readPointField(is);
		}

		bool write(iOstream& os)const
		{
			return writePointField(os);
		}*/
};

/*template<template<class, class> class VectorField, class T, class MemorySpace>
iIstream& operator >> (iIstream & is, pointField<VectorField, T, MemorySpace> & pF )
{
	if( !pF.read(is))
	{
		ioErrorInFile( is.name(), is.lineNumber() ) <<
		"error in reading pointField from file. \n";
		fatalExit;
	}

	return is;
}

template<template<class, class> class VectorField, class T, class MemorySpace>
iOstream& operator << (iOstream& os, const pointField<VectorField, T, MemorySpace>& pF )
{
	if(! pF.write(os) )
	{
		ioErrorInFile( os.name(), os.lineNumber() )<<
		"error in writing pointField into file. \n";
		fatalExit;
	}

	return os;
}*/

}

#include "pointField.cpp"
//#include "pointFieldAlgorithms.hpp"

#endif //  __pointField_hpp__
