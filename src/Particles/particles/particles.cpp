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


#include "particles.hpp"


pFlow::particles::particles
(
	systemControl& control
)
:
	observer(),
	demComponent("particles", control),
	dynPointStruct_(control),
	id_
	(	
		objectFile
		(
			"id",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
		),
		dynPointStruct_,
		static_cast<uint32>(-1)
	),
	propertyId_
	(
		objectFile
		(
			"propertyId",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER
		),
		dynPointStruct_,
		static_cast<int8>(0)
	),
	diameter_
	(
		objectFile
		(
			"diameter",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_ALWAYS
		),
		dynPointStruct_,
		0.00000000001
	),
	mass_
	(
		objectFile
		(
			"mass",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_ALWAYS
		),
		dynPointStruct_,
		0.0000000001
	),
	accelertion_
	(
		objectFile
		(
			"accelertion",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
			),
		dynPointStruct_,
		zero3
	),
	contactForce_
	(
		objectFile
		(
			"contactForce",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
		),
		dynPointStruct_,
		zero3
	),
	contactTorque_
	(
	
		objectFile
		(
			"contactTorque",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
		),
		dynPointStruct_,
		zero3
	)
{
	
	WARNING<<"Subscribe particles"<<END_WARNING;
	//this->subscribe(pStruct());

}

bool pFlow::particles::beforeIteration() 
{
	/*auto domain = this->control().domain();

	auto numMarked = dynPointStruct_.markDeleteOutOfBox(domain);
	
	if(time_.sortTime())
	{
		real min_dx, max_dx;
		boundingSphereMinMax(min_dx, max_dx);
		Timer t;
		t.start();
		REPORT(0)<<"Performing morton sorting on particles ...."<<endREPORT;
		if(!pStruct().mortonSortPoints(domain, max_dx))
		{
			fatalErrorInFunction<<"Morton sorting was not successful"<<endl;
			return false;
		}
		t.end();
		REPORT(1)<<"Active range is "<< pStruct().activeRange()<<endREPORT;
		REPORT(1)<<"It took "<< yellowText(t.totalTime())<<" seconds."<<endREPORT;
	}

	this->zeroForce();
	this->zeroTorque();*/

	return true;
}

/*pFlow::uniquePtr<pFlow::List<pFlow::eventObserver*>> 
pFlow::particles::getFieldObjectList()const
{
	auto objListPtr = makeUnique<pFlow::List<pFlow::eventObserver*>>();
	objListPtr().push_back(
		static_cast<eventObserver*>(&id_) );
		
	objListPtr().push_back(
		static_cast<eventObserver*>(&propertyId_) );

	objListPtr().push_back(
		static_cast<eventObserver*>(&diameter_) );

	objListPtr().push_back(
		static_cast<eventObserver*>(&mass_) );
	
	objListPtr().push_back(
		static_cast<eventObserver*>(&shapeName_) );
	
	return objListPtr;
}*/
