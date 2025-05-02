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
            return false;
		}
		surfMotionIndex.push_back(mInd);
		
		auto surfRange = subSurfaceRange(surfI);
		
		for(uint32 i=0; i<surfRange.numElements(); i++)
		{
			triMotionIndex.push_back(mInd);
		}

		auto pointRange = subSurfacePointRange(surfI);
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

namespace pFlow::GMotion
{
    template<typename ModelInterface>
    void moveGeometry(
        real dt,
        uint32 nPoints,
        const ModelInterface& mModel,
        const deviceViewType1D<uint32>& pointMIndexD,
        const deviceViewType1D<realx3>& pointsD
    )
    {
        Kokkos::parallel_for(
         "geometryMotion<MotionModel>::movePoints",
         deviceRPolicyStatic(0, nPoints),
         LAMBDA_HD(uint32 i){
             auto newPos = mModel.transferPoint(pointMIndexD[i], pointsD[i], dt);
             pointsD[i] = newPos;
         });

        Kokkos::fence();
    }
}

template<typename MotionModel>
 bool pFlow::geometryMotion<MotionModel>::moveGeometry()
 {

    uint32 iter = this->currentIter();
    real t = this->currentTime();
    real dt = this->dt();
        
    auto mModel = motionModel_.getModelInterface(iter, t, dt);
    
    auto& pointMIndexD= pointMotionIndex_.deviceViewAll();
    auto& pointsD = points().deviceViewAll();
    
    pFlow::GMotion::moveGeometry(
        dt, 
        numPoints(), 
        motionModel_.getModelInterface(iter, t, dt),
        pointMotionIndex_.deviceViewAll(),
        points().deviceViewAll()
    );

    /*Kokkos::parallel_for(
         "geometryMotion<MotionModel>::movePoints",
         deviceRPolicyStatic(0, numPoints()),
         LAMBDA_HD(uint32 i){
             auto newPos = mModel.transferPoint(pointMIndexD[i], pointsD[i], dt);
             pointsD[i] = newPos;
         });

    Kokkos::fence();*/

    // move the motion components
    motionModel_.move(iter, t,dt);

    // end of calculations
     

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


 template<typename MotionModel>
 bool pFlow::geometryMotion<MotionModel>::iterate()
 {
     if( motionModel_.isMoving() )
     {
        moveGeomTimer_.start();
          moveGeometry();
          this->calculateNormals();
        moveGeomTimer_.end();
     }
     return true;
 }
