#include "geometryMotion.hpp"
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
bool pFlow::geometryMotion<MotionModel>::findMotionIndex()
{
	
	if(motionComponentName().size() != numSurfaces() )
	{
		fatalErrorInFunction<<
		"size of motion component names in the triSurface is not"<<
		" equal to size of number of sub-surfaces"<<endl;
		 return false;
	}

	uint32Vector surfMotionIndex("surfMotionIndex");
	uint32Vector triMotionIndex("triMotionIndex");
	uint32Vector pointMotionIndex("pointMotionIndex");

	ForAll( surfI, motionComponentName())
	{
		auto mName = motionComponentName()[surfI];
		uint32 mInd=0;
		
		if( !motionModel_.nameToIndex(mName, mInd) )
		{
			fatalErrorInFunction<<
			mName<< " does not exist in the list of motion names -> "<<
			motionModel_.componentNames();
		}
		surfMotionIndex.push_back(mInd);
		
		auto surfRange = this->surface().subSurfaceRange(surfI);
		
		for(uint32 i=0; i<surfRange.numElements(); i++)
		{
			triMotionIndex.push_back(mInd);
		}

		auto pointRange = this->surface().subSurfacePointRange(surfI);
		for(uint32 n=0; n<pointRange.numElements(); n++)
		{
			pointMotionIndex.push_back(mInd);
		}
	}
	
	surfMotionIndex_.assign(surfMotionIndex);
	triMotionIndex_.assign(triMotionIndex);
	pointMotionIndex_.assign(pointMotionIndex);

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
	motionModel_
	(
		objectFile
		(
			motionModelFile__,
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS
		),
		owner()
	),
	moveGeomTimer_("move geometry", &this->timers())
{
	if(!findMotionIndex())
	{
		fatalExit;
	}
}

template <typename MotionModelType>
pFlow::geometryMotion<MotionModelType>::geometryMotion
(
	systemControl &control, 
	const property &prop, 
	multiTriSurface &surf, 
	const wordVector &motionCompName, 
	const wordVector &materialName, 
	const dictionary &motionDict
)
:
	geometry
	(
		control, 
		prop, 
		surf, 
		motionCompName, 
		materialName,
        motionDict
	),
	motionModel_
	(
		objectFile
		(
			motionModelFile__,
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_ALWAYS
		),
		motionDict,
		owner()
	),
	moveGeomTimer_("move geometry", &this->timers())
{
	if(!findMotionIndex())
	{
		fatalExit;
	}
} 

/*template<typename MotionModel>
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
 bool pFlow::geometryMotion<MotionModel>::beforeIteration()
 {
     geometry::beforeIteration();
     return true;
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

 template<typename MotionModel>
 bool pFlow::geometryMotion<MotionModel>::afterIteration()
 {
     geometry::afterIteration();
     return true;
 }

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

     // move the motion components
     motionModel_.move(t,dt);

     // end of calculations
     moveGeomTimer_.end();

     return true;
 }*/