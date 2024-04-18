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

#ifndef __pStructSelector_hpp__
#define __pStructSelector_hpp__


#include "Vectors.hpp"
#include "virtualConstructor.hpp"


namespace pFlow
{


class dictionary;
class pointStructure;

class pStructSelector
{
protected:
	
	const pointStructure& pStruct_;

public:

	// - type info
	TypeInfo("pStructSelector");

		/// the dictionary contains the selector keyword and another dictionary which is 
		/// used for creating selector 
		pStructSelector(const pointStructure& pStruct, const dictionary& UNUSED(dict));

		/// construct using selector type and a dictionary that contains info of selector 
		pStructSelector(
			const word& type, 
			const pointStructure& pStruct, 
			const dictionary& UNUSED(dict));

		create_vCtor
		(
			pStructSelector,
			dictionary,
			(const pointStructure& pStruct, const dictionary& dict),
			(pStruct, dict)
		);

		create_vCtor
		(
			pStructSelector,
			word,
			(
				const word& type, 
				const pointStructure& pStruct, 
				const dictionary& dict
			),
			(type, pStruct, dict)
		);

		virtual ~pStructSelector() = default;

	//// - Methods

		const pointStructure& pStruct()const;


		virtual const uint32Vector& selectedPoints()const = 0;

		virtual uint32Vector& selectedPoints() = 0;

		realx3Vector selectedPointPositions()const;

	static
	uniquePtr<pStructSelector> create(const pointStructure& pStruct, const dictionary& dict);

	static
	uniquePtr<pStructSelector> create(
		const word& type, 
		const pointStructure& pStruct, 
		const dictionary& dict);
};



} // pFlow


#endif //__pStructSelector_hpp__
