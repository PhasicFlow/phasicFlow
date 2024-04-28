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
#ifndef __processorBoundaryContactSearch_hpp__
#define __processorBoundaryContactSearch_hpp__

#include "boundaryContactSearch.hpp"
#include "pointFields.hpp"
#include "twoPartContactSearch.hpp"

namespace pFlow
{

class processorBoundaryContactSearch : public boundaryContactSearch
{
private:

	box                             searchBox_;

	uniquePtr<twoPartContactSearch> ppContactSearch_ = nullptr;

	const realPointField_D&         diameter_;

	bool                            masterSearch_;

	void                            setSearchBox();

public:

	TypeInfo("boundaryContactSearch<MPI,processor>")

	  processorBoundaryContactSearch(
	    const dictionary&    dict,
	    const boundaryBase&  boundary,
	    const contactSearch& cSearch
	  );

	~processorBoundaryContactSearch() override = default;

	add_vCtor(
	  boundaryContactSearch,
	  processorBoundaryContactSearch,
	  boundaryBase
	);

	bool broadSearch(
	  uint32               iter,
	  real                 t,
	  real                 dt,
	  csPairContainerType& ppPairs,
	  csPairContainerType& pwPairs,
	  bool                 force = false
	) override;
};

}

#endif //__processorBoundaryContactSearch_hpp__