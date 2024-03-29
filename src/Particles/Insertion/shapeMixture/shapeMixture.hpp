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

#ifndef __shapeMixture_hpp__
#define __shapeMixture_hpp__


#include "Vectors.hpp"

namespace pFlow
{

class dictionary;

/**
 * Defines a mixture of particles for particle insertion.
 *
 * The mixture composition is defined based on the integer numbers.
 * For example, if there are 3 shape names in the simulaiotn
 * (shape1, shape2, and shape3), the mixture composition can be defined as:
 * \verbatim
 {
	shape1 4;
	shape2 2;
	shape3 6;
 }
 \endverbatim
 * 
 */
class shapeMixture
{
protected:

	/// List of shape names 
	wordVector 		names_;

	/// Number composition 
	uint32Vector   	number_;

	/// Number of inserted particles of each shape 
	uint32Vector   	numberInserted_;

	/// Current number of inserted
	uint32Vector      current_;

public:

	/// Type info
	TypeInfoNV("shapeMixture");

	// - Constrcutors 
		
		/// Construct from dictionary
		shapeMixture(const dictionary & dict);

		/// Copy
		shapeMixture(const shapeMixture&) = default;

		/// Move
		shapeMixture(shapeMixture&&) = default;

		/// Copy assignment 
		shapeMixture& operator=(const shapeMixture&) = default;

		/// Move assignment
		shapeMixture& operator=(shapeMixture&&) = default;

		/// Polymorphic copy
		uniquePtr<shapeMixture> clone()const
		{
			return makeUnique<shapeMixture>(*this);
		}

		/// Polymorphic copy
		shapeMixture* clonePtr()const
		{
			return new shapeMixture(*this);
		}


		/// Destructor 
		~shapeMixture() = default;

	// - Methods 

		/// The name of the next shape that should be inserted
		word getNextShapeName();

		/// The name of the n next shapes that should be inserted
		void getNextShapeNameN(size_t n, wordVector& names);

		/// Size of mixture (names)
		auto size()const {
			return names_.size();
		}

		/// Total number inserted particles
		auto totalInserted()const {
			return sum(numberInserted_);
		}

	// - IO operatoins 
	
		bool read(const dictionary& dict);

		bool write(dictionary& dict) const;

};

} // pFlow

#endif //__shapeMixture_hpp__