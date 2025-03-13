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

/**
 *  Defines an axis of rotation that rotates around itself and rotates around
 * another axis. 
 * 
 * This axis rotates around itself and can have one axis of rotation, and that
 * axis of rotation can have another axis of rotatoin and so on. 
 * 
 * 
 \verbatim
 // This creates an axis around x-axis. This axis rotates round itself at 
 // 1.57 rad/s and at the same time rotates around rotating axis axisName. 
 // axisName is separatly defined in the same dictionray. 
 axis1
 {
  	p1 			(0 0 0);
  	p2			(1 0 0);
  	omega 		1.57;
  	rotationAxis axisName;
  	startTime 	1;
  	endTime 	5;
 }

 axisName 
 {
 	p1 			(0 0 0);
 	p2 			(0 1 1);
 	omega 		3.14;
 } \endverbatim
 *   
 *   
 * | Parameter | Type | Description | Optional [default value] |
 * |----| :---: | ---- | :---: |
 * | p1 | realx3 | begin point of axis | No |
 * | p2 | realx3 | end point of axis | No |
 * | omega | real | rotation speed (rad/s) | No |
 * | rotationAxis | word | the axis rotates around this axis | Yes [none] |
 * | startTime | real | start time of rotation (s) | Yes [0] |
 * | endTime | real | end time of rotation (s) | Yes [infinity] |
 * 
 */
class multiRotatingAxis
:
	public rotatingAxis
{
protected:

	/// This is device pointer to all axes
	multiRotatingAxis*		axisList_;

	/// Index of parent axis
	uint32 					parentAxisIndex_ = static_cast<uint32>(-1);

public:

	TypeInfoNV("multiRotatingAxis");

	// - Constructors

		/// Empty Constructor
		INLINE_FUNCTION_HD
		multiRotatingAxis(){}

		/// Construct from dictionary
		FUNCTION_H
		explicit multiRotatingAxis(const dictionary& dict);

		/// Empty with list of axes
		FUNCTION_H
		multiRotatingAxis(multiRotatingAxisMotion* axisMotion);

		/// Construct from dictionary and list of axes
		FUNCTION_H
		multiRotatingAxis(multiRotatingAxisMotion* axisMotion, const dictionary& dict);
		
		/// Copy constructor 
		FUNCTION_HD
		multiRotatingAxis(const multiRotatingAxis&) = default;

		/// Copy assignment
		FUNCTION_HD
		multiRotatingAxis& operator=(const multiRotatingAxis&) = default;

		/// Destructor
		~multiRotatingAxis() = default;

	/// - Methods

		/// Tangential velocity at point p
		INLINE_FUNCTION_HD
		realx3 pointTangentialVel(const realx3& p)const
		{
			realx3 parentVel(0);
			auto parIndex = parentAxisIndex();

			while(parIndex != -1)
			{
				auto& ax = axisList_[parIndex];
				parentVel += ax.linVelocityPoint(p);
				parIndex = ax.parentAxisIndex();
			}

			return parentVel + rotatingAxis::linVelocityPoint(p);
		}

		/// Translate point p for dt seconds based on the axis information
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

		/// Does this axis have a parent
		INLINE_FUNCTION_HD
		bool hasParent()const
		{
			return parentAxisIndex_ > -1;
		}

		/// Return the index of parent axis
		INLINE_FUNCTION_HD
		int32 parentAxisIndex()const
		{
			return parentAxisIndex_;
		}

		/// Set the pointer to the list of all axes.
		/// This pointer is device pointer 
		INLINE_FUNCTION_H
		void setAxisListPtr(multiRotatingAxis* axisList)
		{
			axisList_ = axisList;
		}

		/**
		 * Move the end points of the axis 
		 * 
		 * This function moves the end points of this axis, if it has any parrents.
		 * It is assumed that the axis with deepest level (with more parrents) is
		 * moved first and then the axis with lower levels.
		 */
		void move(real dt)
		{
			
			if( !hasParent() ) return;
			
			auto lp1 = point1();
			auto lp2 = point2();

			lp1 = axisList_[parentAxisIndex()].transferPoint(lp1, dt);
			lp2 = axisList_[parentAxisIndex()].transferPoint(lp2, dt);

			set(lp1, lp2);
		}


	// - IO operation

		/// Read from dictionary
		FUNCTION_H 
		bool read(multiRotatingAxisMotion* axisMotion, const dictionary& dict);

		/// Write to dictionary
		FUNCTION_H
		bool write(const multiRotatingAxisMotion* axisMotion, dictionary& dict) const;

		/// Write to dictionary 
		FUNCTION_H
		bool write(dictionary& dict) const;

};


}


#endif
