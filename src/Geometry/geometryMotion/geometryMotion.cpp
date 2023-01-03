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

template<typename MotionModel>
bool pFlow::geometryMotion<MotionModel>::moveGeometry()
{

	real dt = this->dt();
	real t = this->currentTime();

	auto pointMIndex= pointMotionIndex_.deviceVector();
	auto mModel = motionModel_.getModel(t);
	realx3* points = triSurface_.pointsData_D();
	auto numPoints = triSurface_.numPoints();


	Kokkos::parallel_for(
		"geometryMotion<MotionModel>::movePoints",
		numPoints,
		LAMBDA_HD(int32 i){
			auto newPos = mModel.transferPoint(pointMIndex[i], points[i], dt);
			points[i] = newPos;
		});

	Kokkos::fence();

	// end of calculations 
	moveGeomTimer_.end();

	return true;
}

template<typename MotionModel>
bool pFlow::geometryMotion<MotionModel>::findMotionIndex()
{
	motionIndex_.clear();
	triMotionIndex_.reserve( this->surface().capacity() );
	triMotionIndex_.clear();

	ForAll( surfI, motionComponentName_)
	{
		auto mName = motionComponentName_[surfI];
		auto mInd = motionModel_.nameToIndex(mName);
		motionIndex_.push_back(mInd);
		// fill motionIndex for triangles of the surface
		int32 surfSize = this->surface().surfNumTriangles(surfI);
		for(int32 i=0; i<surfSize; i++)
		{
			triMotionIndex_.push_back(mInd);
		}
	}
	motionIndex_.syncViews();
	triMotionIndex_.syncViews();

	pointMotionIndex_.reserve(triSurface_.numPoints());
	pointMotionIndex_.clear();

	ForAll(surfI, motionIndex_)
	{
		auto nP = triSurface_.surfNumPoints(surfI);
		for(int32 i=0; i<nP; i++)
		{
			pointMotionIndex_.push_back(motionIndex_[surfI]);
		}
	}
	pointMotionIndex_.syncViews();

	return true;
}

template<typename MotionModel>
pFlow::geometryMotion<MotionModel>::geometryMotion
(
	systemControl& control,
	const property& prop
)
:
	geometry(control, prop),
	motionModel_(
		this->owner().template emplaceObject<MotionModel>(
			objectFile(
				motionModelFile__,
				"",
				objectFile::READ_ALWAYS,
				objectFile::WRITE_ALWAYS
				)
			)
		),
	moveGeomTimer_("move geometry", &this->timers())
{
	findMotionIndex();
}

template<typename MotionModel>
pFlow::geometryMotion<MotionModel>::geometryMotion
(
	systemControl& control,
	const property& prop,
	const multiTriSurface& triSurface,
	const wordVector& motionCompName,
	const wordVector& propName,
	const MotionModel& motionModel
)
:
	geometry(
		control,
		prop,
		triSurface,
		motionCompName,
		propName
		),
	motionModel_(
		this->owner().template emplaceObject<MotionModel>(
			objectFile(
				motionModelFile__,
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_ALWAYS
				),
			motionModel
			)
		),
	moveGeomTimer_("move geometry", &this->timers())
{
	findMotionIndex();
}

template<typename MotionModel>
pFlow::geometryMotion<MotionModel>::geometryMotion
(
	systemControl& control,
	const property& prop,
	const dictionary& dict,
	const multiTriSurface& triSurface,
	const wordVector& motionCompName,
	const wordVector& propName
)
:
	geometry(
		control,
		prop,
		dict,
		triSurface,
		motionCompName,
		propName
		),
	motionModel_(
		this->owner().template emplaceObject<MotionModel>(
			objectFile(
				motionModelFile__,
				"",
				objectFile::READ_NEVER,
				objectFile::WRITE_ALWAYS
				),
			dict
			)
		),
	moveGeomTimer_("move geometry", &this->timers())
{
	findMotionIndex();
}

template<typename MotionModel>
bool pFlow::geometryMotion<MotionModel>::iterate()
{
	if( motionModel_.isMoving() )
	{
		moveGeomTimer_.start();
		moveGeometry();
		moveGeomTimer_.end();
	}
	return true;
}