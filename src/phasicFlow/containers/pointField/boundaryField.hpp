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
#ifndef __boundaryField_hpp__
#define __boundaryField_hpp__

#include "observer.hpp"
#include "boundaryBase.hpp"
#include "Field.hpp"

namespace pFlow
{

template< template<class, class> class VectorField, class T, class MemorySpace = void>
class boundaryField
:
    public observer 
{
public:
	using FieldType = Field<VectorField, T, MemorySpace>;
protected:

    const boundaryBase&     boundary_;

    /// @brief a ref to the internal field 
    FieldType& internal_;

public:

   boundaryField(const boundaryBase& boundary, FieldType& internal);

	bool update
	(
		const message& msg, 
    	const anyList& varList
	) override
    {
		notImplementedFunction;
		return false;
	}

};

}

#include "boundaryField.cpp"

#endif

