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

#ifndef __InsertionRegion_hpp__
#define __InsertionRegion_hpp__


#include "insertionRegion.hpp"
#include "dictionary.hpp"

namespace pFlow
{

/**
 * This manages insertion of particles from a region based on the ShapeType
 *  
 */
template<typename ShapeType>
class InsertionRegion
:
	public insertionRegion
{
protected:

	/// Ref to Shapes
	const ShapeType& 	shapes_;

	static bool checkForContact(
				const realx3Vector& pos,
				const realVector& diams,
				const realx3& p,
				const real& d);

public:

	/// Type info
	TypeInfoTemplateNV("insertionRegion", ShapeType);

	// - Constructors

		/// Construct from dictionary	
		InsertionRegion(const dictionary& dict, const ShapeType& shapes);

		/// Copy
		InsertionRegion(const InsertionRegion<ShapeType>& ) = default;

		/// Move
		InsertionRegion(InsertionRegion<ShapeType>&&) = default;

		/// Copy assignment
		InsertionRegion<ShapeType>& operator=(const InsertionRegion<ShapeType>& ) = default;

		/// Copy assignment
		InsertionRegion<ShapeType>& operator=(InsertionRegion<ShapeType>&&) = default;

		/// Clone
		auto clone()const
		{
			return makeUnique<InsertionRegion<ShapeType>>(*this);
		}

		/// Clone ptr
		auto clonePtr()const
		{
			return new InsertionRegion<ShapeType>(*this);
		}

	// - Methods
		
		/// Insert particles at currentTime
		/// Check if currentTime is the right moment for 
		/// particle insertion. Fill the vectors name, pos and signal
		/// if particle insertion occured or not. 
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


#include "InsertionRegion.cpp"

#endif
