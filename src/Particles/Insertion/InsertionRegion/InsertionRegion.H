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

#ifndef __InsertionRegion_H__
#define __InsertionRegion_H__


#include "insertionRegion.H"
#include "dictionary.H"

namespace pFlow
{

template<typename ShapeType>
class InsertionRegion
:
	public insertionRegion
{
protected:
	// - type of particle shapes 
	const ShapeType& 	shapes_;

	static bool checkForContact(
				const realx3Vector& pos,
				const realVector& diams,
				const realx3& p,
				const real& d);

public:

	// - type info
	TypeNameTemplateNV("insertionRegion", ShapeType);

	InsertionRegion(const dictionary& dict, const ShapeType& shapes);

	InsertionRegion(const InsertionRegion<ShapeType>& ) = default;

	InsertionRegion(InsertionRegion<ShapeType>&&) = default;

	InsertionRegion<ShapeType>& operator=(const InsertionRegion<ShapeType>& ) = default;

	InsertionRegion<ShapeType>& operator=(InsertionRegion<ShapeType>&&) = default;


	auto clone()const
	{
		return makeUnique<InsertionRegion<ShapeType>>(*this);
	}

	auto clonePtr()const
	{
		return new InsertionRegion<ShapeType>(*this);
	}


	bool insertParticles
	(
		real currentTime,
		real dt,
		wordVector& names,
		realx3Vector& pos,
		bool& insertionOccured
	);

	//bool read(const dictionary& dict);

	//bool write(dictionary& dict)const;

};



} // pFlow


#include "InsertionRegion.C"

#endif
