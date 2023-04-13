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


/**
 * Base class for geometry for managing tri-surfaces, geometry motion, 
 * and surface physical properties. 
 * 
 */
class geometry
:
	public demGeometry
{
protected:

	// - Protected members 

		/// Const reference to physical property of materials 
		const property& 	wallProperty_;

		/// Repository to store geometry data at each simulation moment
		repository&  		geometryRepository_;

		/// All triangles in the set of wall surfaces 
		multiTriSurface&  	triSurface_;

		/// The name of motion component of each wall surface 
		wordField& 			motionComponentName_;

		/// Material name of each wall surface  
		wordField&   		materialName_;
		
		/// Property id of each triangle in the set of wall surfaces
		int8TriSurfaceField_D& 		propertyId_;

		/// Contact force on each triangle in the set of wall surfaces
		realx3TriSurfaceField_D& 	contactForceWall_;

		/// Stress on ech triangle in the set of wall surfaces 
		realx3TriSurfaceField_D&    stressWall_;

	// - Protected member functions
	
		/// Find property id of each triangle based on the supplied material name
		/// and the surface wall that the triangle belongs to. 
		bool findPropertyId();

		/// Initialize contact force to zero 
		void zeroForce()
		{
			contactForceWall_.fill(zero3);
		}


public:

	/// Type info
	TypeInfo("geometry");

	// - Constructors 
		
		/// Construct from controlSystem and property, for reading from file
		geometry(systemControl& control, const property& prop);

		/// Construct from components
		geometry(systemControl& control,
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
				 const wordVector& propName);

		/// Destructor
		virtual ~geometry() = default;

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
			(systemControl& control,
			 const property& prop,
			 const dictionary& dict,
			 const multiTriSurface& triSurface,
			 const wordVector& motionCompName,
			 const wordVector& propName),
			(control, prop, dict, triSurface, motionCompName, propName)
		);

	//- Methods 

		/// Size of tri-surface 
		inline
		auto size()const
		{
			return triSurface_.size();
		}

		/// Number of points in the set of surface walls 
		inline
		auto numPoints()const
		{
			return triSurface_.numPoints();
		}

		/// Number of triangles in the set of surface walls 
		inline
		auto numTriangles()const
		{
			return size();
		}

		/// Access to the points
		inline
		const auto& points()const
		{
			return triSurface_.points();
		}

		/// Access to the vertices 
		inline
		const auto& vertices()const
		{
			return triSurface_.vertices();
		}

		/// Obtain an object for accessing triangles 
		inline auto getTriangleAccessor()const
		{
			return triSurface_.getTriangleAccessor();
		}

		/// Surface 
		inline auto& surface()
		{
			return triSurface_;
		}

		/// Surface 
		inline const auto& surface()const
		{
			return triSurface_;
		}

		/// Access to contact force
		inline
		realx3TriSurfaceField_D& contactForceWall()
		{
			return contactForceWall_;
		}

		/// Access to contact force
		inline
		const realx3TriSurfaceField_D& contactForceWall() const
		{
			return contactForceWall_;
		}

		/// Access to property 
		inline const auto& wallProperty()const
		{
			return wallProperty_;
		}

		/// Owner repository
		inline
		const repository& owner()const
		{
			return geometryRepository_;
		}

		/// Owner repository
		inline
		repository& owner()
		{
			return geometryRepository_;
		}

		/// Path to the repository folder 
		inline auto path()
		{
			return owner().path();
		}

		/// The name of motion model 
		virtual 
		word motionModelTypeName()const = 0;

		/// Motion model index of triangles 
		virtual
		const int8Vector_HD& 			triMotionIndex() const =0;

		/// Motion model index of points 
		virtual 
		const int8Vector_HD& 			pointMotionIndex()const = 0;

		/// Property ide of triangles 
		const int8TriSurfaceField_D& 	propertyId() const
		{
			return propertyId_;
		}

		/// Operations before each iteration 
		bool beforeIteration() override;

		/// Operations after each iteration
		bool afterIteration() override;

	//- IO

		/// write 
		bool write()const
		{
			return owner().write();
		}


	//- Static members 

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
