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

class rotatingAxisMotion
{
public:
	
	// - this class shuold be decleared in every motion model with 
	//   exact methods 
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

	axisVector_HD 	axis_;
	
	wordList  		axisName_;

	label  			numAxis_= 0;

	bool readDictionary(const dictionary& dict);

	bool writeDictionary(dictionary& dict)const;

public:
	
	TypeInfoNV("rotatingAxisMotion");

	// empty
	FUNCTION_H
	rotatingAxisMotion();

	// construct with dictionary 
	FUNCTION_H
	rotatingAxisMotion(const dictionary& dict);

	// copy
	FUNCTION_H
	rotatingAxisMotion(const rotatingAxisMotion&) = default;

	rotatingAxisMotion(rotatingAxisMotion&&) = delete;

	FUNCTION_H
	rotatingAxisMotion& operator=(const rotatingAxisMotion&) = default;

	rotatingAxisMotion& operator=(rotatingAxisMotion&&) = delete;

	FUNCTION_H
	~rotatingAxisMotion() = default;


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

	
	/*INLINE_FUNCTION_D
	realx3 pointVelocity(label n, const realx3& p)const 
	{
		return axis_.deviceVectorAll()[n].linTangentialVelocityPoint(p);
	}*/

	

	/*INLINE_FUNCTION_D
	realx3 transferPoint(label n, const realx3 p, real dt)const
	{
		return rotate(p, axis_.deviceVectorAll()[n], dt);
	}

	
	INLINE_FUNCTION_D
	bool transferPoint(label n, realx3* pVec, size_t numP, real dt)
	{
		if( n>=numAxis_)return false;
		
		rotate( pVec, numP, axis_.deviceVectorAll()[n], dt);

		return true;		
	}*/

	INLINE_FUNCTION_HD
	bool isMoving()const
	{
		return true;
	}

	INLINE_FUNCTION_H
	bool move(real dt)
	{
		return true;
	}

	FUNCTION_H
	bool read(iIstream& is);

	FUNCTION_H
	bool write(iOstream& os)const;

	
};

} // pFlow

#endif //__rotatingAxisMotion_hpp__
