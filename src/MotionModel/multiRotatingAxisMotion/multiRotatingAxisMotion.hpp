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

#ifndef __multiRotatingAxisMotion_hpp__
#define __multiRotatingAxisMotion_hpp__


#include "types.hpp"
#include "typeInfo.hpp"
#include "VectorDual.hpp"
#include "List.hpp"
#include "multiRotatingAxis.hpp"


namespace pFlow
{

// forward
class dictionary;

/**
 * Rotating axis motion model for walls
 * 
 * This class is used for simulaiton that at least one wall components 
 * is moving according to multiRotatingAxis motion mode. One or more than one
 * motion components can be defined in multiRotatingAxisMotionInfo dictionary
 * 
\verbatim
// In geometryDict file, this will defines multi-rotating walls during simulation
...
motionModel multiRotatingAxisMotion;

multiRotatingAxisMotionInfo
{
	rotationAxis1
	{
		// the definition based on class multiRotatingAxis
	}
	rotatoinAxis2
	{
		// the definition based on calss multiRotatingAxis
	}
}
...
\endverbatim
 *
 */
class multiRotatingAxisMotion
{
public:
	
	/** Motion model class to be passed to computational units/kernels for
	 *  transfing points and returning velocities at various positions
	 */
	class Model
	{
	protected:
		
		deviceViewType1D<multiRotatingAxis> 	axis_;
		int32 									numAxis_=0;

	public:

		INLINE_FUNCTION_HD
		Model(deviceViewType1D<multiRotatingAxis> axis, int32 numAxis):
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
			return axis_[n].pointTangentialVel(p);
		}

		INLINE_FUNCTION_HD
		realx3 operator()(int32 n, const realx3& p)const
		{
			return pointVelocity(n,p);
		}

		INLINE_FUNCTION_HD
		realx3 transferPoint(int32 n, const realx3 p, real dt)const
		{
			return axis_[n].transferPoint(p, dt);	
		}

		INLINE_FUNCTION_HD int32 numComponents()const
		{
			return numAxis_;
		}
	};

protected:

	using axisVector_HD = VectorDual<multiRotatingAxis>;

	/// Vector of multiRotaingAxis axes
	axisVector_HD 		axis_;
	
	/// Sorted index based on number of parrents each axis ha
	VectorDual<int32> 	sortedIndex_;

	/// List of axes names
	wordList  			axisName_;

	/// Number of axes
	label  				numAxis_= 0;

	/// Read from a dictionary
	bool readDictionary(const dictionary& dict);

	/// Write to a dictionary 
	bool writeDictionary(dictionary& dict)const;

public:
	
	/// Type info
	TypeInfoNV("multiRotatingAxisMotion");

	// - Constructor 

		/// Empty constructor 
		FUNCTION_H
		multiRotatingAxisMotion();

		/// Construct with dictionary 
		FUNCTION_H
		multiRotatingAxisMotion(const dictionary& dict);

		/// Copy constructor 
		FUNCTION_H
		multiRotatingAxisMotion(const multiRotatingAxisMotion&) = default;

		/// No Move 
		multiRotatingAxisMotion(multiRotatingAxisMotion&&) = delete;

		/// Copy assignment 
		FUNCTION_H
		multiRotatingAxisMotion& operator=(const multiRotatingAxisMotion&) = default;

		/// No move assignment 
		multiRotatingAxisMotion& operator=(multiRotatingAxisMotion&&) = delete;

		/// Destructor 
		FUNCTION_H
		~multiRotatingAxisMotion() = default;

	// - Methods 

		/// Retrun motion model at time t 
		Model getModel(real t)
		{
			for(int32 i= 0; i<numAxis_; i++ )
			{
				axis_[i].setTime(t);
				axis_[i].setAxisList(getAxisListPtrDevice());
			}
			axis_.modifyOnHost();
			axis_.syncViews();

			return Model(axis_.deviceVector(), numAxis_);
		}

		/// Pointer to axis list on host side 
		INLINE_FUNCTION_H
		multiRotatingAxis* getAxisListPtrHost()
		{
			return axis_.hostVectorAll().data();
		}

		/// Pointer to axis list on device 
		INLINE_FUNCTION_H
		multiRotatingAxis* getAxisListPtrDevice()
		{
			return axis_.deviceVectorAll().data();
		}

		/// Name of motion component to index 
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

		/// Index of motion component to component name 
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

		/// Is moving 
		INLINE_FUNCTION_HD
		bool isMoving()const
		{
			return true;
		}

		/// Move points 
		FUNCTION_H
		bool move(real t, real dt);
		
	// - IO operation 

		/// Read from input stream is 
		FUNCTION_H
		bool read(iIstream& is);

		/// Write to output stream os 
		FUNCTION_H
		bool write(iOstream& os)const;

	
};

} // pFlow

#endif //__multiRotatingAxisMotion_hpp__
