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

#include "Fields.hpp"


template class pFlow::Field<pFlow::VectorSingle, pFlow::int8>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::int8, pFlow::HostSpace>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::int16>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::int16, pFlow::HostSpace>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::int32>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::int32, pFlow::HostSpace>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::int64>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::int64, pFlow::HostSpace>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::uint32>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::uint32, pFlow::HostSpace>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::label>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::label, pFlow::HostSpace>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::real>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::real, pFlow::HostSpace>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::realx3>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::realx3, pFlow::HostSpace>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::realx3x3>;

template class pFlow::Field<pFlow::VectorSingle, pFlow::realx3x3, pFlow::HostSpace>;

template class pFlow::Field<pFlow::VectorDual, pFlow::int8>;

template class pFlow::Field<pFlow::VectorDual, pFlow::int16>;

template class pFlow::Field<pFlow::VectorDual, pFlow::int32>;

template class pFlow::Field<pFlow::VectorDual, pFlow::int64>;

template class pFlow::Field<pFlow::VectorDual, pFlow::uint32>;

template class pFlow::Field<pFlow::VectorDual, pFlow::label>;

template class pFlow::Field<pFlow::VectorDual, pFlow::real>;

template class pFlow::Field<pFlow::VectorDual, pFlow::realx3>;

template class pFlow::Field<pFlow::VectorDual, pFlow::realx3x3>;


template class pFlow::Field<pFlow::Vector, pFlow::word, pFlow::vecAllocator<pFlow::word>>;


