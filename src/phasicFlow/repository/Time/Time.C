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


#include "Time.H"
#include "dictionary.H"
#include "vocabs.H"

pFlow::Time::Time
(
	repository* owner,
	const dictionary& setiingsDict
)
:
	repository("Time", "", owner),
	timeControl(setiingsDict),
	geometry_
	(
		geometryRepository_,
		geometryFolder__,
		this
	),
	integration_
	(
		integrationRepository__,
		integrationFolder__,
		this
	)
{
	
}

bool pFlow::Time::write
(
	bool verbose
) const
{
	if(outputToFile())
	{
		Report(0)<<"\nWriting to file at time: "<< cyanText(currentTimeWord())<<endReport;
		return repository::write(verbose);
	}
	return true;
}