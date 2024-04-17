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


#ifndef __selectRandom_hpp__
#define __selectRandom_hpp__

#include "pStructSelector.hpp"
#include "pointStructure.hpp"


namespace pFlow
{

class dictionary;

class selectorRandomPoints
:
	public pStructSelector
{
private:
	
	uint32Vector selectedPoints_;

	// begin index
	uint32 	begin_;

	// end index
	uint32 	end_;

	// stride
	uint32 	number_;

	bool selectAllPointsInRange();
	
public:

	// - type info
	TypeInfo("selector<randomPoints>");


		selectorRandomPoints(const pointStructure& pStruct, const dictionary& dict);

		selectorRandomPoints(
			const word& type, 
			const pointStructure& pStruct, 
			const dictionary& UNUSED(dict));

		add_vCtor
		(
			pStructSelector,
			selectorRandomPoints,
			dictionary
		);

		add_vCtor
		(
			pStructSelector,
			selectorRandomPoints,
			word
		);
		~selectorRandomPoints() final = default;

	//// - Methods

		const uint32Vector& selectedPoints()const final
		{
			return selectedPoints_;
		}

		uint32Vector& selectedPoints() final
		{
			return selectedPoints_;
		}

};

} // pFlow


#endif //__pStructSelector_hpp__
