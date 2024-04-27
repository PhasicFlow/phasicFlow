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

#ifndef __boundaryProcessor_hpp__
#define __boundaryProcessor_hpp__


#include "boundaryBase.hpp"
#include "mpiTypes.hpp"
#include "dataSender.hpp"
#include "dataReciever.hpp"

namespace pFlow::MPI
{

class boundaryProcessor
:
 	public boundaryBase
{
private:

	uint32                       neighborProcNumPoints_ = 0;

	uint32                       thisNumPoints_;

	realx3Vector_D               neighborProcPoints_;

	mutable Request              sizeRequest_;

	mutable Request 			sSizeRequest_;

	int req_=0;

	mutable bool                 sizeObtained_ = true;

	mutable dataSender<realx3>   sender_;

	mutable dataReciever<realx3> reciever_;

	mutable bool                 dataRecieved_ = true;

	void checkSize()const;

	void checkDataRecieved()const;
	
	/// @brief  Update processor boundary data for this processor
	/// @param step It is either 1 or 2 in the input to indicate 
	/// the update step 
	/// @return true if successful 
	/// @details This method is called by boundaryList two times to 
	/// allow processor boundaries to exchange data in two steps.
	/// The first step is a buffered non-blocking send and the second
	/// step is non-blocking recieve to get data. 
	bool updataBoundary(int step)override;

public:

	TypeInfo("boundary<processor>");

	boundaryProcessor(
	  const dictionary& dict,
	  const plane&      bplane,
	  internalPoints&   internal,
	  boundaryList&     bndrs,
	  uint32            thisIndex
	);

	~boundaryProcessor() override = default;

	add_vCtor
	(
		boundaryBase, 
		boundaryProcessor, 
		dictionary
	);

	bool beforeIteration(uint32 iterNum, real t, real dt) override;

	bool iterate(uint32 iterNum, real t, real dt) override;

	bool afterIteration(uint32 iterNum, real t, real dt) override;

	/// @brief Return number of points in the neighbor processor boundary.
	/// This is overriden from boundaryBase.
	uint32 neighborProcSize() const override;

	/// @brief Return a reference to point positions in the neighbor
	/// processor boundary.
	realx3Vector_D& neighborProcPoints() override;

	/// @brief Return a const reference to point positions in the
	/// neighbor processor boundary.
	const realx3Vector_D& neighborProcPoints() const override;

};

} // namespace pFlow::MPI

#endif //__boundaryProcessor_hpp__