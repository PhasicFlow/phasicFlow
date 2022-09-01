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


#ifndef __boxAll_H__
#define __boxAll_H__

#include "pStructSelector.H"
#include "pointStructure.H"
#include "box.H"


namespace pFlow
{

class dictionary;

class boxAll
:
	public pStructSelector
{
protected:
	
	int32Vector selectedPoints_;

	box 		box_;

	void selectAllPointsInBox();
	
public:

	// - type info
	TypeName("boxAll");


		boxAll(const pointStructure& pStruct, const dictionary& dict);

		add_vCtor
		(
			pStructSelector,
			boxAll,
			dictionary
		);

		virtual ~boxAll() = default;

	//// - Methods

		virtual const int32Vector& selectedPoinsts()const override
		{
			return selectedPoints_;
		}

		virtual int32Vector& selectedPoinsts() override
		{
			return selectedPoints_;
		}

};

} // pFlow


#endif //__pStructSelector_H__
