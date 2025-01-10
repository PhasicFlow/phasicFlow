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

#ifndef __createDataIO_hpp__
#define __createDataIO_hpp__

#include "uniquePtr.hpp"
#include "dataIO.hpp"
#include "dataIORegular.hpp"
#include "dataIOMPI.hpp"

namespace pFlow
{

template<typename T>
uniquePtr<dataIO<T>> createDataIO(
    const word& runType, const IOPattern& iop)
{
    if(runType == "regular")
    {
        return makeUnique<dataIORegular<T>>(iop);
    }
    else if(runType == "MPI")
    {
        return makeUnique<dataIOMPI<T>>(iop);
    }
    else
    {
        fatalErrorInFunction<< "not dataIO pattern is specified for this "<<
        runType<<endl;
        fatalExit;
    }
    return nullptr;
}

}

#endif