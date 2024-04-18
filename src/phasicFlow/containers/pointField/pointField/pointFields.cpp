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

#include "pointFields.hpp"
#include "createBoundaryFields.hpp"


#define createPointFields(DataType) 							\
  template class pFlow::pointField<DataType, pFlow::HostSpace>; \
  createBoundaryFields(DataType, pFlow::HostSpace); 			\
              													\
  template class pFlow::pointField<DataType>;   				\
  createBoundaryFields(DataType, void);

// uint8
createPointFields(pFlow::uint8);

/// uint32
createPointFields(pFlow::uint32);

/// uint64
createPointFields(pFlow::uint64);

/// real 
createPointFields(pFlow::real);

/// realx3
createPointFields(pFlow::realx3);


/// realx4
createPointFields(pFlow::realx4);


/// word, only on host
template class pFlow::pointField<pFlow::word, pFlow::HostSpace>; 
createBoundaryFields(pFlow::word, pFlow::HostSpace);
