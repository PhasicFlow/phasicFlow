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
#include "scatteredFieldAccess.hpp"

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
        deviceScatteredFieldAccess<realx3>;

private:

	const plane& 	boundaryPlane_;

	/// list of particles indices on device 
	uint32Vector_D 	indexList_;

	/// The length defined for creating neighbor list 
	real  			neighborLength_;	

	/// a reference to 
	internalPoints& internal_;

	uint32 			mirrorProcessoNo_;

	word 		name_;

	word 		type_;

protected:

	void setNewIndices(deviceViewType1D<uint32> newIndices);

public:

	TypeInfo("boundaryBase");

	
	boundaryBase(
		const dictionary& 	dict,
		const plane& 		bplane,
		internalPoints& 	internal);

	boundaryBase(const boundaryBase&) = delete;

	boundaryBase& operator=(const boundaryBase&) = delete;

	boundaryBase(boundaryBase&&) = default;

	boundaryBase& operator=(boundaryBase&&) = default;

	~boundaryBase() override = default;
	

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

	inline 
	auto neighborLength()const
	{
		return neighborLength_;
	}


	const word& type()const
	{
		return type_;
	}

	const word& name()const
	{
		return name_;
	}

	bool empty()const
	{
		return indexList_.size()==0;
	}

	auto size()const
	{
		return indexList_.size();
	}

	const plane& boundaryPlane()const
	{
		return boundaryPlane_;
	}

	auto capacity()const
	{
		return indexList_.capacity();
	}

	const internalPoints& internal()const
	{
		return internal_;
	}

	internalPoints& internal()
	{
		return internal_;
	}

	/// @brief set the size of indexList 
	/// Always make sure that size+1 <= capacity
	void setSize(uint32 newSize);

	virtual 
    bool beforeIteration(uint32 iterNum, real t, real dt) = 0 ;

	virtual 
    bool iterate(uint32 iterNum, real t) = 0;

	virtual 
    bool afterIteration(uint32 iterNum, real t) = 0;

	
	const auto& indexList()const
	{
		return indexList_;
	}

	/*auto& indexList()
	{
		return indexList_;
	}*/

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