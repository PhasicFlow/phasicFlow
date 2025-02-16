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

#ifndef __positionFile_hpp__
#define __positionFile_hpp__

#include "positionParticles.hpp"

namespace pFlow
{


class positionFile 
:
	public positionParticles
{
private:

	dictionary 	poDict_;

	// word 		fileName_;

	fileSystem fileName_;
	
	realx3Vector 	position_;

	Logical commaSeparated_;

	bool positionPointsFile();

public:

	// - type Info
	TypeInfo("file");

	positionFile(
		systemControl& control,
		const dictionary& dict);

	// - add this class to vCtor selection table 
	add_vCtor(
		positionParticles,
		positionFile,
		dictionary);

	~positionFile() final = default;

	//// - Methods 

	uint32 numPoints()const final
	{
		return static_cast<uint32>(position_.size());
	}

	uint32 size()const final
	{
		return static_cast<uint32>(position_.size());
	}

	real maxDiameter() const final
	{
		return 0;
	}

	// bool commaSeparated()const
	// {
	// 	return commaSeparated_();
	// } 
	
	// - const access to position
	const realx3Vector& position()const final 
	{
		return position_;
	}

	// - access to position 
	realx3Vector& position() final
	{
		return position_;
	}
};	


}



#endif // __positionFile_hpp__
