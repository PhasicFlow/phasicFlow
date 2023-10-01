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

#ifndef __rcbPartitioning_hpp__
#define __rcbPartitioning_hpp__

#include "box.hpp"
#include "processors.hpp"



namespace pFlow
{

class Zoltan;


class partitioning
{

protected:

	float 					version_ 	= 0.0;

	std::unique_ptr<Zoltan> zoltan_ 	= nullptr;

	bool					validPointers_ = false;

	const box& 				globalBox_;

	box 					localBox_; 	

	int32 	changes_, numImport_, numExport_;
  	
  	id_t* importGlobalGids_, importLocalGids_, exportGlobalGids_, exportLocalGids_; 
  	
  	int32*importProcs_, *importToPart_, *exportProcs_, *exportToPart_;

	static inline bool 		zoltanInitialized__ = false;

	void freeZoltan();

public:

	partitioning(int argc, char *argv[], const box& globalBox);

	virtual 
	~partitioning();

		
	virtual 
	bool partition() = 0;

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

	void printBox()const;

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
};


}


#endif //__rcbPartitioning_hpp__