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
:
	//public pointStructure
public eventObserver
{
protected:

	Time& time_;

	word 			integrationMethod_;

	pointStructure& 	pStruct_;

	realx3PointField_D&  	velocity_;

	uniquePtr<integration>  integrationPos_;

	uniquePtr<integration>  integrationVel_;

public:

	TypeName("dynamicPointStructure");

	dynamicPointStructure(Time& time, const word& integrationMethod);

	dynamicPointStructure(const dynamicPointStructure& ps) = default;

	/*dynamicPointStructure(const dynamicPointStructure& ps):
		pointStructure(ps),
		time_(ps.time_),
		integrationMethod_(ps.integrationMethod_),
		velocity_(ps.velocity_),
		integrationPos_(ps.integrationPos_->clone()),
		integrationVel_(ps.integrationVel_->clone())
	{

	}*/

		
		// - no move construct 
	dynamicPointStructure(dynamicPointStructure&&) = delete;

	// - copy assignment 
	//
	//   should be changed, may causs undefined behavior 
	//////////////////////////////////////////////////////////////
	dynamicPointStructure& operator=(const dynamicPointStructure&) = default;

	// - no move assignment 
	dynamicPointStructure& operator=(dynamicPointStructure&&) = delete;

	// - destructor 
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


	// - update data structure by inserting/setting new points 
	//   Notifies all the fields in the registered list of data structure
	//   and exclude the fields that re in the exclusionList
	//   retrun nullptr if it fails 
	/*FUNCTION_H
	virtual uniquePtr<int32IndexContainer> insertPoints(
		const realx3Vector& pos,
		const List<eventObserver*>& exclusionList={nullptr}
	)override;*/

	bool update(const eventMessage& msg) override;
	

};

}


#endif
