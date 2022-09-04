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

#ifndef __fixedWall_H__
#define __fixedWall_H__


#include "types.H"
#include "typeInfo.H"
#include "Vectors.H"
#include "uniquePtr.H"



namespace pFlow
{

class dictionary;

class fixedWall
{
public:

	// - this class shuold be decleared in every motion model with 
	//   exact methods 
	class Model
	{

	public:

		INLINE_FUNCTION_HD
		Model(){}

		INLINE_FUNCTION_HD
		Model(const Model&) = default;
		
		INLINE_FUNCTION_HD
		Model& operator=(const Model&) = default;


		INLINE_FUNCTION_HD
		realx3 pointVelocity(int32 n, const realx3 p)const
		{
			return 0.0;
		}

		INLINE_FUNCTION_HD
		realx3 operator()(int32 n, const realx3& p)const
		{
			return 0.0;
		}

		INLINE_FUNCTION_HD
		realx3 transferPoint(int32 n, const realx3 p, real dt)const
		{
			return p;
		}

		INLINE_FUNCTION_HD int32 numComponents()const
		{
			return 0;
		}
	};

protected:

	const word name_ = "none";

	bool readDictionary(const dictionary& dict);

	bool writeDictionary(dictionary& dict)const;

public:
	
	TypeNameNV("fixedWall");

	// empty
	fixedWall();

	// construct with dictionary 
	fixedWall(const dictionary& dict);

	fixedWall(const fixedWall&) = default;

	fixedWall(fixedWall&&) = default;

	fixedWall& operator=(const fixedWall&) = default;

	fixedWall& operator=(fixedWall&&) = default;

	~fixedWall() = default;

	Model getModel()const
	{
		return Model();
	}

	int32 nameToIndex(const word& name)const
	{
		return 0;
	}

	word indexToName(label i)const
	{
		return name_;
	}

	
	INLINE_FUNCTION_HD
	realx3 pointVelocity(label n, const realx3& p)const 
	{
		return zero3;
	}

	

	INLINE_FUNCTION_HD
	realx3 transferPoint(label n, const realx3 p, real dt)const
	{
		return p;
	}

	

	INLINE_FUNCTION_HD
	bool transferPoint(label n, realx3* pVec, size_t numP, real dt)
	{
		return true;		
	}

	INLINE_FUNCTION_HD
	bool isMoving()const
	{
		return false;
	}

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

#endif //__fixed_H__
