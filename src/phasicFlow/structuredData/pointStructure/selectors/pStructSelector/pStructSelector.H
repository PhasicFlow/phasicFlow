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


#ifndef __pStructSelector_H__
#define __pStructSelector_H__


#include "Vectors.H"
#include "virtualConstructor.H"

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
	TypeName("pStructSelector");


		pStructSelector(const pointStructure& pStruct, const dictionary& UNUSED(dict));

		create_vCtor
		(
			pStructSelector,
			dictionary,
			(const pointStructure& pStruct, const dictionary& dict),
			(pStruct, dict)
		);

		virtual ~pStructSelector() = default;

	//// - Methods

		const pointStructure& pStruct()const;


		virtual const int32Vector& selectedPoinsts()const = 0;

		virtual int32Vector& selectedPoinsts() = 0;


	static
	uniquePtr<pStructSelector> create(const pointStructure& pStruct, const dictionary& dict);

};

} // pFlow


#endif //__pStructSelector_H__
