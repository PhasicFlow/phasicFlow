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

#ifndef __createBoundaryFields_hpp__
#define __createBoundaryFields_hpp__

#include "boundaryField/boundaryField.hpp"
#include "exitBoundaryField/exitBoundaryField.hpp"
#include "periodicBoundaryField/periodicBoundaryField.hpp"
#include "reflectiveBoundaryField/reflectiveBoundaryField.hpp"

#define createDerivedBoundary(DataType, MemorySpaceType)                    \
	template class pFlow::exitBoundaryField<DataType, MemorySpaceType>;     \
	template class pFlow::periodicBoundaryField<DataType, MemorySpaceType>; \
	template class pFlow::reflectiveBoundaryField<DataType, MemorySpaceType>;

#define createBaseBoundary(DataType, MemorySpaceType) \
	template class pFlow::boundaryField<DataType, MemorySpaceType>;

#define createBoundaryFields(DataType, MemorySpaceType) \
	createBaseBoundary(DataType, MemorySpaceType);      \
	createDerivedBoundary(DataType, MemorySpaceType);

#endif //__createBoundaryFields_hpp__
