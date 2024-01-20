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

#ifndef __partitioning_hpp__
#define __partitioning_hpp__

#include "zoltan_cpp.h"

#include "pFlowProcessors.hpp"
#include "virtualConstructor.hpp"
#include "box.hpp"
#include "span.hpp"
#include "pointFlag.hpp"
#include "procVector.hpp"

namespace pFlow
{

struct pointCollection
{
	span<realx3> points_;
	pFlagTypeHost pFlag_;

	uint32 numActivePoints()const
	{
		return pFlag_.numActive();
	}
};

struct dataCollection
{
    span<char> srcData_;
    span<char> dstData_;
    uint32 elementSize_;
};

class partitioning
{
protected:

	float 					version_ 	= 0.0;

	std::unique_ptr<Zoltan> zoltan_ 	= nullptr;

	bool					validPointers_ = false;

	box 					globalBox_;

	box 					localBox_; 	

	int32 	changes_, numImport_, numExport_;
  	
  	id_t *importGlobalGids_, *importLocalGids_, *exportGlobalGids_, *exportLocalGids_; 
  	
  	int32 *importProcs_, *importToPart_, *exportProcs_, *exportToPart_;

    uint32 numBeforePartition_ = 0 ;

	static inline bool 		zoltanInitialized__ = false;

	void freeZoltan();

	virtual 
	bool partition(pointCollection& points) = 0;

public:

	partitioning(
		const dictionary& dict, 
		const box& globalBox);

	virtual 
	~partitioning();

    create_vCtor(
        partitioning,
        dictionary,
        (
            const dictionary& dict, 
		    const box& globalBox
        ),
        (dict, globalBox));

	bool partition(
		span<realx3> points, 
		pFlagTypeHost flags);

    
    bool migrateData(span<char> src, span<char> dst, uint32 elementSize);

	inline
	auto localBox()const
	{
		return localBox_;
	}

	inline
	const auto& globalBox()const
	{
		return globalBox_;
	}

	inline 
	bool partitionsChanged()const
	{
		return changes_ == 1;
	}

    
    uint32 numberImportThisProc()const
    {
        return numImport_;
    }

    uint32 numberExportThisProc()const
    {
        return numExport_;
    }
    
    virtual
    span<int32> exportList(int procNo)const = 0;

    virtual 
    pFlow::MPI::procVector<span<int32>> allExportLists()const=0;

	void printBox()const;

	
};


}


#endif //__partitioning_hpp__



/*static 
	int getNumberOfPoints(void *data, int32 *ierr);

	static 
	void getPointList(
		void *data, 
		int32 sizeGID, 
		int32 sizeLID,
        id_t* globalID, 
        id_t* localID,
        int32 wgt_dim, 
        float *obj_wgts, 
        int32 *ierr);*/