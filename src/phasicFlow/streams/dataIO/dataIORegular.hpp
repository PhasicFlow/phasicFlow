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

#ifndef __datIONoMPI_hpp__
#define __datIONoMPI_hpp__

#include "dataIO.hpp"

namespace pFlow
{
template<typename T>
class dataIORegular
:
	public dataIO<T>
{
protected:

    bool gatherData(span<T> data ) override
    {
        /// in serial mode, no gathering is required. 
        this->bufferSpan_ = data;
        return true;
    }

public:

	TypeInfo("dataIO<regular>");

	dataIORegular(const IOPattern& iop)
    :
	    dataIO<T>(iop)
    {}

	dataIORegular(const dataIORegular&) = default;

	dataIORegular(dataIORegular&&) = default;

	dataIORegular& operator=(const dataIORegular&) = default;

	dataIORegular& operator=(dataIORegular&&) = default;

	~dataIORegular() = default;
	
};


}


#endif