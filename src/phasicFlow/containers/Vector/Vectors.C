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

#include "Vectors.H"

// instantiation just for numeral types 
template class pFlow::Vector<pFlow::int8>;

template class pFlow::Vector<pFlow::int16>;

template class pFlow::Vector<pFlow::int32>;

template class pFlow::Vector<pFlow::int64>;

template class pFlow::Vector<pFlow::uint32>;

template class pFlow::Vector<pFlow::label>;

template class pFlow::Vector<pFlow::real>;

template class pFlow::Vector<pFlow::realx3>;

template class pFlow::Vector<pFlow::realx3x3>;

//template class pFlow::Vector<pFlow::word>;

 


