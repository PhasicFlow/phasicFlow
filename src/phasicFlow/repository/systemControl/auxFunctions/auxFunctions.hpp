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

#ifndef __auxFunctions_hpp__
#define __auxFunctions_hpp__

#include "timeInfo.hpp"
#include "uniquePtr.hpp"
#include "virtualConstructor.hpp"

namespace pFlow
{

class systemControl;

class auxFunctions
{

public:

    TypeInfo("auxFunctions");

    auxFunctions(const systemControl& control){};

    virtual ~auxFunctions()=default;

    create_vCtor
    (
        auxFunctions,
        systemControl,
        (const systemControl& control),
        (control)
    );

    virtual 
    bool execute() = 0;
    
    virtual 
    bool write()const = 0 ; 

    static
    uniquePtr<auxFunctions> create(const systemControl& control);

};

} // namespace pFlow

#endif 