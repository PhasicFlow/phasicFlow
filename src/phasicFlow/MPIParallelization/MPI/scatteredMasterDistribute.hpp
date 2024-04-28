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

#ifndef __scatteredMasterDistribute_hpp__
#define __scatteredMasterDistribute_hpp__

#include "mpiCommunication.hpp"
#include "procCommunication.hpp"
#include "procVector.hpp"
#include "stdVectorHelper.hpp"
#include "streams.hpp"

namespace pFlow::MPI
{

template<typename T>
class scatteredMasterDistribute : public procCommunication
{
protected:

	procVector<DataType> indexedMap_;

	void freeIndexedMap();

public:

	scatteredMasterDistribute(const localProcessors& procs);

	~scatteredMasterDistribute()
	{
		freeIndexedMap();
	}

	scatteredMasterDistribute(const scatteredMasterDistribute&) = delete;

	scatteredMasterDistribute& operator=(const scatteredMasterDistribute&) =
	  delete;

	bool setDataMaps(procVector<span<uint32>>& maps);

	bool setDataMaps(procVector<span<int32>>& maps);

	bool distribute(span<T>& sendBuff, span<T>& recvb);
};

} // pFlow::MPI

#include "scatteredMasterDistribute.cpp"

#endif //__scatteredMasterDistribute_hpp__
