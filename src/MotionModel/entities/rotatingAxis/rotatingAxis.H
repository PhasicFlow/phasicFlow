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

#ifndef __rotatingAxis_H__
#define __rotatingAxis_H__

#include "line.H"

namespace pFlow
{

class dictionary;
class rotatingAxis;

#include "rotatingAxisFwd.H"

class rotatingAxis
:
	public line
{
protected:
	
	// rotation speed 
	real 	omega_ = 0;

	bool  rotating_ = false;

public:

	FUNCTION_HD
	rotatingAxis(){}

	FUNCTION_H
	rotatingAxis(const dictionary& dict);
	
	FUNCTION_HD
	rotatingAxis(const realx3& p1, const realx3& p2, real omega = 0.0);

	FUNCTION_HD
	rotatingAxis(const rotatingAxis&) = default;

	rotatingAxis& operator=(const rotatingAxis&) = default;

	FUNCTION_HD
	real setOmega(real omega);


	INLINE_FUNCTION_HD
	real omega()const 
	{
		return omega_;
	}

	INLINE_FUNCTION_HD
	realx3 linTangentialVelocityPoint(const realx3 &p)const;

	// - IO operation
	FUNCTION_H 
	bool read(const dictionary& dict);

	FUNCTION_H
	bool write(dictionary& dict) const;

	FUNCTION_H
	bool read(iIstream& is);

	FUNCTION_H
	bool write(iOstream& os)const;

};

inline iOstream& operator <<(iOstream& os, const rotatingAxis& ax)
{
	if(!ax.write(os))
	{
		fatalExit;
	}
	return os;
}

inline iIstream& operator >>(iIstream& is, rotatingAxis& ax)
{
	if( !ax.read(is) )
	{
		fatalExit;
	}
	return is;
}

}


#include "rotatingAxisI.H"

#endif
