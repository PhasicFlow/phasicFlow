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

#include "sphereParticles.hpp"
#include "systemControl.hpp"
#include "vocabs.hpp"
#include "sphereParticlesKernels.hpp"


//#include "setFieldList.hpp"
/*pFlow::uniquePtr<pFlow::List<pFlow::eventObserver*>> 
pFlow::sphereParticles::getFieldObjectList()const
{
	auto objListPtr = particles::getFieldObjectList();

	objListPtr().push_back(
		static_cast<eventObserver*>(&I_) );

	return objListPtr;
}

bool pFlow::sphereParticles::diameterMassInertiaPropId
(
	const word& shName,
	real& diam,
	real& mass,
	real& I,
	int8& propIdx
)
{
	uint32 idx;
	if( !shapes_.nameToIndex(shName, idx) )
	{
		printKeys(fatalErrorInFunction<<
		"  wrong shape name in the input: "<< shName<<endl<<
		"  available shape names are: ", shapes_.names())<<endl;
		return false;
	}

	diam = shapes_.diameter(idx);
	word materialName = shapes_.material(idx);
	uint32 pIdx;
	if( !property_.nameToIndex(materialName, pIdx) )
	{
		fatalErrorInFunction << 
		"  wrong material name "<< materialName <<" specified for shape "<< shName<<
		"  in the sphereShape dictionary.\n"<<
		"  available materials are "<< property_.materials()<<endl;
		return false;
	}
	real rho = property_.density(pIdx);

	mass = Pi/6.0*pow(diam,3.0)*rho;
	I = 0.4 * mass * pow(diam/2.0,2.0);
	propIdx= static_cast<int8>(pIdx);
	return true;
}

bool pFlow::sphereParticles::initializeParticles()
{
	
	int32IndexContainer indices(
		0, 
		static_cast<int32>(shapeName_.size()));
	
	return insertSphereParticles(shapeName_, indices, false);
}*/


/*bool pFlow::sphereParticles::beforeIteration() 
{
	particles::beforeIteration();
	
	intPredictTimer_.start();
	
	//INFO<<"before dyn predict"<<endINFO;
		dynPointStruct_.predict(this->dt(), accelertion_);
	//INFO<<"after dyn predict"<<endINFO;

	//INFO<<"before revel predict"<<endINFO;
		rVelIntegration_().predict(this->dt(),rVelocity_, rAcceleration_);
	//INFO<<"after rvel predict"<<endINFO;

	intPredictTimer_.end();

	return true;
}*/





/*bool pFlow::sphereParticles::afterIteration() 
{
	return true;
}*/


/*bool pFlow::sphereParticles::insertSphereParticles(
	const wordVector& names,
	const int32IndexContainer& indices,
	bool setId
	)
{

	if(names.size()!= indices.size())
	{
		fatalErrorInFunction <<
		"sizes of names ("<<names.size()<<") and indices ("
			<< indices.size()<<") do not match \n";
		return false;
	}

	auto len = names.size();
		
	realVector  diamVec(len, RESERVE());
	realVector  massVec(len, RESERVE());
	realVector  IVec(len, RESERVE());
	int8Vector  pIdVec(len, RESERVE());
	int32Vector IdVec(len, RESERVE());

	real d, m, I;
	int8 pId;

	ForAll(i, names )
	{
		
		if (diameterMassInertiaPropId(names[i], d, m, I, pId))
		{
			diamVec.push_back(d);
			massVec.push_back(m);
			IVec.push_back(I);
			pIdVec.push_back(pId);
			if(setId) IdVec.push_back(idHandler_.getNextId());
			//output<<" we are in sphereParticles nextId "<< idHandler_.nextId()<<endl;
		}
		else
		{
			fatalErrorInFunction<< "failed to calculate properties of shape " <<
			 names[i]<<endl;
			return false;
		}
		
	}

	if(!diameter_.insertSetElement(indices, diamVec))
	{
		fatalErrorInFunction<< " failed to insert diameters to the diameter field. \n";
		return false;
	}

	if(!mass_.insertSetElement(indices, massVec))
	{
		fatalErrorInFunction<< " failed to insert mass to the mass field. \n";
		return false;
	}

	if(!I_.insertSetElement(indices, IVec))
	{
		fatalErrorInFunction<< " failed to insert I to the I field. \n";
		return false;
	}

	if(!propertyId_.insertSetElement(indices, pIdVec))
	{
		fatalErrorInFunction<< " failed to insert propertyId to the propertyId field. \n";
		return false;
	}

	if(setId)
	{
		if( !id_.insertSetElement(indices, IdVec))
		{
			fatalErrorInFunction<< " failed to insert id to the id field. \n";
			return false;
		}	
	}
	

	return true;

}*/

bool pFlow::sphereParticles::initializeParticles()
{
	
	using exeSpace = typename realPointField_D::execution_space;
	using policy = Kokkos::RangePolicy<
			exeSpace,
			Kokkos::IndexType<uint32>>;
	
	auto [minIndex, maxIndex] = minMax(shapeIndex().internal());		

	if( !spheres_.indexValid(maxIndex) )
	{
		fatalErrorInFunction<< 
		"the maximum value of shapeIndex is "<< maxIndex << 
		" which is not valid."<<endl;
		return false;
	}

	auto aPointsMask 	= dynPointStruct().activePointsMaskDevice();
	auto aRange 		= aPointsMask.activeRange();
	
	auto field_shapeIndex 	= shapeIndex().deviceView();
	auto field_diameter 	= diameter_.deviceView();
	auto field_mass 		= mass_.deviceView();
	auto field_propId 		= propertyId_.deviceView();
	auto field_I 			= I_.deviceView();

	// get info from spheres shape 
	realVector_D d("diameter", spheres_.boundingDiameter());
	realVector_D mass("mass",spheres_.mass());
	uint32Vector_D propId("propId", spheres_.shapePropertyIds());
	realVector_D  I("I", spheres_.Inertia());

	auto d_d 		= d.deviceView();
	auto d_mass 	= mass.deviceView();
	auto d_propId 	= propId.deviceView();
	auto d_I 		= I.deviceView();
		
	Kokkos::parallel_for(
		"particles::initInertia",
		policy(aRange.start(), aRange.end()),
		LAMBDA_HD(uint32 i)
		{
			if(aPointsMask(i))
			{
				uint32 index = field_shapeIndex[i];
				field_I[i] = d_I[index];
				field_diameter[i] = d_d[index];
				field_mass[i] = d_mass[index];
				field_propId[i] = d_propId[index];
			}
		});
	Kokkos::fence();
	
	return true;
}

bool
pFlow::sphereParticles::getParticlesInfoFromShape(
  const wordVector& shapeNames,
  uint32Vector&     propIds,
  realVector&       diams,
  realVector&       m,
  realVector&       Is,
  uint32Vector&     shIndex
)
{
	auto numNew = static_cast<uint32>(shapeNames.size());

	propIds.clear();
	propIds.reserve(numNew);

	diams.clear();
	diams.reserve(numNew);

	m.clear();
	m.reserve(numNew);

	Is.clear();
	Is.reserve(numNew);

	shIndex.clear();
	shIndex.reserve(numNew);
	

	for(const auto& name:shapeNames)
	{
		uint32 indx;
		if(spheres_.shapeNameToIndex(name,indx))
		{
			shIndex.push_back(indx);
			Is.push_back( spheres_.Inertia(indx));
			m.push_back(spheres_.mass(indx));
			diams.push_back(spheres_.boundingDiameter(indx));
			propIds.push_back( spheres_.propertyId(indx));
		}
		else
		{	
			fatalErrorInFunction<<"Shape name "<< name << 
			"does not exist. The list is "<<spheres_.shapeNameList()<<endl;
			return false;
		}
	}
	
	return true;
}

pFlow::sphereParticles::sphereParticles(
	systemControl &control,
	const property& prop
)
:
	particles(control),
	spheres_
	(
		shapeFile__,
		&control.caseSetup(),
		prop
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
		dynPointStruct(),
		0u
	),
	diameter_
	(
		objectFile(
			"diameter",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER),
		dynPointStruct(),
		0.00000000001
	),
	mass_
	(
		objectFile(
			"mass",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER),
		dynPointStruct(),
		0.0000000001
	),
	I_
	(
		objectFile
		(
			"I",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER
		),
		dynPointStruct(),
		static_cast<real>(0.0000000001)
	),
	rVelocity_
	(
		objectFile
		(
			"rVelocity",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
		),
		dynPointStruct(),
		zero3
	),
	rAcceleration_
	(
		objectFile(
			"rAcceleration",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
			),
		dynPointStruct(),
		zero3
	),
	boundarySphereParticles_
	(
		dynPointStruct().boundaries(),
		*this
	),
	accelerationTimer_(
		"Acceleration", &this->timers() ),
	intPredictTimer_(
		"Integration-predict", &this->timers() ),
	intCorrectTimer_(
		"Integration-correct", &this->timers() ),
	fieldUpdateTimer_(
		"fieldUpdate", &this->timers() )
{
	
	auto intMethod = control.settingsDict().getVal<word>("integrationMethod");
	REPORT(1)<<"Creating integration method "<<Green_Text(intMethod)
		  << " for rotational velocity."<<END_REPORT;
		  
	rVelIntegration_ = integration::create
		(
			"rVelocity",
			dynPointStruct(),
			intMethod,
			rVelocity_.field()
		);

	if( !rVelIntegration_ )
	{
		fatalErrorInFunction<<
		"  error in creating integration object for rVelocity. \n";
		fatalExit;
	}

	WARNING<<"setFields for rVelIntegration_"<<END_WARNING;
	
	if(!initializeParticles())
	{
		fatalErrorInFunction;
		fatalExit;
	}
	
} 

/*bool pFlow::sphereParticles::update(const eventMessage& msg) 
{
	
	if(rVelIntegration_->needSetInitialVals())
	{
		
		
		auto indexHD = pStruct().insertedPointIndex();
	
		auto n = indexHD.size();
		auto index = indexHD.indicesHost();

		realx3Vector rvel(n,RESERVE());
		const auto hrVel = rVelocity_.hostView();

		for(auto i=0; i<n; i++)
		{
			rvel.push_back( hrVel[index(i)]);
		}
		
		rVelIntegration_->setInitialVals(indexHD, rvel);

	}	

	return true;
}*/

/*bool pFlow::sphereParticles::insertParticles
(
	const realx3Vector& position,
 	const wordVector&  shapes,
 	const setFieldList& setField
 )
{
	
	if( position.size() != shapes.size() )
	{
		fatalErrorInFunction<< 
		"  size of vectors position ("<<position.size()<<
		") and shapes ("<<shapes.size()<<") are not the same. \n";
		return false;
	}



	auto exclusionListAllPtr = getFieldObjectList(); 
	
	auto newInsertedPtr = pStruct().insertPoints( position, setField, time(), exclusionListAllPtr());
	

	if(!newInsertedPtr)
	{
		fatalErrorInFunction<<
		"  error in inserting points into pStruct. \n";
		return false;
	}

	auto& newInserted = newInsertedPtr();
	
	if(!shapeName_.insertSetElement(newInserted, shapes))
	{
		fatalErrorInFunction<<
		"  error in inserting shapes into sphereParticles system.\n";
		return false;
	}	

	if( !insertSphereParticles(shapes, newInserted) )
	{
		fatalErrorInFunction<< 
		"error in inserting shapes into the sphereParticles. \n";
		return false;
	}
	

	auto activeR = this->activeRange();

	REPORT(1)<< "Active range is "<<yellowText("["<<activeR.first<<", "<<activeR.second<<")")<<
	" and number of active points is "<< cyanText(this->numActive())<<
	" and pointStructure capacity is "<<cyanText(this->capacity())<<endREPORT;

	return true;
		
}*/

bool pFlow::sphereParticles::beforeIteration()
{
	particles::beforeIteration();
	intPredictTimer_.start();
		auto dt = this->dt();
		dynPointStruct().predict(dt, accelertion());
		rVelIntegration_().predict(dt,rVelocity_, rAcceleration_);
	intPredictTimer_.end();

	fieldUpdateTimer_.start();
	propertyId_.updateBoundariesSlaveToMasterIfRequested();
	diameter_.updateBoundariesSlaveToMasterIfRequested();
	mass_.updateBoundariesSlaveToMasterIfRequested();
	I_.updateBoundariesSlaveToMasterIfRequested();
	rVelocity_.updateBoundariesSlaveToMasterIfRequested();
	rAcceleration_.updateBoundariesSlaveToMasterIfRequested();
	rVelIntegration_().updateBoundariesSlaveToMasterIfRequested();
	fieldUpdateTimer_.end();
	
	return true;
}

bool pFlow::sphereParticles::iterate() 
{

	timeInfo ti = TimeInfo();
	realx3 g = control().g();

	particles::iterate();
	accelerationTimer_.start();
		pFlow::sphereParticlesKernels::acceleration(
			g,
			mass().deviceViewAll(),
			contactForce().deviceViewAll(),
			I().deviceViewAll(),
			contactTorque().deviceViewAll(),
			dynPointStruct().activePointsMaskDevice(),
			accelertion().deviceViewAll(),
			rAcceleration().deviceViewAll()
			);
		for(auto& bndry:boundarySphereParticles_)
		{
			bndry->acceleration(ti, g);
		}
	accelerationTimer_.end();
	
	intCorrectTimer_.start();
	
		if(!dynPointStruct().correct(dt(), accelertion()))
		{
			return false;
		}
		if(!rVelIntegration_().correct(
			dt(), 
			rVelocity_, 
			rAcceleration_))
		{
			return false;
		}
	
	intCorrectTimer_.end();
	
	return true;
}

bool pFlow::sphereParticles::insertParticles
(
	const realx3Vector &position, 
	const wordVector &shapesNames, 
	const anyList &setVarList
)
{
	anyList newVarList(setVarList);

	realVector mass("mass");
	realVector I("I");
	realVector diameter("diameter");
	uint32Vector propId("propId");
	uint32Vector shapeIdx("shapeIdx");

	if(!getParticlesInfoFromShape(
		shapesNames,
		propId,
		diameter,
		mass,
		I,
		shapeIdx))
	{
		return false;
	}

	newVarList.emplaceBack(
		mass_.name()+"Vector",
		std::move(mass));

	newVarList.emplaceBack(
		I_.name()+"Vector",
		std::move(I));
	
	newVarList.emplaceBack(
		diameter_.name()+"Vector",
		std::move(diameter));

	newVarList.emplaceBack(
		propertyId_.name()+"Vector",
		std::move(propId));

	newVarList.emplaceBack(
		shapeIndex().name()+"Vector",
		std::move(shapeIdx));
	
	if(!dynPointStruct().insertPoints(position, newVarList))
	{
		return false;
	}

    return true;
}

pFlow::word pFlow::sphereParticles::shapeTypeName()const
{
	return "sphere";
}

const pFlow::shape &pFlow::sphereParticles::getShapes() const
{
    return spheres_;
}

void pFlow::sphereParticles::boundingSphereMinMax
(
	real & minDiam, 
	real& maxDiam
)const
{
	minDiam = spheres_.minBoundingSphere();
	maxDiam = spheres_.maxBoundingSphere();
}
