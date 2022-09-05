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


#ifndef __Wall_H__
#define __Wall_H__


#include "virtualConstructor.H"
#include "Vectors.H"
#include "dictionary.H"

namespace pFlow
{


bool checkNormalVec(
	const realx3 &p1,
	const realx3 &p2,
	const realx3 &p3,
	realx3& norm );


class Wall
{
protected:

	realx3x3Vector 	triangles_;

	word   			name_;

	word  			materialName_;

	word  			motionName_;

	bool readCommon(const dictionary& ditc);

public:

	// - type info
	TypeName("Wall");

	//// - Constructors

		// - empty
		Wall(){}

		// - from dictionary
		Wall(const dictionary& dict);

		
		virtual ~Wall()=default;

		create_vCtor
		(
			Wall,
			dictionary,
			(const dictionary& dict),
			(dict)
		);

	//// - Methods

		// - 
		const realx3x3Vector& triangles()const
		{
			return triangles_;
		}

		word name()const
		{
			return name_;
		}

		word materialName()const
		{
			return materialName_;
		}

		word motionName()const
		{
			return motionName_;
		}

		static 
			bool checkTrianlge(const realx3& p1, const realx3& p2, const realx3& p3);

		static
			uniquePtr<Wall> create(const dictionary& dict);

};

}


#endif
