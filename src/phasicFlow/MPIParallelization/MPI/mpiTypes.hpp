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

#ifndef __mpiTypes_H__
#define __mpiTypes_H__



#include <mpi.h>

namespace pFlow::MPI
{
	// types
	using Comm 			= MPI_Comm;
    using Group         = MPI_Group;
	using Status 		= MPI_Status;
	using Offset 		= MPI_Offset;
	using Request 		= MPI_Request;
	using Operation 	= MPI_Op;
	using Information	= MPI_Info;
	using DataType 		= MPI_Datatype;
	
	inline Comm CommWorld 		= MPI_COMM_WORLD;

	// all nulls

	inline auto ProcNull 		= MPI_PROC_NULL;
	inline auto InfoNull  		= MPI_INFO_NULL;
	inline auto RequestNull		= MPI_REQUEST_NULL;
	inline auto StatusIgnore 	= MPI_STATUS_IGNORE;
	inline auto StatusesIgnore 	= MPI_STATUSES_IGNORE;
	inline auto FileNull 		= MPI_FILE_NULL;
	inline Comm  CommNull 		= MPI_COMM_NULL;
    inline auto TypeNull        = MPI_DATATYPE_NULL;

	// errors
	inline const auto Success 	= MPI_SUCCESS;
	inline const auto ErrOp 	= MPI_ERR_OP;

	inline const auto SumOp		= MPI_SUM;
	inline const auto MaxOp 	= MPI_MAX;
	inline const auto MinOp 	= MPI_MIN;

	inline const size_t MaxNoProcessors = 2048;
	
}







#endif //__mpiTypes_H__
