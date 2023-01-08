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

#ifndef __multiRotatingAxis_hpp__
#define __multiRotatingAxis_hpp__


#include "rotatingAxis.hpp"
#include "KokkosTypes.hpp"


namespace pFlow
{

class dictionary;
class multiRotatingAxisMotion;


class multiRotatingAxis
:
	public rotatingAxis
{
protected:

	// this is either host/device pointer 
	multiRotatingAxis*							axisList_;

	int32 										parentAxisIndex_ = -1;

public:

	INLINE_FUNCTION_HD
	multiRotatingAxis(){}

	FUNCTION_H
	multiRotatingAxis(multiRotatingAxisMotion* axisMotion);

	FUNCTION_H
	multiRotatingAxis(multiRotatingAxisMotion* axisMotion, const dictionary& dict);
	
	/*FUNCTION_HD
	multiRotatingAxis(const realx3& p1, const realx3& p2, real omega = 0.0);*/
 
	FUNCTION_HD
	multiRotatingAxis(const multiRotatingAxis&) = default;

	FUNCTION_HD
	multiRotatingAxis& operator=(const multiRotatingAxis&) = default;

	INLINE_FUNCTION_HD
	realx3 pointTangentialVel(const realx3& p)const
	{
		realx3 parentVel(0);
		auto parIndex = parentAxisIndex();

		while(parIndex != -1)
		{
			auto& ax = axisList_[parIndex];
			parentVel += ax.linTangentialVelocityPoint(p);
			parIndex = ax.parentAxisIndex();
		}

		return parentVel + rotatingAxis::linTangentialVelocityPoint(p);
	}



	INLINE_FUNCTION_HD
	realx3 transferPoint(const realx3& p, real dt)const
	{
		auto newP = p;

		// rotation around this axis
		if(isRotating())
		{
			newP = pFlow::rotate(p, *this, dt);
		}

		auto parIndex = parentAxisIndex_;
		while(parIndex != -1)
		{
			auto& ax = axisList_[parIndex];
			newP = pFlow::rotate(newP, ax, dt);
			parIndex = ax.parentAxisIndex();
		}

		return newP;
	}

	INLINE_FUNCTION_HD
	bool hasParrent()const
	{
		return parentAxisIndex_ > -1;
	}

	INLINE_FUNCTION_HD
	int32 parentAxisIndex()const
	{
		return parentAxisIndex_;
	}

	// this pointer is device pointer 
	INLINE_FUNCTION_H
	void setAxisList(multiRotatingAxis* axisList)
	{
		axisList_ = axisList;
	}
	// it is assumed that the axis with deepest level 
	// (with more parrents) is moved first and then
	// the axis with lower levels
	void move(real dt)
	{
		
		if( !hasParrent() ) return;
		
		auto lp1 = point1();
		auto lp2 = point2();

		lp1 = axisList_[parentAxisIndex()].transferPoint(lp1, dt);
		lp2 = axisList_[parentAxisIndex()].transferPoint(lp2, dt);

		set(lp1, lp2);
	}



	// - IO operation
	FUNCTION_H 
	bool read(multiRotatingAxisMotion* axisMotion, const dictionary& dict);

	FUNCTION_H
	bool write(const multiRotatingAxisMotion* axisMotion, dictionary& dict) const;

	/*FUNCTION_H
	bool read(iIstream& is);

	FUNCTION_H
	bool write(iOstream& os)const;*/

};

/*inline iOstream& operator <<(iOstream& os, const multiRotatingAxis& ax)
{
	if(!ax.write(os))
	{
		fatalExit;
	}
	return os;
}

inline iIstream& operator >>(iIstream& is, multiRotatingAxis& ax)
{
	if( !ax.read(is) )
	{
		fatalExit;
	}
	return is;
}*/

}


#endif
