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


template class pFlow::pointField<pFlow::int8, pFlow::HostSpace>; 
createBaseBoundary(pFlow::int8, pFlow::HostSpace);
createBoundary(pFlow::int8, pFlow::HostSpace, exit);

template class pFlow::pointField<pFlow::int8>; 
createBaseBoundary(pFlow::int8, void);
createBoundary(pFlow::int8, void, exit);


template class pFlow::pointField<pFlow::uint8, pFlow::HostSpace>; 
createBaseBoundary(pFlow::uint8, pFlow::HostSpace);
createBoundary(pFlow::uint8, pFlow::HostSpace, exit);

template class pFlow::pointField<pFlow::uint8>; 
createBaseBoundary(pFlow::uint8, void);
createBoundary(pFlow::uint8, void, exit);

template class pFlow::pointField<pFlow::int32, pFlow::HostSpace>; 
createBaseBoundary(pFlow::int32, pFlow::HostSpace);
createBoundary(pFlow::int32, pFlow::HostSpace, exit);

template class pFlow::pointField<pFlow::int32>; 
createBaseBoundary(pFlow::int32, void);
createBoundary(pFlow::int32, void, exit);

template class pFlow::pointField<pFlow::uint32, pFlow::HostSpace>; 
createBaseBoundary(pFlow::uint32, pFlow::HostSpace);
createBoundary(pFlow::uint32, pFlow::HostSpace, exit);

template class pFlow::pointField<pFlow::uint32>; 
createBaseBoundary(pFlow::uint32, void);
createBoundary(pFlow::uint32, void, exit);

template class pFlow::pointField<pFlow::real, pFlow::HostSpace>; 
createBaseBoundary(pFlow::real, pFlow::HostSpace);
createBoundary(pFlow::real, pFlow::HostSpace, exit);


template class pFlow::pointField<pFlow::real>; 
createBaseBoundary(pFlow::real, void);
createBoundary(pFlow::real, void, exit);

template class pFlow::pointField<pFlow::realx3, pFlow::HostSpace>; 
createBaseBoundary(pFlow::realx3, pFlow::HostSpace);
createBoundary(pFlow::realx3, pFlow::HostSpace, exit);


template class pFlow::pointField<pFlow::realx3>; 
createBaseBoundary(pFlow::realx3, void);
createBoundary(pFlow::realx3, void, exit);


template class pFlow::pointField<pFlow::realx4, pFlow::HostSpace>; 
createBaseBoundary(pFlow::realx4, pFlow::HostSpace);
createBoundary(pFlow::realx4, pFlow::HostSpace, exit);


template class pFlow::pointField<pFlow::realx4>; 
createBaseBoundary(pFlow::realx4, void);
createBoundary(pFlow::realx4, void, exit);

template class pFlow::pointField<pFlow::word, pFlow::HostSpace>; 
createBaseBoundary(pFlow::word, pFlow::HostSpace);
createBoundary(pFlow::word, pFlow::HostSpace, exit);