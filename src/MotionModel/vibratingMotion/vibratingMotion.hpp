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

#ifndef __vibratingMotion_hpp__
#define __vibratingMotion_hpp__


#include "types.hpp"
#include "typeInfo.hpp"
#include "VectorDual.hpp"
#include "Vectors.hpp"
#include "List.hpp"
#include "vibrating.hpp"



namespace pFlow
{

// forward
class dictionary;

/**
 * Vibrating motion model for walls
 * 
 * This class is used for simulaiton that at least one wall components 
 * are moving according to a sinoidal viration defined in class vibrating.
 * One or more than one motion components can be defined in 
 * vibratingMotionInfo dictionary
 * 
\verbatim
// In geometryDict file, this will defines vibrating walls during simulation
...
motionModel vibratingMotion;

vibratingMotionInfo
{
	vibComponent1
	{
		// the definition based on class vibrating
	}
	vibComponent2
	{
		// the definition based on calss vibrating
	}
}
...
\endverbatim
 *
 */
class vibratingMotion
{
public:
	
	/** Motion model class to be passed to computational units/kernels for
	 *  transfing points and returning velocities at various positions
	 */
	class Model
	{
	protected:
		
		deviceViewType1D<vibrating> 	components_;
		int32 							numComponents_=0;

	public:

		INLINE_FUNCTION_HD
		Model(deviceViewType1D<vibrating> comps, int32 numComps):
			components_(comps),
			numComponents_(numComps)
		{}

		INLINE_FUNCTION_HD
		Model(const Model&) = default;
		
		
		INLINE_FUNCTION_HD
		Model& operator=(const Model&) = default;


		INLINE_FUNCTION_HD
		realx3 pointVelocity(int32 n, const realx3& p)const
		{
			return components_[n].linTangentialVelocityPoint(p);
		}

		INLINE_FUNCTION_HD
		realx3 operator()(int32 n, const realx3& p)const
		{
			return pointVelocity(n,p);
		}

		INLINE_FUNCTION_HD
		realx3 transferPoint(int32 n, const realx3 p, real dt)const
		{
			return components_[n].transferPoint(p, dt);	
		}

		INLINE_FUNCTION_HD int32 numComponents()const
		{
			return numComponents_;
		}
	};

protected:

	using axisVector_HD = VectorDual<vibrating>;

	/// Vibrating motion components 
	axisVector_HD 	components_;
	
	/// Names of components
	wordList  		componentName_;

	/// Number of components
	label  			numComponents_= 0;

	/// Read from a dictionary
	bool readDictionary(const dictionary& dict);

	/// Write to a dictionary 
	bool writeDictionary(dictionary& dict)const;

public:
	
	/// Type info
	TypeInfoNV("vibratingMotion");

	/// Empty
	FUNCTION_H
	vibratingMotion();

	/// Construct with dictionary 
	FUNCTION_H
	vibratingMotion(const dictionary& dict);

	/// Copy constructor 
	FUNCTION_H
	vibratingMotion(const vibratingMotion&) = default;

	/// No move 
	vibratingMotion(vibratingMotion&&) = delete;

	/// Copy assignment 
	FUNCTION_H
	vibratingMotion& operator=(const vibratingMotion&) = default;

	/// No Move assignment 
	vibratingMotion& operator=(vibratingMotion&&) = delete;

	/// Destructor 
	FUNCTION_H
	~vibratingMotion() = default;

	/// Return motion model at time t 
	Model getModel(real t)
	{
		for(int32  i= 0; i<numComponents_; i++ )
		{
			components_[i].setTime(t);
		}
		components_.modifyOnHost();
		components_.syncViews();

		return Model(components_.deviceVectorAll(), numComponents_);
	}

	/// Name to component index 
	INLINE_FUNCTION_H
	int32 nameToIndex(const word& name)const
	{
		if( auto i = componentName_.findi(name); i == -1)
		{
			fatalErrorInFunction<<
			"component name " << name << " does not exist. \n";
			fatalExit;
			return i;
		}
		else
		{
			return i;
		}
		
	}

	/// Index to name 
	INLINE_FUNCTION_H
	word indexToName(label i)const
	{
		if(i < numComponents_ )
			return componentName_[i];
		else
		{
			fatalErrorInFunction<<
			"out of range access to the list of axes " << i <<endl<<
			" size of components_ is "<<numComponents_<<endl;
			fatalExit;
			return "";
		}
	}

	/// velocity at point p according to motion component n	
	INLINE_FUNCTION_H
	realx3 pointVelocity(label n, const realx3& p)const 
	{
		return components_.hostVectorAll()[n].linTangentialVelocityPoint(p);
	}

	/// Transfer point p for dt seconds based on motion component n 
	INLINE_FUNCTION_H
	realx3 transferPoint(label n, const realx3 p, real dt)const
	{
		return components_.hostVectorAll()[n].transferPoint(p, dt);
	}

	/// Is moving 
	INLINE_FUNCTION_HD
	bool isMoving()const
	{
		return true;
	}

	/// Move ponits at time t for dt seconds 
	INLINE_FUNCTION_H
	bool move(real t, real dt)
	{
		return true;
	}

	/// Read from input stream is 
	FUNCTION_H
	bool read(iIstream& is);

	/// Write to output stream os
	FUNCTION_H
	bool write(iOstream& os)const;

};

} // pFlow

#endif //__rotatingAxisMotion_hpp__
