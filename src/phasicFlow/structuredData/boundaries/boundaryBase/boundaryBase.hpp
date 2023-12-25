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

#ifndef __boundaryBase_hpp__
#define __boundaryBase_hpp__

#include "virtualConstructor.hpp"
#include "subscriber.hpp"
#include "VectorSingles.hpp"
#include "plane.hpp"
#include "scatterFieldAccess.hpp"

#include "streams.hpp"

namespace pFlow
{

// forward 

class internalPoints;
class dictionary;

class boundaryBase
:
	public subscriber
{
public:

    using pointFieldAccessType = 
        scatterFieldAccess<realx3,DefaultExecutionSpace>;

	enum DIRECTION: int8
	{
		
	};

protected:

	const plane& 	boundaryPlane_;

	/// The length defined for creating neighbor list 
	real  			neighborLength_;	

	/// a reference to 
	internalPoints& internal_;

	/// list of particles indices on device 
	uint32Vector_D 	indexList_;

	uint32 			mirrorProcessoNo_;

	word 		name_;

	word 		type_;


public:

	TypeInfo("boundaryBase");

	/*boundaryBase(
		const plane& 		bplane,
		uint32 				mirrorProc, 
		const word& 		name,
		const word& 		type,
		internalPoints& 	internal);*/

	boundaryBase(
		const dictionary& 	dict,
		const plane& 		bplane,
		internalPoints& 	internal);

	boundaryBase(const boundaryBase&) = delete;

	boundaryBase& operator=(const boundaryBase&) = delete;

	boundaryBase(boundaryBase&&) = default;

	boundaryBase& operator=(boundaryBase&&) = default;

	virtual ~boundaryBase() = default;
	

	create_vCtor
	(
		boundaryBase,
		dictionary,
		(
			const dictionary& 	dict,
			const plane& 		bplane,
			internalPoints& 	internal
		),
		(dict, bplane, internal)
	);

	virtual 
    bool beforeIteratoin(uint32 iterNum, real t) = 0 ;

	virtual 
    bool iterate(uint32 iterNum, real t) = 0;

	virtual 
    bool afterIteration(uint32 iterNum, real t) = 0;

    pointFieldAccessType thisPoints();

    virtual
    pointFieldAccessType neighborPoints();
    

	static
	uniquePtr<boundaryBase> create
	(
		const dictionary& 	dict,
		const plane& 		bplane,
		internalPoints& 	internal
	);

	
};

}


#endif //__boundaryBase_hpp__