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

#include "sphereShape.hpp"


bool pFlow::sphereShape::readFromDictionary3()
{

	diameters_ = getVal<realVector>("diameters");

	if(diameters_.size() != numShapes() )
	{
		fatalErrorInFunction<< 
		" number of elements in diameters in "<< globalName()<<" is not consistent"<<endl;
		return false;
	}

	return true;
}

bool pFlow::sphereShape::writeToDict(dictionary& dict)const 
{
	
	if(!shape::writeToDict(dict))return false;

	if( !dict.add("diameters", diameters_) )
	{
		fatalErrorInFunction<<
		"  Error in writing diameters to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	return true;
}

pFlow::sphereShape::sphereShape
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

pFlow::sphereShape::sphereShape
(
	const word &shapeType, 
	const word &fileName, 
	repository *owner, 
	const property &prop
)
:
	sphereShape(fileName, owner, prop)
{
}

pFlow::real pFlow::sphereShape::maxBoundingSphere() const
{
    return max(diameters_);
}

pFlow::real pFlow::sphereShape::minBoundingSphere() const
{
    return min(diameters_);
}

bool pFlow::sphereShape::boundingDiameter(uint32 index, real &bDiam) const
{
	if( indexValid(index))
	{
		bDiam = diameters_[index];
		return true;
	}
    return false;
}

pFlow::real pFlow::sphereShape::boundingDiameter(uint32 index) const
{
    if(indexValid(index))
	{
		return diameters_[index];
	}
	fatalErrorInFunction<<"Invalid index for diameter "<<
	index<<endl;
	fatalExit;
	return 0.0;
}

pFlow::realVector pFlow::sphereShape::boundingDiameter() const
{
    return diameters_;
}

pFlow::realVector pFlow::sphereShape::volume() const
{
    return realVector("volume", Pi/6*pow(diameters_,(real)3.0));
}

bool pFlow::sphereShape::mass(uint32 index, real &m) const
{
    if( indexValid(index) )
	{
		real d = diameters_[index];
		real rho = indexToDensity(index);
		m = Pi/6.0*pow(d,3)*rho;
		return true;
	}
	return false;
}

pFlow::real pFlow::sphereShape::mass(uint32 index) const
{
    if(real m; mass(index, m))
	{
		return m;
	}
	fatalErrorInFunction<<"bad index for mass "<< index<<endl;
	fatalExit;
	return 0;
}

pFlow::realVector pFlow::sphereShape::mass() const
{
    return realVector ("mass", Pi/6*pow(diameters_,(real)3.0)*density());
}

pFlow::realVector pFlow::sphereShape::density()const 
{
	auto pids = shapePropertyIds();
	realVector rho("rho", numShapes());
	ForAll(i, pids)
	{
		rho[i] = properties().density(pids[i]);
	}
	return rho;
}

bool pFlow::sphereShape::Inertia(uint32 index, real &I) const
{
	if( indexValid(index) )
	{
		I = 0.4 * mass(index) * pow(diameters_[index]/2.0,2.0);
		return true;
	}
    return false;
}

pFlow::real pFlow::sphereShape::Inertia(uint32 index) const
{
	if(real I; Inertia(index, I))
	{
		return I;
	}
    fatalExit;
	return 0;
}

pFlow::realVector pFlow::sphereShape::Inertia() const
{
    return realVector("I", (real)0.4*mass()*pow((real)0.5*diameters_,(real)2.0));
}

bool pFlow::sphereShape::Inertia_xx(uint32 index, real &Ixx) const
{
    return Inertia(index,Ixx);
}

pFlow::real pFlow::sphereShape::Inertial_xx(uint32 index) const
{
    return Inertia(index);
}

bool pFlow::sphereShape::Inertia_yy(uint32 index, real &Iyy) const
{
    return Inertia(index,Iyy);
}

pFlow::real pFlow::sphereShape::Inertial_yy(uint32 index) const
{
    return Inertia(index);
}

bool pFlow::sphereShape::Inertia_zz(uint32 index, real &Izz) const
{
    return Inertia(index,Izz);
}

pFlow::real pFlow::sphereShape::Inertial_zz(uint32 index) const
{
    return Inertia(index);
}
