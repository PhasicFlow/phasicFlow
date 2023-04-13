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
#ifndef __geometryMotion_hpp__
#define __geometryMotion_hpp__


#include "geometry.hpp"
#include "VectorDuals.hpp"

namespace pFlow
{

/**
 * 	A class that represent surfaces in the simulation and moves surfaces 
 * based on the MotionModelType. MotionModelType can be any motion model. 
 * 
 */
template<typename MotionModelType>
class	geometryMotion 
:
	public geometry
{
public:
	
	using MotionModel = MotionModelType;

protected: 
	
	/// Ref to motion model 
	MotionModel& 		motionModel_;
	
	/// motion indext mapped on each surface 
	int32Vector_HD  	motionIndex_;

	/// motion index mapped on each triangle 
	int8Vector_HD 		triMotionIndex_; 

	/// motion index mapped on each point 
	int8Vector_HD		pointMotionIndex_;

	/// timer for moveGeometry
	Timer 				moveGeomTimer_;			

	/// determine the motion index of each triangle 
	bool findMotionIndex();
	
public:

	/// Type info
	TypeInfoTemplate("geometry", MotionModel);

	// - Constructors
		
		geometryMotion(systemControl& control, const property& prop);

		
		geometryMotion(
			systemControl& control,
			const property& prop,
			const multiTriSurface& triSurface,
			const wordVector& motionCompName,
			const wordVector& propName,
			const MotionModel& motionModel);

		/// Construct from components and dictionary that contains 
		/// motionModel
		geometryMotion(systemControl& control,
				 const property& prop,
				 const dictionary& dict,
				 const multiTriSurface& triSurface,
				 const wordVector& motionCompName,
				 const wordVector& propName);

		/// Add virtual constructor 
		add_vCtor
		(
			geometry,
			geometryMotion,
			systemControl
		);

		/// Add virtual constructor 
		add_vCtor
		(
			geometry,
			geometryMotion,
			dictionary
		);

	// - Methods

		/// Obtain motion model at time t 
		auto getModel(real t)const
		{
			return motionModel_.getModel(t);
		}

		/// TypeName / TypeInfo of motion model 
		word motionModelTypeName()const override
		{
			return motionModel_.typeName();
		}

		/// Access to motion model index of triangles 
		const int8Vector_HD& triMotionIndex()const override
		{
			return triMotionIndex_;
		}

		/// Access to motion model index of points 
		const int8Vector_HD& pointMotionIndex()const override
		{
			return pointMotionIndex_;
		}

		/// Operations before each iteration 
		bool beforeIteration() override;

		/// Iterate geometry one time step  
		bool iterate() override ;

		/// Operations after each iteration 
		bool afterIteration() override;

		/// Move geometry 
		bool moveGeometry();

};

} // pFlow

#include "geometryMotion.cpp"

#ifndef BUILD_SHARED_LIBS
	#include "geometryMotionsInstantiate.cpp"
#endif


#endif  //__geometryMotion_hpp__
