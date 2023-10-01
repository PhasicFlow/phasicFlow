
#include "zoltan_cpp.h"

#include "partitioning.hpp"
#include "error.hpp"
#include "streams.hpp"

void pFlow::partitioning::freeZoltan()
{
	if(validPointers_)
	{
		Zoltan::LB_Free_Part(&importGlobalGids_, &importLocalGids_, 
                      	 	 &importProcs_, &importToPart_);

		Zoltan::LB_Free_Part(&exportGlobalGids_, &exportLocalGids_, 
                      		 &exportProcs_, &exportToPart_);
        validPointers_ = false;
	}
	
	zoltan_.release();
}


pFlow::partitioning::partitioning(int argc, char *argv[], const box& globalBox)
:
	globalBox_(globalBox)
{
	if(!zoltanInitialized__)
	{
		auto rc = Zoltan_Initialize(argc, argv, &version_);
		if (rc != ZOLTAN_OK)
		{
			fatalErrorInFunction<<"Cannot initialize zoltan"<<endl;
			fatalExit;
		}
		zoltanInitialized__ = true;
	}

	// Creates Zoltan object 
	zoltan_ = std::make_unique<Zoltan>(MPI::COMM_WORLD);

	zoltan_->Set_Param("DEBUG_LEVEL", "0");
  	zoltan_->Set_Param("LB_METHOD", "RCB");
  	zoltan_->Set_Param("NUM_GID_ENTRIES", "1"); 
  	zoltan_->Set_Param("NUM_LID_ENTRIES", "1");
  	zoltan_->Set_Param("OBJ_WEIGHT_DIM", "0");
    zoltan_->Set_Param("RETURN_LISTS", "ALL");
	
}

pFlow::partitioning::~partitioning()
{
	freeZoltan();
}

void pFlow::partitioning::printBox()const
{
	pOutput<< "localBox:" << localBox_<<endl;
}


 
/*int partitioning::getNumberOfPoints(void *data, int *ierr)
{
	auto* obj = static_cast<pointCollection *>(data);

	*ierr = ZOLTAN_OK;

	return obj->numberOfPOints();
}


void partitioning::getPointList(
	void *data, 
	int sizeGID, 
	int sizeLID,
    ZOLTAN_ID_PTR globalID, 
    ZOLTAN_ID_PTR localID,
    int wgt_dim, 
    float *obj_wgts, 
    int *ierr)
{
		
	auto* obj = static_cast<pointCollection *>(data);
	*ierr = ZOLTAN_OK;

	for (int i=0; i< obj->numberOfPOints(); i++)
	{
		globalID[i] = obj->globalIDList()[i];
		localID[i] = i;
	}
}*/