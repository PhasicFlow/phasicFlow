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

class shapeMixture
{

protected:
	// - list of shape names 
	wordVector 		names_;

	// - number composition 
	uint32Vector   	number_;

	// - number inserted of each shape 
	uint32Vector   	numberInserted_;

	uint32Vector      current_;

public:

	//- type Info
	TypeInfoNV("shapeMixture");

	//// - constrcutores 
		
		//
		shapeMixture(const dictionary & dict);

		shapeMixture(const shapeMixture&) = default;

		shapeMixture(shapeMixture&&) = default;

		shapeMixture& operator=(const shapeMixture&) = default;

		shapeMixture& operator=(shapeMixture&&) = default;


		uniquePtr<shapeMixture> clone()const
		{
			return makeUnique<shapeMixture>(*this);
		}

		shapeMixture* clonePtr()const
		{
			return new shapeMixture(*this);
		}


		//
		~shapeMixture() = default;

	//// - Methods 
		word getNextShapeName();


		void getNextShapeNameN(size_t n, wordVector& names);


		auto size()const {
			return names_.size();
		}

		auto totalInserted()const {
			return sum(numberInserted_);
		}

	//// - IO operatoins 
		bool read(const dictionary& dict);

		bool write(dictionary& dict) const;

};

} // pFlow

#endif //__shapeMixture_hpp__