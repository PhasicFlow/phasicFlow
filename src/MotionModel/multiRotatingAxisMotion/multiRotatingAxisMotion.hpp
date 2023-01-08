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

class dictionary;

class multiRotatingAxisMotion
{
public:
	
	// - this class shuold be decleared in every motion model with 
	//   exact methods 
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

	axisVector_HD 		axis_;
	
	VectorDual<int32> 	sortedIndex_;

	wordList  			axisName_;

	label  				numAxis_= 0;



	bool readDictionary(const dictionary& dict);

	bool writeDictionary(dictionary& dict)const;

public:
	
	TypeInfoNV("multiRotatingAxisMotion");

	// empty
	FUNCTION_H
	multiRotatingAxisMotion();

	// construct with dictionary 
	FUNCTION_H
	multiRotatingAxisMotion(const dictionary& dict);

	// copy
	FUNCTION_H
	multiRotatingAxisMotion(const multiRotatingAxisMotion&) = default;

	multiRotatingAxisMotion(multiRotatingAxisMotion&&) = delete;

	FUNCTION_H
	multiRotatingAxisMotion& operator=(const multiRotatingAxisMotion&) = default;

	multiRotatingAxisMotion& operator=(multiRotatingAxisMotion&&) = delete;

	FUNCTION_H
	~multiRotatingAxisMotion() = default;


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

	INLINE_FUNCTION_H
	multiRotatingAxis* getAxisListPtrHost()
	{
		return axis_.hostVectorAll().data();
	}


	INLINE_FUNCTION_H
	multiRotatingAxis* getAxisListPtrDevice()
	{
		return axis_.deviceVectorAll().data();
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


	INLINE_FUNCTION_HD
	bool isMoving()const
	{
		return true;
	}

	FUNCTION_H
	bool move(real t, real dt);
	

	FUNCTION_H
	bool read(iIstream& is);

	FUNCTION_H
	bool write(iOstream& os)const;

	
};

} // pFlow

#endif //__multiRotatingAxisMotion_hpp__
