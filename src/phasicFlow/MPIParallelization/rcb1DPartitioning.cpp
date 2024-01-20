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

#include "zoltan_cpp.h"


#include "error.hpp"
#include "processors.hpp"
#include "rcb1DPartitioning.hpp"

bool pFlow::rcb1DPartitioning::partition(pointCollection &points)
{
    
    zoltan_->Set_Param("RCB_OUTPUT_LEVEL", "0");
  	zoltan_->Set_Param("RCB_RECTILINEAR_BLOCKS", "1");
  	zoltan_->Set_Param("KEEP_CUTS", "1"); 
  	zoltan_->Set_Param("REDUCE_DIMENSIONS", "1");
  	zoltan_->Set_Param("RCB_RECOMPUTE_BOX", "1");
  	zoltan_->Set_Param("AVERAGE_CUTS", "0");
    zoltan_->Set_Param("MIGRATE_ONLY_PROC_CHANGES", "0");

  	zoltan_->Set_Num_Obj_Fn(rcb1DPartitioning::getNumberOfPoints, &points);
  	zoltan_->Set_Obj_List_Fn(rcb1DPartitioning::getPointList, &points);
  	zoltan_->Set_Num_Geom_Fn(rcb1DPartitioning::getNumGeometry, &points);
  	switch (direction_)
    {
    case Direction::X:
        zoltan_->Set_Geom_Multi_Fn(rcb1DPartitioning::getGeometryList_x, &points);
        break;
    case Direction::Y:
        zoltan_->Set_Geom_Multi_Fn(rcb1DPartitioning::getGeometryList_y, &points);
        break;
    case Direction::Z:
        zoltan_->Set_Geom_Multi_Fn(rcb1DPartitioning::getGeometryList_z, &points);
        break;
    }
    
	int numGidEntries_, numLidEntries_;
	int rc = zoltan_->LB_Partition(changes_, numGidEntries_, numLidEntries_,
    	numImport_, importGlobalGids_, importLocalGids_, importProcs_, importToPart_,
    	numExport_, exportGlobalGids_, exportLocalGids_, exportProcs_, exportToPart_);

  	if (rc != ZOLTAN_OK)
  	{	
        fatalErrorInFunction<< "Zoltan faild to perform partitioning."<<endl;
    	return false;
  	}
    
    for(auto& ids:exportIds_)
    {
        ids.clear();
    }
    
    std::vector<int32> thisProc(points.numActivePoints(),0);
    
        
    for(auto i =0; i<numExport_; i++)
    {
        exportIds_[exportProcs_[i]].push_back(exportGlobalGids_[i]);
        thisProc[exportGlobalGids_[i]] = exportGlobalGids_[i];
    }

    for(int i=0; i<thisProc.size(); i++)
    {
        if(thisProc[i]==0)
            exportIds_[0].push_back(i);
    }

  	validPointers_ = true;

  	int nDim;
  	double x0, y0, z0, x1, y1,z1;
  	zoltan_->RCB_Box
    (
        processors::globalRank(), 
        nDim,
   		x0, y0, z0,
   		x1, y1, z1
    );

  	localBox_ = globalBox_;

    switch (direction_)
    {
    case Direction::X :
        localBox_.minPoint().x_ = x0;
  	    localBox_.maxPoint().x_ = x1;
        break;

    case Direction::Y :
        localBox_.minPoint().y_ = x0;
  	    localBox_.maxPoint().y_ = x1;
        break;
    
    case Direction::Z :
        localBox_.minPoint().z_ = x0;
  	    localBox_.maxPoint().z_ = x1;
        break;
    }
  	

   	localBox_.minPoint() = max(localBox_.minPoint(), globalBox_.minPoint());
   	localBox_.maxPoint() = min(localBox_.maxPoint(), globalBox_.maxPoint());


  	return true;
}

pFlow::rcb1DPartitioning::rcb1DPartitioning
(
    const dictionary &dict,
    const box &globalBox
)
: 
    partitioning(dict, globalBox),
    exportIds_(pFlowProcessors())
{

    word directionName = dict.getVal<word>("direction");

	if(toUpper(directionName)== "X")
    {
        direction_ = Direction::X;
        dirVector_ ={1.0, 0.0, 0.0};
    }
    else if( toUpper(directionName) == "Y")
    {
        direction_ = Direction::Y;
        dirVector_ ={0.0, 1.0, 0.0};
    }
    else if( toUpper(directionName) == "Z")
    {
        direction_ = Direction::Z;
        dirVector_ ={0.0, 0.0, 1.0};
    }
    else
    {
        fatalErrorInFunction<< "wrong direction  in dictionary "<<
        dict.globalName()<<". Directions should be one of x, y, or z."<<endl;
        fatalError;
    }       
			
}

int pFlow::rcb1DPartitioning::getNumGeometry(void *data, int *ierr)
{
  *ierr = ZOLTAN_OK;
  return 1;
}

int pFlow::rcb1DPartitioning::getNumberOfPoints(void *data, int *ierr)
{
    auto *obj = static_cast<pointCollection *>(data);

    *ierr = ZOLTAN_OK;

    return obj->numActivePoints();
}

void pFlow::rcb1DPartitioning::getPointList
(
    void *data, 
    int sizeGID, 
    int sizeLID, 
    ZOLTAN_ID_PTR globalID, 
    ZOLTAN_ID_PTR localID, 
    int wgt_dim, 
    float *obj_wgts, 
    int *ierr
)
{			
    auto* obj = static_cast<pointCollection *>(data);
    *ierr = ZOLTAN_OK;
    
    auto activeRange = obj->pFlag_.activeRange();
    uint32 n = 0;
    for (auto i=activeRange.start(); i<activeRange.end(); i++)
    {
        if( obj->pFlag_.isActive(i) )
        {
            globalID[n] = i;
            localID[n] = n;
            n++;
        }
    }

}

void pFlow::rcb1DPartitioning::getGeometryList_x
(
	void *data, 
	int sizeGID, 
	int sizeLID,
	int num_obj,
    ZOLTAN_ID_PTR globalID, 
    ZOLTAN_ID_PTR localID,
    int num_dim, 
    double *geom_vec, 
    int *ierr
)
{

	auto* obj = static_cast<pointCollection *>(data);

  	if ( (sizeGID != 1) || (sizeLID != 1) || (num_dim != 1))
  	{
    	*ierr = ZOLTAN_FATAL;
    	return;
  	}

    auto activeRange = obj->pFlag_.activeRange();
    uint32 n = 0;
    for (auto i=activeRange.start(); i<activeRange.end(); i++)
    {
        if( obj->pFlag_.isActive(i) )
        {
            geom_vec[n] 	  = obj->points_[i].x_;
            n++;
        }
    }

  	*ierr = ZOLTAN_OK;

	return;
}

void pFlow::rcb1DPartitioning::getGeometryList_y
(
	void *data, 
	int sizeGID, 
	int sizeLID,
	int num_obj,
    ZOLTAN_ID_PTR globalID, 
    ZOLTAN_ID_PTR localID,
    int num_dim, 
    double *geom_vec, 
    int *ierr
)
{

	auto* obj = static_cast<pointCollection *>(data);

  	if ( (sizeGID != 1) || (sizeLID != 1) || (num_dim != 1))
  	{
    	*ierr = ZOLTAN_FATAL;
    	return;
  	}

    auto activeRange = obj->pFlag_.activeRange();
    uint32 n = 0;
    for (auto i=activeRange.start(); i<activeRange.end(); i++)
    {
        if( obj->pFlag_.isActive(i) )
        {
            geom_vec[n] 	  = obj->points_[i].y_;
            n++;
        }
    }

  	*ierr = ZOLTAN_OK;

	return;
}

void pFlow::rcb1DPartitioning::getGeometryList_z
(
	void *data, 
	int sizeGID, 
	int sizeLID,
	int num_obj,
    ZOLTAN_ID_PTR globalID, 
    ZOLTAN_ID_PTR localID,
    int num_dim, 
    double *geom_vec, 
    int *ierr
)
{

	auto* obj = static_cast<pointCollection *>(data);

  	if ( (sizeGID != 1) || (sizeLID != 1) || (num_dim != 1))
  	{
    	*ierr = ZOLTAN_FATAL;
    	return;
  	}

    auto activeRange = obj->pFlag_.activeRange();
    uint32 n = 0;
    for (auto i=activeRange.start(); i<activeRange.end(); i++)
    {
        if( obj->pFlag_.isActive(i) )
        {
            geom_vec[n] 	  = obj->points_[i].z_;
            n++;
        }
    }

  	*ierr = ZOLTAN_OK;

	return;
}

