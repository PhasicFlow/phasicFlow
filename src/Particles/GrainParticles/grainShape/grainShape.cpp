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

#include "grainShape.hpp"


bool pFlow::grainShape::readFromDictionary3()
{

	grainDiameters_ = getVal<realVector>("grainDiameters");

	sphereDiameters_ = getVal<realVector>("sphereDiameters");

	coarseGrainFactor_ = grainDiameters_ / sphereDiameters_ ;


	if(grainDiameters_.size() != numShapes() )
	{
		fatalErrorInFunction<< 
		" number of elements in grain diameters in "<< globalName()<<" is not consistent"<<endl;
		return false;
	}

	if(sphereDiameters_.size() != numShapes() )
	{
		fatalErrorInFunction<< 
		" number of elements in sphere diameters in "<< globalName()<<" is not consistent"<<endl;
		return false;
	}

	return true;
}

bool pFlow::grainShape::writeToDict(dictionary& dict)const 
{
	
	if(!shape::writeToDict(dict))return false;

	return true;
}

pFlow::grainShape::grainShape
(
	const word& fileName,
	repository* owner,
	const property& prop
)
:
	shape(fileName, owner, prop)
{

	if(!readFromDictionary3())
	{
		fatalExit;
		fatalErrorInFunction;
	}
}

pFlow::grainShape::grainShape
(
	const word &shapeType, 
	const word &fileName, 
	repository *owner, 
	const property &prop
)
:
	grainShape(fileName, owner, prop)
{
}

pFlow::real pFlow::grainShape::maxBoundingSphere() const
{
    return max(grainDiameters_);
}

pFlow::real pFlow::grainShape::minBoundingSphere() const
{
    return min(grainDiameters_);
}

bool pFlow::grainShape::boundingDiameter(uint32 index, real &bDiam) const
{
	if( indexValid(index))
	{
		bDiam = grainDiameters_[index];
		return true;
	}
    return false;
}

pFlow::real pFlow::grainShape::boundingDiameter(uint32 index) const
{
    if(indexValid(index))
	{
		return grainDiameters_[index];
	}

	fatalErrorInFunction
		<<"Invalid index for diameter "
		<<index<<endl;
	fatalExit;
	
	return 0.0;
}

pFlow::realVector pFlow::grainShape::boundingDiameter() const
{
    return grainDiameters_;
}

pFlow::real pFlow::grainShape::coarseGrainFactor(uint32 index) const
{
    if(indexValid(index))
	{
		return coarseGrainFactor_[index];
	}
	fatalErrorInFunction<<"Invalid index for coarse Grain Factor "<<
	index<<endl;
	fatalExit;
	return 0.0;
}

pFlow::realVector pFlow::grainShape::volume() const
{
    return realVector("volume", Pi/6*pow(grainDiameters_,(real)3.0));
}

pFlow::realVector pFlow::grainShape::coarseGrainFactor() const
{
    return coarseGrainFactor_;
}

pFlow::real pFlow::grainShape::originalDiameter(uint32 index) const
{
	if(indexValid(index))
	{
    return sphereDiameters_[index];
	}
    fatalErrorInFunction<<"Invalid index for sphere diameter "<<
	index<<endl;
	fatalExit;
	return 0.0;
}



pFlow::realVector pFlow::grainShape::originalDiameter() const
{
    return sphereDiameters_;
}

bool pFlow::grainShape::mass(uint32 index, real &m) const
{
    if( indexValid(index) )
	{
		real d = grainDiameters_[index];
		real rho = indexToDensity(index);
		m = Pi/6.0*pow(d,3)*rho;
		return true;
	}
	return false;
}

pFlow::real pFlow::grainShape::mass(uint32 index) const
{
    if(real m; mass(index, m))
	{
		return m;
	}
	fatalErrorInFunction<<"bad index for mass "<< index<<endl;
	fatalExit;
	return 0;
}

pFlow::realVector pFlow::grainShape::mass() const
{
    return realVector ("mass", Pi/6*pow(grainDiameters_,(real)3.0)*density());
}

pFlow::realVector pFlow::grainShape::density()const 
{
	auto pids = shapePropertyIds();
	realVector rho("rho", numShapes());
	ForAll(i, pids)
	{
		rho[i] = properties().density(pids[i]);
	}
	return rho;
}

bool pFlow::grainShape::Inertia(uint32 index, real &I) const
{
	if( indexValid(index) )
	{
		I = 0.4 * mass(index) * pow(grainDiameters_[index]/2.0,2.0);
		return true;
	}
    return false;
}

pFlow::real pFlow::grainShape::Inertia(uint32 index) const
{
	if(real I; Inertia(index, I))
	{
		return I;
	}
    fatalExit;
	return 0;
}

pFlow::realVector pFlow::grainShape::Inertia() const
{
    return realVector("I", (real)0.4*mass()*pow((real)0.5*grainDiameters_,(real)2.0));
}

bool pFlow::grainShape::Inertia_xx(uint32 index, real &Ixx) const
{
    return Inertia(index,Ixx);
}

pFlow::real pFlow::grainShape::Inertial_xx(uint32 index) const
{
    return Inertia(index);
}

bool pFlow::grainShape::Inertia_yy(uint32 index, real &Iyy) const
{
    return Inertia(index,Iyy);
}

pFlow::real pFlow::grainShape::Inertial_yy(uint32 index) const
{
    return Inertia(index);
}

bool pFlow::grainShape::Inertia_zz(uint32 index, real &Izz) const
{
    return Inertia(index,Izz);
}

pFlow::real pFlow::grainShape::Inertial_zz(uint32 index) const
{
    return Inertia(index);
}
