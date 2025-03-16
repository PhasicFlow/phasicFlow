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
{
	if(!read(nullptr, dict))
	{
		fatalErrorInFunction<<
		"  error in reading multiRotatingAxis from dictionary "<< dict.globalName()<<endl;
		fatalExit;
	}
}

FUNCTION_H
pFlow::multiRotatingAxis::multiRotatingAxis
(
	multiRotatingAxisMotion* axisMotion
)
{
	//axisList_ = axisMotion->getAxisListPtr();	
}

FUNCTION_H
pFlow::multiRotatingAxis::multiRotatingAxis
(
	multiRotatingAxisMotion* axisMotion,
	const dictionary& dict
)
{

	if(!read(axisMotion, dict))
	{
		fatalErrorInFunction<<
		"  error in reading rotatingAxis from dictionary "<< dict.globalName()<<endl;
		fatalExit;
	}

	//axisList_ = axisMotion->getAxisListPtr();
}



FUNCTION_H
bool pFlow::multiRotatingAxis::read
(
	multiRotatingAxisMotion* axisMotion,
	const dictionary& dict
)
{

	if(!rotatingAxis::read(dict))return false;
			
	word rotAxis = dict.getValOrSet<word>("rotationAxis", "none");

	if(rotAxis == "none")
	{
		parentAxisIndex_ = -1;
	}
	else
	{
		bool result = axisMotion-> nameToIndex(rotAxis, parentAxisIndex_);
	}

	return true;
}

FUNCTION_H
bool pFlow::multiRotatingAxis::write
(
	const multiRotatingAxisMotion* axisMotion,
	dictionary& dict
) const
{
	if( !rotatingAxis::write(dict) ) return false;
	
	if(parentAxisIndex_ == -1)
	{
		dict.add("rotationAxis", "none");
	}
	else
	{
		word rotAxis;
		bool result  = axisMotion->indexToName(parentAxisIndex_, rotAxis);
		dict.add("rotationAxis", rotAxis);	
	}
	
	return true;
}


FUNCTION_H
bool pFlow::multiRotatingAxis::write
(
	dictionary& dict
) const
{
	// if( !rotatingAxis::write(dict) ) return false;
	
	// if(parentAxisIndex_ == -1)
	// {
	// 	dict.add("rotationAxis", "none");
	// }
	// else
	// {
	// 	word rotAxis;
	// 	bool result  = axisMotion->indexToName(parentAxisIndex_, rotAxis);
	// 	dict.add("rotationAxis", rotAxis);	
	// }
	
	return true;
}
