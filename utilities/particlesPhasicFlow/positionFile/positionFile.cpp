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

#include "error.hpp"
#include "dictionary.hpp"
#include "positionFile.hpp"
#include "iFstream.hpp"

bool pFlow::positionFile::positionPointsFile()
{
	std::cout << "Reading user defined position file....";

	position_.clear();

	// ToDo: read position data from file.

	std::ifstream inFile(fileName_);

	inFile >> numPoints_;

	realx3 tempPoint;

	for(int i = 0; i < numPoints_; i++)
	{
		inFile >> tempPoint.x_ >> tempPoint.y_ >> tempPoint.z_;
		position_.push_back(tempPoint);
	}

	std::cout << "Done!" << std::endl;

	return true;
}

pFlow::positionFile::positionFile
(
	systemControl& control,
	const dictionary& dict
)
:
	positionParticles(control, dict),
	poDict_
	(  
		dict.subDict("fileInfo")
	),
	fileName_
	(
		poDict_.getVal<word>("name")
	),
	numPoints_
	(
		poDict_.getVal<uint64>("numPoints")
	),
	position_
	(
		"position", 
		max(maxNumberOfParticles(), numPoints_), 
		numPoints_ ,
		RESERVE()
	)
{
	if(!positionPointsFile())
	{
		fatalExit;
	}
}