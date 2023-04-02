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

// forward
class dictionary;


/**
 * Vibrating model for a wall
 * 
 * Creates a sinoidal virating model for a wall. The viration is defined by
 * frequency, amplitude and phase angle. 
 * \f[ 
	\vec{v}(t) = \vec{A} sin(\vec{\omega}(t-startTime)+\vec{\phi}) 
  \f]
 \verbatim
 // This creates sinoidal vibration on the wall in x-direction. The viration
 // starts at t = 0 s and ends at t = 10 s. 
 {
  	angularFreq 	(10 0 0);
  	amplitude 		( 1 0 0);
  	phaseAngle		( 0 0 0);
  	startTime 		0;
  	endTime 		10;
 } \endverbatim
 * 
 * * 
 * | Parameter | Type | Description | Optional [default value] |
 * |----| :---: | ---- | :---: |
 * | angularFreq | realx3 | angular frequency of vibration (rad/s) | No |
 * | amplitude | realx3 | rotation speed (m/s) | No |
 * | phaseAngle | realx3 | phase angle (rad) | Yes [(0 0 0)] |
 * | startTime | real | start time of rotation (s) | Yes [0] |
 * | endTime | real | end time of rotation (s) | Yes [infinity] |
 * 
 */
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
