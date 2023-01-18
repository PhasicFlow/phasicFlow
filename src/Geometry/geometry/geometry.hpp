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
#include "demGeometry.hpp"
#include "property.hpp"
#include "Fields.hpp"
#include "Vectors.hpp"
#include "multiTriSurface.hpp"
#include "triSurfaceFields.hpp"
#include "dictionary.hpp"

namespace pFlow
{

class geometry
:
	public demGeometry
{
protected:

	const property& 	wallProperty_;

	// - this object is owned by geometryRepository_ 
	repository&  		geometryRepository_;

	// all triangles of walls 
	multiTriSurface&  	triSurface_;

	// 
	wordField& 			motionComponentName_;

	// 
	wordField&   		materialName_;
		
	int8TriSurfaceField_D& 		propertyId_;

	realx3TriSurfaceField_D& 	contactForceWall_;

	realx3TriSurfaceField_D&    stressWall_;

	bool findPropertyId();

	void zeroForce()
	{
		contactForceWall_.fill(zero3);
	}


public:

	// - type info
	TypeInfo("geometry");

	//// - Constructors 
		
		// - empty
		geometry(systemControl& control, const property& prop);

		// - from components
		geometry(systemControl& control,
				 const property& prop,
				 const multiTriSurface& triSurface,
				 const wordVector& motionCompName,
				 const wordVector& propName
				);

		geometry(systemControl& control,
				 const property& prop,
				 const dictionary& dict,
				 const multiTriSurface& triSurface,
				 const wordVector& motionCompName,
				 const wordVector& propName);


		virtual ~geometry() = default;

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

		create_vCtor
		(
			geometry,
			dictionary,
			(systemControl& control,
			 const property& prop,
			 const dictionary& dict,
			 const multiTriSurface& triSurface,
			 const wordVector& motionCompName,
			 const wordVector& propName),
			(control, prop, dict, triSurface, motionCompName, propName)
		);

	////- Methods 

		inline
		auto size()const
		{
			return triSurface_.size();
		}

		inline
		auto numPoints()const
		{
			return triSurface_.numPoints();
		}

		inline
		auto numTriangles()const
		{
			return size();
		}

		inline
		const auto& points()const
		{
			return triSurface_.points();
		}

		inline
		const auto& vertices()const
		{
			return triSurface_.vertices();
		}

		inline auto getTriangleAccessor()const
		{
			return triSurface_.getTriangleAccessor();
		}

		inline auto& surface()
		{
			return triSurface_;
		}

		inline const auto& surface()const
		{
			return triSurface_;
		}

		inline
		realx3TriSurfaceField_D& contactForceWall()
		{
			return contactForceWall_;
		}

		inline
		const realx3TriSurfaceField_D& contactForceWall() const
		{
			return contactForceWall_;
		}

		inline const auto& wallProperty()const
		{
			return wallProperty_;
		}

		// owner repository
		inline
		const repository& owner()const
		{
			return geometryRepository_;
		}

		inline
		repository& owner()
		{
			return geometryRepository_;
		}

		inline auto path()
		{
			return owner().path();
		}

		virtual 
		word motionModelTypeName()const = 0;

		virtual
		const int8Vector_HD& 			triMotionIndex() const =0;

		virtual 
		const int8Vector_HD& 			pointMotionIndex()const = 0;

		const int8TriSurfaceField_D& 	propertyId() const
		{
			return propertyId_;
		}

		bool beforeIteration() override { 
			
			this->zeroForce();
			return true;
			
		}

		bool afterIteration() override { 
			
			auto Force = contactForceWall_.deviceVectorAll();
			auto area = triSurface_.area().deviceVectorAll();
			auto stress = stressWall_.deviceVectorAll();
			auto numTri =triSurface_.size();
			

			Kokkos::parallel_for(
				"geometry::calculateStress",
				numTri,
				LAMBDA_HD(int32 i){
					stress[i] = Force[i]/area[i];
				});
			Kokkos::fence();
			return true;
			
		}

		bool write()const
		{
			return owner().write();
		}


		// static 

		static
			uniquePtr<geometry> create(systemControl& control, const property& prop);

		static
			uniquePtr<geometry> create(
				systemControl& control,
				const property& prop,
				const dictionary& dict,
				const multiTriSurface& triSurface,
				const wordVector& motionCompName,
				const wordVector& propName);

};

}

#endif
