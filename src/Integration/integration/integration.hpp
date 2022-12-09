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

#ifndef __integration_hpp__
#define __integration_hpp__


#include "virtualConstructor.hpp"
#include "Vectors.hpp"
#include "pointStructure.hpp"
#include "repository.hpp"


namespace pFlow
{


class integration
{
protected:

	repository& owner_;

	const word baseName_;

	const pointStructure& pStruct_;

public:

	// type info
	TypeInfo("integration");

	//// - Constructors
		integration(
			const word& baseName,
			repository& owner,
			const pointStructure& pStruct,
			const word& method);

		virtual ~integration()=default;

		// - add a virtual constructor 
		create_vCtor(
			integration,
			word,
			(const word& baseName,
			repository& owner,
			const pointStructure& pStruct,
			const word& method),
			(baseName, owner, pStruct, method) );


	//// - Methods

		const auto& pStruct()const
		{
			return pStruct_;
		}

		virtual bool predict(real dt, realx3Vector_D& y, realx3Vector_D& dy) = 0;

		virtual bool correct(real dt, realx3Vector_D& y, realx3Vector_D& dy) = 0;

		virtual bool setInitialVals(
				const int32IndexContainer& newIndices,
				const realx3Vector& y) = 0;

		virtual bool needSetInitialVals()const = 0;

		virtual uniquePtr<integration> clone()const=0;

		const word& baseName()const
		{
			return baseName_;
		}

		repository& owner()
		{
			return owner_;
		}

	static
		uniquePtr<integration> create(
			const word& baseName,
			repository& owner,
			const pointStructure& pStruct,
			const word& method);

};

} // pFlow


#endif //__integration_hpp__
