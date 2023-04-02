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

#ifndef __fixedWall_hpp__
#define __fixedWall_hpp__


#include "types.hpp"
#include "typeInfo.hpp"
#include "Vectors.hpp"
#include "uniquePtr.hpp"



namespace pFlow
{

class dictionary;

/**
 * Fixed wall motion model for walls
 * 
 * This class is used for geometries that are fixed during simulation.
 * 
 * 
\verbatim
// In geometryDict file, this will defines fixed walls during simulation
...
motionModel fixedWall; 
...
\endverbatim
 */
class fixedWall
{
public:

	/** Motion model class to be passed to computational units/kernels for
	 *  transfing points and returning velocities at various positions
	 */
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
	
	/// Name
	const word name_ = "none";


	/// Read from a dictionary 
	bool readDictionary(const dictionary& dict);

	/// write to a dictionary
	bool writeDictionary(dictionary& dict)const;

public:
	
	/// Type info
	TypeInfoNV("fixedWall");

	// - Constructors 

		/// Empty
		fixedWall();

		/// Constructor with dictionary 
		fixedWall(const dictionary& dict);

		/// Copy constructor
		fixedWall(const fixedWall&) = default;

		/// Move constructor
		fixedWall(fixedWall&&) = default;

		/// Copy assignment
		fixedWall& operator=(const fixedWall&) = default;

		/// Move assignment 
		fixedWall& operator=(fixedWall&&) = default;

		/// Destructor 
		~fixedWall() = default;

	// - Methods 

		/// Return motion model
		/// t is the current simulation time 
		Model getModel(real t)const
		{
			return Model();
		}

		/// Name of the motion component to index 
		int32 nameToIndex(const word& name)const
		{
			return 0;
		}

		/// Index of motion component to name 
		word indexToName(label i)const
		{
			return name_;
		}

		/// Velocity at point p
		INLINE_FUNCTION_HD
		realx3 pointVelocity(label n, const realx3& p)const 
		{
			return zero3;
		}

		
		/// Transfer point p for dt seconds according to motion component n
		INLINE_FUNCTION_HD
		realx3 transferPoint(label n, const realx3 p, real dt)const
		{
			return p;
		}

		/// Transfer a vector of point pVec for dt seconds according to motion
		/// component n
		INLINE_FUNCTION_HD
		bool transferPoint(label n, realx3* pVec, size_t numP, real dt)
		{
			return true;		
		}

		/// Are walls moving 
		INLINE_FUNCTION_HD
		bool isMoving()const
		{
			return false;
		}

		/// Move points
		bool move(real t, real dt)
		{
			return true;
		}

	// - IO operations 

		/// Read from input stream is 
		FUNCTION_H
		bool read(iIstream& is);

		/// Write to output stream os 
		FUNCTION_H
		bool write(iOstream& os)const;	
};

} // pFlow

#endif //__fixed_hpp__
