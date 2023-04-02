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

#ifndef __rotatingAxis_hpp__
#define __rotatingAxis_hpp__

#include "timeInterval.hpp"
#include "line.hpp"

namespace pFlow
{

class dictionary;
class rotatingAxis;

#include "rotatingAxisFwd.hpp"

/**
 * An axis which rotates around itself at specified speed
 * 
 * This defines an axis with two end points that rotates around 
 * itself at specified speed (rad/s).
 * 
 * 
 \verbatim
 // This creates an axis of rotation around x-axis, rotation starts at t = 1 s
 // and ends at t = 5 s. 
 {
  	p1 		(0 0 0);
  	p2		(1 0 0);
  	omega 	1.57;
  	startTime 	1;
  	endTime 	5;
 } \endverbatim
 * 
 * | Parameter | Type | Description | Optional [default value] |
 * |----| :---: | ---- | ---- |
 * | p1 | realx3 | begin point of axis | No |
 * | p2 | realx3 | end point of axis | No |
 * | omega | real | rotation speed (rad/s) | No |
 * | startTime | real | start time of rotation (s) | Yes [0] |
 * | endTime | real | end time of rotation (s) | Yes [infinity] |
 * 
 */
class rotatingAxis
:
	public timeInterval,
	public line
{
protected:
	
	/// rotation speed 
	real 	omega_ = 0;

	/// is rotating
	bool  rotating_ = false;

public:

	// - Constructor

	/// Empty constructor
	FUNCTION_HD
	rotatingAxis(){}

	/// Construct from dictionary
	FUNCTION_H
	rotatingAxis(const dictionary& dict);
	
	/// Construct from components
	FUNCTION_HD
	rotatingAxis(const realx3& p1, const realx3& p2, real omega = 0.0);

	/// Copy constructor
	FUNCTION_HD
	rotatingAxis(const rotatingAxis&) = default;

	/// Copy asssignment
	rotatingAxis& operator=(const rotatingAxis&) = default;

	/// Set omega
	FUNCTION_HD
	real setOmega(real omega);

	/// Return omega
	INLINE_FUNCTION_HD
	real omega()const 
	{
		return omega_;
	}

	/// Is rotating
	INLINE_FUNCTION_HD
	bool isRotating()const
	{
		return rotating_;
	}

	/// Linear tangential velocity at point p
	INLINE_FUNCTION_HD
	realx3 linTangentialVelocityPoint(const realx3 &p)const;

	// - IO operation

	/// Read from dictionary
	FUNCTION_H 
	bool read(const dictionary& dict);

	/// Write to dictionary 
	FUNCTION_H
	bool write(dictionary& dict) const;

	/// Read from input stream is
	FUNCTION_H
	bool read(iIstream& is);

	/// Write to output stream os
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


#include "rotatingAxisI.hpp"

#endif
