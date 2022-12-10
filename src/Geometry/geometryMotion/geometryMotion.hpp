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


template<typename MotionModelType>
class	geometryMotion 
:
	public geometry
{
public:
	
	using MotionModel = MotionModelType;

protected: 
	

	MotionModel& 		motionModel_;
	
	// motion indext mapped on each surface 
	int32Vector_HD  	motionIndex_;

	// motion index mapped on each triangle 
	int8Vector_HD 		triMotionIndex_; 

	/// motion index mapped on each point 
	int8Vector_HD		pointMotionIndex_;

	// timer for moveGeometry
	Timer 				moveGeomTimer_;			

	
	bool findMotionIndex();
	
public:

	// type info
	TypeInfoTemplate("geometry", MotionModel);

	//// - Constructors
		
		geometryMotion(systemControl& control, const property& prop);

		// construct from components 
		geometryMotion(
			systemControl& control,
			const property& prop,
			const multiTriSurface& triSurface,
			const wordVector& motionCompName,
			const wordVector& propName,
			const MotionModel& motionModel);

		// - construct from components and dictionary that contains 
		//   motionModel
		geometryMotion(systemControl& control,
				 const property& prop,
				 const dictionary& dict,
				 const multiTriSurface& triSurface,
				 const wordVector& motionCompName,
				 const wordVector& propName);
		


		add_vCtor
		(
			geometry,
			geometryMotion,
			systemControl
		);

		add_vCtor
		(
			geometry,
			geometryMotion,
			dictionary
		);

	//// - Methods

	auto getModel()const
	{
		return motionModel_.getModel();
	}

	word motionModelTypeName()const override
	{
		return motionModel_.typeName();
	}

	const int8Vector_HD& triMotionIndex()const override
	{
		return triMotionIndex_;
	}

	const int8Vector_HD& pointMotionIndex()const override
	{
		return pointMotionIndex_;
	}

	// - iterate
	bool beforeIteration() override { 
		geometry::beforeIteration();
		return true;
	}

	bool iterate() override ;

	bool afterIteration() override {
		geometry::afterIteration();
		return true;
	}


	bool moveGeometry();

};


}

#include "geometryMotion.cpp"

#ifndef BUILD_SHARED_LIBS
	#include "geometryMotionsInstantiate.cpp"
#endif


#endif  //__geometryMotion_hpp__
