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

#include "multiRotatingAxis.hpp"
#include "multiRotatingAxisMotion.hpp"
#include "dictionary.hpp"

/// Construct from dictionary
FUNCTION_H
pFlow::multiRotatingAxis::multiRotatingAxis(const dictionary& dict)
:
	rotatingAxis(dict)
{}


FUNCTION_H
pFlow::multiRotatingAxis::multiRotatingAxis
(
	multiRotatingAxis* axisListPtr,
	const wordList& componentsNames,
	const dictionary& dict
)
:
	rotatingAxis(dict),
	axisList_(axisListPtr)
{

	if(!read(dict, componentsNames))
	{
		fatalErrorInFunction<<
		"  error in reading rotatingAxis from dictionary "<< dict.globalName()<<endl;
		fatalExit;
	}
}



FUNCTION_H
bool pFlow::multiRotatingAxis::read
(
	const dictionary& dict,
	const wordList& componentNames
)
{
			
	word rotAxis = dict.getValOrSet<word>("rotationAxis", "none");

	if(rotAxis == "none")
	{
		parentAxisIndex_ = static_cast<uint32>(-1);
	}
	else
	{
		if( auto i = componentNames.findi(rotAxis); i != -1 )
		{
			parentAxisIndex_ = i;
		}
		else
		{
			fatalErrorInFunction<<"crotationAxis "<< rotAxis<<" in dictionary "<<
			dict.globalName()<<" is not found in list of axis names "<< componentNames<<endl;
			return false;
		}
	}

	return true;
}

FUNCTION_H
bool pFlow::multiRotatingAxis::write
(
	dictionary& dict, 
	const wordList& componentNames
) const
{
	if( !rotatingAxis::write(dict) ) return false;
	
	if(parentAxisIndex_ == -1)
	{
		dict.add("rotationAxis", "none");
	}
	else
	{
		dict.add("rotationAxis", componentNames[parentAxisIndex_]);	
	}
	
	return true;
}
