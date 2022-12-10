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


#ifndef __particles_hpp__
#define __particles_hpp__

#include "dynamicPointStructure.hpp"
#include "particleIdHandler.hpp"
#include "demParticles.hpp"
namespace pFlow
{

class setFieldList;

class particles
:
	public eventObserver,
	public demParticles
{
protected:
	
	// owner repository 
	Time& time_;

	const word integrationMethod_;

	// dynamic point structure for particles 
	dynamicPointStructure 	dynPointStruct_;

	// - name of shapes - this is managed by particles 
	wordPointField&  		shapeName_;

	// id of particles on host
	int32PointField_HD&		id_;

	// property id on device
	int8PointField_D& 		propertyId_;

	// diameter / boundig sphere size of particles on device
	realPointField_D&		diameter_;

	// mass on device
	realPointField_D& 		mass_;

	// - acceleration on device
	realx3PointField_D& 	accelertion_;


	realx3PointField_D&     contactForce_;


	realx3PointField_D&     contactTorque_;

	
	// - object handling particle id
	particleIdHandler		idHandler_;

	virtual uniquePtr<List<eventObserver*>> getFieldObjectList()const;

	void zeroForce()
	{
		contactForce_.fill(zero3);
	}

	void zeroTorque()
	{
		contactTorque_.fill(zero3);
	}

public:

	// type info
	TypeInfo("particles");

	particles(systemControl& control, const word& integrationMethod);

	inline const auto& time()const {
		return time_;
	}

	inline auto& time() {
		return time_;
	}
	
	inline auto integrationMethod()const
	{
		return integrationMethod_;
	}

	inline const auto& dynPointStruct()const 
	{
		return dynPointStruct_;
	}

	inline auto& dynPointStruct()
	{
		return dynPointStruct_;
	}

	inline const auto& pStruct()const{
		return dynPointStruct_.pStruct();
	}

	inline auto& pStruct(){
		return dynPointStruct_.pStruct();
	}

	inline auto size()const{
		return pStruct().size();
	}

	inline auto capacity() const{
		return pStruct().capacity();
	}

	inline auto activePointsMaskD()const{
		return pStruct().activePointsMaskD();
	}

	inline auto numActive()const
	{
		return pStruct().numActive();
	}
	
	inline bool allActive()const{
		return pStruct().allActive();
	}

	inline auto activeRange()const{
		return pStruct().activeRange();
	}

	inline auto activePointsMaskH()const{
		return pStruct().activePointsMaskH();
	}

	inline const auto& pointPosition()const{
		return pStruct().pointPosition();
	}

	inline const auto& position()const
	{
		return pStruct().pointPosition();	
	}

	inline const auto& pointVelocity()const{
		return dynPointStruct().velocity();
	}

	inline const auto& velocity()const{
		return dynPointStruct().velocity();
	}

	inline const auto& id()const{
		return id_;
	}

	inline auto& id(){
		return id_;
	}

	inline const auto& diameter()const{
		return diameter_;
	}

	inline auto& diameter(){
		return diameter_;
	}

	inline const auto& mass()const{
		return mass_;
	}

	inline auto& mass()	{
		return mass_;
	}

	inline const auto& accelertion()const{
		return accelertion_;
	}

	inline auto& accelertion(){
		return accelertion_;
	}

	inline 
	realx3PointField_D& contactForce() 
	{
		return contactForce_;
	}

	inline
	const realx3PointField_D& contactForce() const
	{
		return contactForce_;
	}

	inline
	realx3PointField_D& contactTorque()
	{
		return contactTorque_;
	}

	inline
	const realx3PointField_D& contactTorque() const 
	{
		return contactTorque_;
	}

	inline const auto& propertyId()const{
		return propertyId_;
	}

	inline auto& propertyId(){
		return propertyId_;
	}

	inline const auto& shapeName()const{
		return shapeName_;
	}

	inline auto& shapName(){
		return shapeName_;
	}

	bool beforeIteration() override
	{
		auto domain = this->control().domain();

		auto numMarked = dynPointStruct_.markDeleteOutOfBox(domain);
		/*if(numMarked)
		{
			output<<"\nNumber of deleted points/particles that are out of domain box: "<<numMarked<<endl;
		}*/

		this->zeroForce();
		this->zeroTorque();

		return true;
	}

	virtual
	bool insertParticles
	(
		const realx3Vector& position,
	 	const wordVector&  shapes,
	 	const setFieldList& setField
	 ) = 0;

	

	virtual
	realx3PointField_D& rAcceleration() = 0;

	virtual
	const realx3PointField_D& rAcceleration() const = 0;

	virtual
	const realVector_D& boundingSphere()const = 0;

	virtual 
	word shapeTypeName()const = 0;

	virtual
	void boundingSphereMinMax(real & minDiam, real& maxDiam)const = 0;

	

}; // particles

} // pFlow

#endif //__particles_hpp__
