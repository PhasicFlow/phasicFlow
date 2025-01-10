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


#ifndef __setFieldEntry_hpp__
#define __setFieldEntry_hpp__

#include "twoPartEntry.hpp"
#include "pointFields.hpp"
#include "repository.hpp"
#include "indexContainer.hpp"

namespace pFlow
{

class setFieldEntry
{
protected:
		
	twoPartEntry 	entry_;

public:

	//// Constructors 
		
		setFieldEntry(const dataEntry& entry);

		setFieldEntry(const setFieldEntry&) = default;

		setFieldEntry(setFieldEntry&&) = default;

		setFieldEntry& operator=(const setFieldEntry&) = default;

		setFieldEntry& operator=(setFieldEntry&&) = default;

		~setFieldEntry()=default;

	//// - Methods 
	
		word fieldName()const;

		template <typename Type>
		bool checkForType()const;

		template <typename Type>
		bool checkForTypeAndValue()const;

		bool checkForTypeAndValueAll()const;
		
		template <typename Type>
		uniquePtr<pointField_H<Type>> 
			setPointFieldDefaultValueNew
		(
			pointStructure& pStruct,
			bool verbose = false
		);

		uniquePtr<IOobject>
			 setPointFieldDefaultValueNewAll
		(
			pointStructure& pStruct,
			bool verbose = false
		);

		template <typename Type>
		bool setPointFieldSelected
		(
			repository& owner,
			uint32IndexContainer& selected,
			bool verbose = false
		);

		

		bool setPointFieldSelectedAll
		(
			repository& owner,
			uint32IndexContainer& selected,
			bool verbose = false
		);

};

} // pFlow


#include "setFieldEntryTemplates.cpp"

#endif
