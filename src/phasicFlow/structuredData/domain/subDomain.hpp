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

#ifndef __subDomain_hpp__
#define __subDomain_hpp__

#include "domain.hpp"


namespace pFlow
{

class subDomain
{
protected:	

	/// the actual limits of global domain 
	const domain& 		globalDomain_;

	/// @brief dictionary of boundaries
	const dictionary& 	boundaryDict_;


	/// the actual limits of this processor domain 
	domain 		thisDomain_;



public:

	// - type info
	TypeInfo("subDomain");


	subDomain(
		const domain& gDomain,
		const domain& thisD, 
		const dictionary& bDicts)
	:
		globalDomain_(gDomain),
		thisDomain_(thisD),
		boundaryDict_(bDicts)
	{}
	
	const auto& thisDomain()const
	{
		return thisDomain_;
	}

	const dictionary& leftDict()const
	{
		return boundaryDict_.subDict("left");
	}

	const dictionary& rightDict()const
	{
		return boundaryDict_.subDict("right");
	}

	const dictionary& bottomDict()const
	{
		return boundaryDict_.subDict("bottom");;
	}

	const dictionary& topDict()const
	{
		return boundaryDict_.subDict("top");;
	}

	const dictionary& rearDict()const
	{
		return boundaryDict_.subDict("rear");;
	}

	const dictionary& frontDict()const
	{
		return boundaryDict_.subDict("front");;
	}
	
}; // subDomain



}


#endif //__subDomain_hpp__
