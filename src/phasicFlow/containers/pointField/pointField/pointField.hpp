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

#ifndef __pointField_hpp__
#define __pointField_hpp__


#include "pointStructure.hpp"
#include "internalField.hpp"
#include "boundaryFieldList.hpp"
#include "Time.hpp"

namespace pFlow
{

template<class T, class MemorySpace=void>
class pointField
:
	public IOobject,
	public internalField<T, MemorySpace>
{
public:
	
	using PointFieldType 	= pointField<T, MemorySpace>;

	using InternalFieldType = internalField<T, MemorySpace>;

	using boundaryFieldListType = boundaryFieldList<T, MemorySpace>;

	using FieldType 		= typename InternalFieldType::FieldType;

	using VectorType  		= typename InternalFieldType::VectorType;

	using memory_space 		= typename InternalFieldType::memory_space;

	using execution_space 	= typename InternalFieldType::execution_space;

private:

    //// - data members 

        /// @brief list of boundaries
        boundaryFieldListType       boundaryFieldList_;
        
		/// @brief  refrence to point structure 
		const pointStructure&       pStruct_;


public:
	
	
	// - type info
	TypeInfoTemplate111("pointField", T, VectorType::memoerySpaceName());

	
	//// - Constructors

		// - construct a field from pointStructure and set defaultValue_ and field value to defVal
		pointField(
			const objectFile& objf, 
            pointStructure& pStruct, 
            const T& defVal);
		
		pointField(
			const objectFile& objf,
			repository* owner,
			pointStructure& pStruct,
			const T& defVal);
		
		pointField(
			const objectFile& objf,
			pointStructure& pStruct,
			const T& defVal,
			const T& val);
		
		

	//// - Methods

		const word& name() const final
		{
			return  IOobject::name();
		}
	
		const auto& internal()const
		{
			return static_cast<const InternalFieldType&>(*this);
		}

		void updateBoundaries(DataDirection direction)const
		{
			uint32 iter =  this->time().currentIter();
			auto& bList = const_cast<boundaryFieldListType&>(boundaryFieldList_);
			bList.updateBoundaries(iter, direction);
		}

		/// @brief update boundaries if it is requested previousely (slave to master). 
		/// It only checks for SlaveToMaster direction.
		void updateBoundariesSlaveToMasterIfRequested()
		{
			if(boundaryFieldList_.slaveToMasterUpdateRequested())
			{
				updateBoundaries(DataDirection::SlaveToMaster);
			}
		}

		const auto& boundaryFields()const
		{
			return boundaryFieldList_;
		}

		const auto& BoundaryField(uint32 i)const
		{
			return boundaryFieldList_[i];
		}

		// - reference to pointStructure
		inline const pointStructure& pStruct()const {
			return pStruct_;
		}

		void fill(const T& val)
		{
			InternalFieldType::fillInternal(val);
			boundaryFieldList_.fill(val);
		}

	//// -  IO operations
		bool readPointField(iIstream& is, const IOPattern& iop);

		bool writePointField(iOstream& os, const IOPattern& iop)const;


		bool read(iIstream& is, const IOPattern& iop)override
		{
			return readPointField(is, iop);
		}

		bool write(iOstream& os, const IOPattern& iop)const override
		{
			return writePointField(os, iop);
		}
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
}*/

template<class T, class MemorySpace>
iOstream& operator << (iOstream& os, const pointField<T, MemorySpace>& pF )
{
	if(! pF.write(os, IOPattern::AllProcessorsDifferent) )
	{
		ioErrorInFile( os.name(), os.lineNumber() )<<
		"error in writing pointField into file. \n";
		fatalExit;
	}

	return os;
}

}

#include "pointField.cpp"
//#include "pointFieldAlgorithms.hpp"

#endif //  __pointField_hpp__
