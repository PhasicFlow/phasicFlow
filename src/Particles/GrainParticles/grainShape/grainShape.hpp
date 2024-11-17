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

#ifndef __grainShape_hpp__
#define __grainShape_hpp__

#include "shape.hpp"

namespace pFlow
{

class grainShape
:
	public shape
{
private:
	
	// - diameter of spheres
	realVector 			grainDiameters_;
	realVector 			sphereDiameters_;
	realVector 			coarseGrainFactor_;

	
	bool readFromDictionary3();

protected:

	bool writeToDict(dictionary& dict)const override;

public:

	// - type info
	TypeInfo("shape<grain>");

	grainShape(
		const word& fileName,
		repository* owner,
		const property& prop);

	
	~grainShape() override = default;

	//// - Methods

	real maxBoundingSphere()const override;

    real minBoundingSphere()const override;

	bool boundingDiameter(uint32 index, real& bDiam)const override;

	real boundingDiameter(uint32 index)const override;

	realVector boundingDiameter()const override;

		real coarseGrainFactor(uint32 index)const ;

	realVector coarseGrainFactor()const ;

	real orginalDiameter(uint32 index)const ;

	realVector orginalDiameter()const ;

	bool mass(uint32 index, real& m)const override;

	real mass(uint32 index) const override;

	realVector mass()const override;

    realVector density() const override;

	bool Inertia(uint32 index, real& I)const override;

	real Inertia(uint32 index)const override;

	realVector Inertia()const override;
	 
    bool Inertia_xx(uint32 index, real& Ixx)const override;

    real Inertial_xx(uint32 index)const override;
	
    bool Inertia_yy(uint32 index, real& Iyy)const override;

    real Inertial_yy(uint32 index)const override;

    bool Inertia_zz(uint32 index, real& Izz)const override;

    real Inertial_zz(uint32 index)const override;

};

} // pFlow

#endif //__grainShape_hpp__
