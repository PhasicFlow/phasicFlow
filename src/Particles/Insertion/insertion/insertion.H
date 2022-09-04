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

#ifndef __insertion_H__
#define __insertion_H__


#include "streams.H"

namespace pFlow
{

class particles;
class dictionary;

class insertion
{
protected:
	// - insertion active 
	Logical 	active_ = "No";

	// - check for collision / desabled for now 
	Logical  	checkForCollision_ = "No";

	// - particles 
	particles& 				  particles_;

	
	bool readInsertionDict(const dictionary& dict);

	bool writeInsertionDict(dictionary& dict)const;

public:

	// type info
	TypeName("insertion");

	insertion(particles& prtcl);

	virtual ~insertion() = default;

	bool isActive()const {
		return active_();
	}


	virtual bool read(iIstream& is) = 0;

	virtual bool write(iOstream& os)const = 0;


};
}

#endif
