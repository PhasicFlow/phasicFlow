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

#ifndef __rotatingAxisMotion_hpp__
#define __rotatingAxisMotion_hpp__


#include "types.hpp"
#include "typeInfo.hpp"
#include "VectorDual.hpp"
#include "Vectors.hpp"
#include "List.hpp"
#include "rotatingAxis.hpp"


namespace pFlow
{

class dictionary;

/**
 * Rotating axis motion model for walls
 * 
 * This class is used for simulaiton that at least one wall components 
 * is moving according to rotatingAxis motion mode. One or more than one
 * motion components can be defined in rotatingAxisMotionInfo dictionary
 * 
\verbatim
// In geometryDict file, this will defines rotating walls during simulation
...
motionModel rotatingAxisMotion;

rotatingAxisMotionInfo
{
	rotationAxis1
	{
		// the definition based on class rotatingAxis
	}
	rotatoinAxis2
	{
		// the definition based on calss rotatingAxis
	}
}
...
\endverbatim
 *
 */
class rotatingAxisMotion
{
public:
	
	/** Motion model class to be passed to computational units/kernels for
	 *  transfing points and returning velocities at various positions
	 */
	class Model
	{
	protected:
		
		deviceViewType1D<rotatingAxis> 	axis_;
		int32 							numAxis_=0;

	public:

		INLINE_FUNCTION_HD
		Model(deviceViewType1D<rotatingAxis> axis, int32 numAxis):
			axis_(axis),
			numAxis_(numAxis)
		{}

		INLINE_FUNCTION_HD
		Model(const Model&) = default;
		
		
		INLINE_FUNCTION_HD
		Model& operator=(const Model&) = default;


		INLINE_FUNCTION_HD
		realx3 pointVelocity(int32 n, const realx3& p)const
		{
			return axis_[n].linTangentialVelocityPoint(p);
		}

		INLINE_FUNCTION_HD
		realx3 operator()(int32 n, const realx3& p)const
		{
			return pointVelocity(n,p);
		}

		INLINE_FUNCTION_HD
		realx3 transferPoint(int32 n, const realx3 p, real dt)const
		{
			return rotate(p, axis_[n], dt);	
		}

		INLINE_FUNCTION_HD int32 numComponents()const
		{
			return numAxis_;
		}
	};

protected:

	using axisVector_HD = VectorDual<rotatingAxis>;

	/// Vector to store axes 
	axisVector_HD 	axis_;
	
	/// Names of axes
	wordList  		axisName_;

	/// Number of axes components
	label  			numAxis_= 0;

	/// Read from dictionary 
	bool readDictionary(const dictionary& dict);

	/// Write to dictionary
	bool writeDictionary(dictionary& dict)const;

public:
	
	/// Type info
	TypeInfoNV("rotatingAxisMotion");

	// - Constructors 
	
		/// Empty
		FUNCTION_H
		rotatingAxisMotion();

		/// Construct with dictionary 
		FUNCTION_H
		rotatingAxisMotion(const dictionary& dict);

		/// Copy constructor 
		FUNCTION_H
		rotatingAxisMotion(const rotatingAxisMotion&) = default;

		/// No move constructor 
		rotatingAxisMotion(rotatingAxisMotion&&) = delete;

		/// Copy assignment
		FUNCTION_H
		rotatingAxisMotion& operator=(const rotatingAxisMotion&) = default;

		/// No move assignment
		rotatingAxisMotion& operator=(rotatingAxisMotion&&) = delete;

		/// Destructor 
		FUNCTION_H
		~rotatingAxisMotion() = default;

	// - Methods
		/// Return the motion model at time t 
		Model getModel(real t)
		{
			for(int32 i= 0; i<numAxis_; i++ )
			{
				axis_[i].setTime(t);
			}
			axis_.modifyOnHost();
			axis_.syncViews();

			return Model(axis_.deviceVector(), numAxis_);
		}

		/// Motion component name to index
		INLINE_FUNCTION_H
		int32 nameToIndex(const word& name)const
		{
			if( auto i = axisName_.findi(name); i == -1)
			{
				fatalErrorInFunction<<
				"axis name " << name << " does not exist. \n";
				fatalExit;
				return i;
			}
			else
			{
				return i;
			}
			
		}

		/// Motion index to motion component name 
		INLINE_FUNCTION_H
		word indexToName(label i)const
		{
			if(i < numAxis_ )
				return axisName_[i];
			else
			{
				fatalErrorInFunction<<
				"out of range access to the list of axes " << i <<endl<<
				" size of axes_ is "<<numAxis_<<endl;
				fatalExit;
				return "";
			}
		}

		
		/// Are walls moving 
		INLINE_FUNCTION_HD
		bool isMoving()const
		{
			return true;
		}

		/// Move 
		INLINE_FUNCTION_H
		bool move(real t, real dt)
		{
			return true;
		}

	// - IO operation 
		/// Read from input stream is 
		FUNCTION_H
		bool read(iIstream& is);

		/// Write to output stream os
		FUNCTION_H
		bool write(iOstream& os)const;

};

} // pFlow

#endif //__rotatingAxisMotion_hpp__
