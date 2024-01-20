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
#ifndef __rcb1DPartitioning_hpp__
#define __rcb1DPartitioning_hpp__

#include "partitioning.hpp"
#include "procVector.hpp"

namespace pFlow
{


class rcb1DPartitioning
:
public partitioning
{
public:

	enum Direction  
	{
		X = 0,
		Y = 1,
		Z = 2
	};

protected:

	/// Direction of partitioning
	Direction   direction_ = Direction::X;

	realx3 		dirVector_ = {1.0, 0.0, 0.0};

    word        directionName_ = "x";

    MPI::procVector<std::vector<int>> exportIds_;
	
	bool partition(pointCollection& points) override;

public:

	
	rcb1DPartitioning(
		const dictionary& dict, 
		const box& globalBox);
	
	
	virtual 
	~rcb1DPartitioning()=default;

    span<int32> exportList(int procNo)const override
    {
        return span<int32>(
            const_cast<int32*>(exportIds_[procNo].data()), 
            exportIds_[procNo].size());
    }

    
    pFlow::MPI::procVector<span<int32>> allExportLists()const override
    {
        pFlow::MPI::procVector<span<int32>> allList(pFlowProcessors());

        for(int i=0; i<allList.size(); i++)
            allList[i]= exportList(i);

        return allList;
    }

	static 
	int getNumGeometry(void *data, int *ierr);

	static
	int getNumberOfPoints(void *data, int *ierr);
	

	static
	void getPointList
	(
		void *data, 
		int sizeGID, 
		int sizeLID,
		ZOLTAN_ID_PTR globalID, 
		ZOLTAN_ID_PTR localID,
		int wgt_dim, 
		float *obj_wgts, 
		int *ierr
	);

    static
    void getGeometryList_x(
        void *data, 
        int sizeGID, 
        int sizeLID, 
        int num_obj, 
        ZOLTAN_ID_PTR globalID, 
        ZOLTAN_ID_PTR localID, 
        int num_dim, 
        double *geom_vec, 
        int *ierr);

    static
    void getGeometryList_y(
        void *data, 
        int sizeGID, 
        int sizeLID, 
        int num_obj, 
        ZOLTAN_ID_PTR globalID, 
        ZOLTAN_ID_PTR localID, 
        int num_dim, 
        double *geom_vec, 
        int *ierr);

    static
    void getGeometryList_z(
        void *data, 
        int sizeGID, 
        int sizeLID, 
        int num_obj, 
        ZOLTAN_ID_PTR globalID, 
        ZOLTAN_ID_PTR localID, 
        int num_dim, 
        double *geom_vec, 
        int *ierr);
};

/*class RCB_y_partitioning
:
public partitioning
{
public:

	
	RCB_y_partitioning(int argc, char *argv[], pointCollection& collection, const box& gBox)
	:
	partitioning(argc, argv, collection, gBox)
	{}

	virtual 
	~RCB_y_partitioning()=default;

	
	bool partition() override;


	static 
	void getGeometryList(
		void *data, 
		int sizeGID, 
		int sizeLID,
		int num_obj,
        ZOLTAN_ID_PTR globalID, 
        ZOLTAN_ID_PTR localID,
        int num_dim, 
        double *geom_vec, 
        int *ierr)
	{

		auto* obj = static_cast<pointCollection *>(data);

	  	if ( (sizeGID != 1) || (sizeLID != 1) || (num_dim != 1))
	  	{
	    	*ierr = ZOLTAN_FATAL;
	    	return;
	  	}

	  	*ierr = ZOLTAN_OK;

	  	for (int i=0;  i < num_obj ; i++)
	  	{
	    	geom_vec[i] 	  = obj->pointList()[i].y_;
		}

	  return;
	}

	
	static 
	int getNumGeometry(void *data, int *ierr)
	{
	  *ierr = ZOLTAN_OK;
	  return 1;
	}

};


class RCB_x_partitioning
:
public partitioning
{
public:

	
	RCB_x_partitioning(int argc, char *argv[], pointCollection& collection, const box& gBox)
	:
	partitioning(argc, argv, collection, gBox)
	{}

	virtual 
	~RCB_x_partitioning()=default;

	
	bool partition() override;


	static 
	void getGeometryList(
		void *data, 
		int sizeGID, 
		int sizeLID,
		int num_obj,
        ZOLTAN_ID_PTR globalID, 
        ZOLTAN_ID_PTR localID,
        int num_dim, 
        double *geom_vec, 
        int *ierr);
	
	static 
	int getNumGeometry(void *data, int *ierr);
	

};*/

} // pFlow
#endif //__rcb1DPartitioning_hpp__