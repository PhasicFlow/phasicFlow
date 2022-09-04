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

#ifndef __demInteraction_H__
#define __demInteraction_H__

#include "property.H"
#include "demComponent.H"
#include "pointFields.H"
#include "triSurfaceFields.H"

namespace pFlow
{

class demInteraction
:
	public property,
	public demComponent
{
protected:

public:
	demInteraction(systemControl& control)
	:
		property(),
		demComponent("interaction", control)
	{}

	demInteraction(systemControl& control, const fileSystem& file)
	:
		property(file),
		demComponent("interaction", control)
	{}



};


}

#endif //__interaction_H__ 
