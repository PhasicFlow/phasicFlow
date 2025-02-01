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
#ifndef __periodicBoundaryContactSearch_hpp__
#define __periodicBoundaryContactSearch_hpp__

#include "boundaryContactSearch.hpp"
#include "box.hpp"
#include "ppwBndryContactSearch.hpp"
#include "pointFields.hpp"
#include "wallBoundaryContactSearch.hpp"

namespace pFlow
{


class periodicBoundaryContactSearch
	: public boundaryContactSearch
{
private:
	box searchBox_;

	realx3 transferVec_;

	uint32 thisIndex_;

	uint32 mirrorIndex_;

	uniquePtr<ppwBndryContactSearch> 	ppContactSearch_ = nullptr;

	uniquePtr<wallBoundaryContactSearch> pwContactSearch_ = nullptr;

	const realPointField_D 				&diameter_;

	bool masterSearch_ = false;

	void setSearchBox();

public:
	TypeInfo("boundaryContactSearch<regular,periodic>")

		periodicBoundaryContactSearch(
			const dictionary &dict,
			const boundaryBase &boundary,
			const contactSearch &cSearch);

	~periodicBoundaryContactSearch() override = default;

	add_vCtor(
		boundaryContactSearch,
		periodicBoundaryContactSearch,
		boundaryBase);

	bool broadSearch(
		uint32 iter,
		real t,
		real dt,
		csPairContainerType &ppPairs,
		csPairContainerType &pwPairs,
		bool force = false) override;
	
	bool isActive()const override
	{
		return true;
	}
};

}

#endif //__periodicBoundaryContactSearch_hpp__