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


#ifndef __boxRegion_H__
#define __boxRegion_H__

#include "box.H"
#include "uniformRandomReal.H"

namespace pFlow
{

class boxRegion
{
protected:
	box 			  box_;

	mutable uniformRandomReal random_;
public:

	// - type info
	TypeNameNV("boxRegion");

	boxRegion(const dictionary& dict);

	~boxRegion() = default;

	//// - methods 
	bool isInside(const realx3& p) const;

	realx3 peek()const;


	//// IO operation
	bool read(const dictionary& dict);

	bool write(dictionary& dict)const;


};

}

#endif
