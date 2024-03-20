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

#ifndef __geometry_hpp__
#define __geometry_hpp__


#include "virtualConstructor.hpp"
#include "demComponent.hpp"
#include "property.hpp"
#include "multiTriSurface.hpp"
#include "triSurfaceFields.hpp"
//#include "Fields.hpp"
//#include "Vectors.hpp"



namespace pFlow
{


/**
 * Base class for geometry for managing tri-surfaces, geometry motion, 
 * and surface physical properties. 
 * 
 */
class geometry
:
	public multiTriSurface,
	public demComponent
{
private:

	// - Protected members 

		/// Const reference to physical property of materials 
		const property& 	wallProperty_;

		/// The name of motion component of each wall surface 
		wordField_H 		motionComponentName_{
								"motionComponentName",	
								"motionComponentName"};

		/// Material name of each wall surface  
		wordField_H   		materialName_{
								"materialName",
								"materialName"};
		
		/// Property id of each triangle in the set of wall surfaces
		uint32TriSurfaceField_D 		propertyId_;

		/// Contact force on each triangle in the set of wall surfaces
		realx3TriSurfaceField_D 		contactForceWall_;

		/// Stress on each triangle in the set of wall surfaces 
		realx3TriSurfaceField_D 		normalStressWall_;

		/// Stress on each triangle in the set of wall surfaces 
		realx3TriSurfaceField_D 		shearStressWall_;


		bool readWholeObject_ = true; 			

	// - Protected member functions
	
		/// Find property id of each triangle based on the supplied material name
		/// and the surface wall that the triangle belongs to. 
		bool createPropertyId();

		/// Initialize contact force to zero 
		void zeroForce();
		
public:

	/// Type info
	TypeInfo("geometry");

	// - Constructors 
		
		/// Construct from controlSystem and property, for reading from file
		geometry(systemControl& control, const property& prop);

		geometry(systemControl& control, 
			const property& prop,
			multiTriSurface& surf,
			const wordVector& motionCompName,
			const wordVector& materialName,
			const dictionary& motionDict);

		/// Construct from components
		/*geometry(systemControl& control,
				 const property& prop,
				 const multiTriSurface& triSurface,
				 const wordVector& motionCompName,
				 const wordVector& propName
				);

		/// Construct from components and dictionary that contains 
		/// motionModel
		geometry(systemControl& control,
				 const property& prop,
				 const dictionary& dict,
				 const multiTriSurface& triSurface,
				 const wordVector& motionCompName,
				 const wordVector& propName);*/

		/// Destructor
		~geometry()override = default;

		/// Virtual constructor 
		create_vCtor
		(
			geometry,
			systemControl,
			(
				systemControl& control,
				const property& prop
			),
			(control, prop)
		);

		/// Virtual constructor 
		create_vCtor
		(
			geometry,
			dictionary,
			(
				systemControl& control, 
				const property& prop,
				multiTriSurface& surf,
				const wordVector& motionCompName,
				const wordVector& materialName,
				const dictionary& motionDic),
			(control, prop, surf, motionCompName, materialName, motionDic)
		);

	//- Methods 	
		inline 
		const auto& motionComponentName()const
		{
			return motionComponentName_;
		}
		
		/// Access to contact force
		inline
		auto& contactForceWall()
		{
			return contactForceWall_;
		}

		/// Access to contact force
		inline
		const auto& contactForceWall() const
		{
			return contactForceWall_;
		}

		/// Property ide of triangles 
		inline
		const auto& propertyId()const
		{
			return propertyId_;
		}

		/// Access to property 
		inline const auto& wallProperty()const
		{
			return wallProperty_;
		}

		/// The name of motion model 
		virtual 
		word motionModelTypeName()const = 0;

		/// Motion model index of triangles 
		virtual
		const uint32Field_D& triMotionIndex() const =0;

		/// Motion model index of points 
		virtual 
		const uint32Field_D& pointMotionIndex()const = 0;

		
		bool beforeIteration() override;
		
		/// This is called in time loop. Perform the main calculations 
		/// when the component should evolve along time.
		bool iterate() override;
		
		/// This is called in time loop, after iterate.
		bool afterIteration() override;
		
	//- IO

		bool read(iIstream& is, const IOPattern& iop) override;

		/// write 
		bool write( iOstream& os, const IOPattern& iop )const override;
		


	//- Static members 

		static
		uniquePtr<geometry> create(
			systemControl& control, 
			const property& prop);

		static
		uniquePtr<geometry> create(
			systemControl& control, 
			const property& prop,
			multiTriSurface& surf,
			const wordVector& motionCompName,
			const wordVector& materialName,
			const dictionary& motionDic);

};

}

#endif
