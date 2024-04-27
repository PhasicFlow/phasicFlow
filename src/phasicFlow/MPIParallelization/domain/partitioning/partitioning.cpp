

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


pFlow::partitioning::partitioning
(
    const dictionary& dict, 
    const box& globalBox
)
:
	globalBox_(globalBox)
{
	if(!zoltanInitialized__)
	{
		auto rc = Zoltan_Initialize
        (
            processors::argc(), 
            processors::argv(), 
            &version_
        );
        
		if (rc != ZOLTAN_OK)
		{
			fatalErrorInFunction<<"Cannot initialize zoltan"<<endl;
			fatalExit;
		}
		zoltanInitialized__ = true;
	}

	// Creates Zoltan object 
	zoltan_ = std::make_unique<Zoltan>(pFlowProcessors().localCommunicator());

	zoltan_->Set_Param("DEBUG_LEVEL", "0");
  	zoltan_->Set_Param("LB_METHOD", "RCB");
  	zoltan_->Set_Param("NUM_GID_ENTRIES", "1"); 
  	zoltan_->Set_Param("NUM_LID_ENTRIES", "1");
  	zoltan_->Set_Param("OBJ_WEIGHT_DIM", "0");
    zoltan_->Set_Param("RETURN_LISTS", "ALL");
	
}

bool pFlow::partitioning::partition(span<realx3> points, pFlagTypeHost flags)
{
    pointCollection pointCollctn{points, flags};

    return partition(pointCollctn);
}
int GetObjectSize
(
    void *data,
    int num_gid_entries, 
    int num_lid_entries,
    ZOLTAN_ID_PTR global_id,
    ZOLTAN_ID_PTR local_id,
    int *ierr
)
{
    *ierr = ZOLTAN_OK;
    pFlow::uint32 s = *(static_cast<pFlow::uint32*>(data));
    return static_cast<int>(s);
}

void PackObject 
(
    void *data,
    int num_gid_entries,
    int num_lid_entries,
    ZOLTAN_ID_PTR global_id,
    ZOLTAN_ID_PTR local_id,
    int dest,
    int size,
    char *buf,
    int *ierr
)
{
    
}

bool pFlow::partitioning::migrateData(span<char> src, span<char> dst, uint32 elementSize)
{
    dataCollection data{src, dst, elementSize};

    zoltan_->Set_Obj_Size_Fn(GetObjectSize, &elementSize);
    return false;
}

pFlow::partitioning::~partitioning()
{
	freeZoltan();
}

void pFlow::partitioning::printBox()const
{
	pOutput<< "localBox:" << localBox_<<endl;
}
