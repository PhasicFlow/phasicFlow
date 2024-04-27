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

#ifndef __scatteredMasterDistributeChar_hpp__
#define __scatteredMasterDistributeChar_hpp__

#include "scatteredMasterDistribute.hpp"

namespace pFlow::MPI
{

template<>
class scatteredMasterDistribute<char> : public procCommunication
{
protected:

	procVector<DataType> indexedMap_;

	size_t               sizeOfElement_;

	void freeIndexedMap();

public:

	scatteredMasterDistribute(
	  size_t                 sizeOfElement,
	  const localProcessors& procs
	);

	~scatteredMasterDistribute()
	{
		freeIndexedMap();
	}

	scatteredMasterDistribute(const scatteredMasterDistribute&) = delete;

	scatteredMasterDistribute& operator=(const scatteredMasterDistribute&) =
	  delete;

	bool setDataMaps(procVector<span<uint32>>& maps);

	bool setDataMaps(procVector<span<int32>>& maps);

	bool distribute(span<char>& sendBuff, span<char>& recvb);
};

} // pFlow::MPI

#endif //__scatteredMasterDistributeChar_hpp__
