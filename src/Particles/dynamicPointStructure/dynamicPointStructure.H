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


#ifndef __dynamicPointStructure_H__
#define __dynamicPointStructure_H__

#include "Time.H"
#include "pointFields.H"
#include "integrations.H"
#include "uniquePtr.H"

namespace pFlow
{

class dynamicPointStructure
{
protected:

	Time& time_;

	const word 			integrationMethod_;

	pointStructure& 	pStruct_;

	realx3PointField_D&  	velocity_;

	uniquePtr<integration>  integrationPos_;

	uniquePtr<integration>  integrationVel_;

public:

	TypeName("dynamicPointStructure");

	dynamicPointStructure(Time& time, const word& integrationMethod);

	virtual ~dynamicPointStructure() = default;

	inline pointStructure& pStruct() 
	{
		return pStruct_;
	}

	inline const pointStructure& pStruct() const
	{
		return pStruct_;
	}

	inline const realx3PointField_D& velocity()const
	{
		return velocity_;
	}

	auto markDeleteOutOfBox(const box& domain)
	{
		return pStruct_.markDeleteOutOfBox(domain);
	}

	bool predict(real dt, realx3PointField_D& acceleration);

	bool correct(real dt, realx3PointField_D& acceleration);


};

}


#endif
