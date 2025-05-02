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
#ifndef __shape_hpp__
#define __shape_hpp__

#include "baseShapeNames.hpp"
#include "property.hpp"

namespace pFlow
{


class shape
:
	public baseShapeNames
{
private:

	/// property of materials 
  	const property& 	property_; 

	/// list of property ids of shapes (index)
	uint32Vector		shapePropertyIds_;

	/// list of material names 
	wordVector 			materialNames_;
		
	
	bool findPropertyIds();

	bool readFromDictionary2();	

protected:

	
	bool writeToDict(dictionary& dict)const override;

public:
    
    TypeInfo("shape");

	shape(
		const word& fileName,
		repository* owner,
		const property& prop);
	
	shape(
		const word& shapeType,
		const word& fileName,
		repository* owner,
		const property& prop);

    ~shape() override=default;

	create_vCtor
	(
		shape,
		word,
		(
			const word& shapeType,
			const word& fileName,
			repository* owner,
			const property& prop
		),
		(shapeType, fileName, owner, prop)	
	);

	inline 
	const auto& properties()const
	{
		return property_;
	}

    
	inline
    const wordVector& materialNames()const
	{
		return materialNames_;
	}

	inline
	const auto& shapePropertyIds()const
	{
		return shapePropertyIds_;
	}

    
	
    inline
    bool shapeNameToPropId(const word& name, int8& propId)const
	{	
		if(uint32 index; shapeNameToIndex(name, index))
		{
			propId = shapePropertyIds_[index];
			return true;
		}
		return false;
	}

	inline
    bool propIdValid(uint32 propId)const
	{
		return static_cast<uint32>(propId) < property_.numMaterials();
	}


	inline
    bool indexToDensity(uint32 index, real& rho)const
	{
		if(indexValid(index))
			return property_.density(shapePropertyIds_[index], rho);
		return false;
	}

    inline
    real indexToDensity(uint32 index)const
	{
		if(indexValid(index))
		{
			return property_.density(shapePropertyIds_[index]);
		}
		
		fatalExit;
		return 0.0;
	}

	inline 
	uint32 propertyId(uint32 index)const
	{
		if(indexValid(index))
		{
			return shapePropertyIds_[index];
		}
		fatalExit;
		return 0.0;
	}

	virtual 
    real maxBoundingSphere()const = 0;

    virtual 
    real minBoundingSphere()const = 0;

    virtual 
    bool boundingDiameter(uint32 index, real& bDiam)const =0;

    virtual
    real boundingDiameter(uint32 index)const = 0;

	virtual
	realVector boundingDiameter()const = 0;

	virtual 
	realVector volume()const = 0;

    virtual 
    bool mass(uint32 index, real& m)const = 0;

    virtual 
    real mass(uint32 index) const =0;

	virtual
	realVector mass()const =0;

	virtual
	realVector density()const =0;

    virtual 
    bool Inertia(uint32 index, real& I)const = 0;

    virtual
    real Inertia(uint32 index)const = 0;

	virtual 
	realVector Inertia()const=0;

    virtual 
    bool Inertia_xx(uint32 index, real& Ixx)const = 0;

    virtual
    real Inertial_xx(uint32 index)const =0;
	
    virtual 
    bool Inertia_yy(uint32 index, real& Iyy)const = 0;

    virtual 
    real Inertial_yy(uint32 index)const = 0;

    virtual 
    bool Inertia_zz(uint32 index, real& Izz)const = 0;

    virtual 
    real Inertial_zz(uint32 index)const = 0;

	static
	uniquePtr<shape> create(
		const word& shapeType,
		const word& fileName,
		repository* owner,
		const property& prop);

};

}

#endif //__shape_hpp__