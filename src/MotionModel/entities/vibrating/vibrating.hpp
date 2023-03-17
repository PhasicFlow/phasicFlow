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

#ifndef __vibrating_hpp__
#define __vibrating_hpp__


#include "timeInterval.hpp"

#include "streams.hpp"

namespace pFlow
{

class dictionary;
class vibrating;


class vibrating
:
	public timeInterval
{

protected:
	
	// rotation speed 
	realx3	angularFreq_{0,0,0};

	realx3 	phaseAngle_{0,0,0};

	realx3  amplitude_{0,0,0};

	realx3  velocity_{0,0,0};

	realx3  velocity0_{0,0,0};

	INLINE_FUNCTION_HD
	void calculateVelocity()
	{
		if(inTimeRange())
		{
			velocity_ = amplitude_ * sin( angularFreq_*(time()-startTime() ) + phaseAngle_ );
		}else
		{
			velocity_ = {0,0,0};
		}
	}

public:

	FUNCTION_HD
	vibrating(){}

	FUNCTION_H
	vibrating(const dictionary& dict);
	

	FUNCTION_HD
	vibrating(const vibrating&) = default;

	vibrating& operator=(const vibrating&) = default;

	INLINE_FUNCTION_HD
	void setTime(real t)
	{
		if( !equal(t, time()) ) velocity0_ = velocity_;
		timeInterval::setTime(t);
		calculateVelocity();
	}

	INLINE_FUNCTION_HD
	realx3 linTangentialVelocityPoint(const realx3 &p)const
	{
		return velocity_;
	}

	INLINE_FUNCTION_HD
	realx3 transferPoint(const realx3& p, real dt)
	{
		if(!inTimeRange()) return p;
		return p + static_cast<real>(0.5)*dt*(velocity0_+velocity_);
	}

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

inline iOstream& operator <<(iOstream& os, const vibrating& obj)
{
	if(!obj.write(os))
	{
		fatalExit;
	}
	return os;
}

inline iIstream& operator >>(iIstream& is, vibrating& obj)
{
	if( !obj.read(is) )
	{
		fatalExit;
	}
	return is;
}

}


#endif
