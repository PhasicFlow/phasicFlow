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


#ifndef __Control_hpp__
#define __Control_hpp__


// top-level entity repository for the whole application 
// Each application that is executed in pFlow, should have 
// settings/systemControl file in it. 
// This  repository holds two main repositories: Time and settings 

#include "systemControl.hpp"
#include "timeFolder.hpp"

namespace pFlow
{


inline systemControl& Control()
{
	static systemControl control_;
	return control_;
}

} // pFlow


#endif // __Control_hpp__
