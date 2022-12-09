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

#include "IOstream.hpp"
#include "iOstream.hpp"
#include "error.hpp"


unsigned int pFlow::IOstream::precision_ = 6;

pFlow::word pFlow::IOstream::staticName_("IOstream");


const pFlow::word& pFlow::IOstream::name() const
{
    return staticName_;
}


pFlow::word& pFlow::IOstream::name()
{
    return staticName_;
}

bool pFlow::IOstream::check(const char* operation) const
{
    return fatalCheck(operation);
}


bool pFlow::IOstream::fatalCheck(const char* operation) const
{
    const bool ok = !bad();

    if (!ok)
    {
    	fatalErrorInFunction
            << "error in IOstream " << name() << " for operation " << operation;
            fatalExit;
    }

    return ok;
}