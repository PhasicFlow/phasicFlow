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
#ifndef __searchBoundary_hpp__
#define __searchBoundary_hpp__


#include "generalBoundary.hpp"
#include "virtualConstructor.hpp"

namespace pFlow
{

class contactSearch;

class searchBoundary
:
    public generalBoundary
{
private:

    const contactSearch&  contactSearch_;

    /// @brief update interval in terms of iteration numebr 
    uint32      updateInterval_;

    /// @brief  last iteration number which contact search has been performed
	uint32 		lastUpdated_ 	= 0;

	/// @brief performed search? 
	bool 		performedSearch_ = false;

public:

    // type info
    TypeInfo("searchBoundary<regular,none>");


    searchBoundary(
		const dictionary& dict,
		const boundaryBase& boundary,
		const contactSearch& cSearch);

    create_vCtor
    (
        searchBoundary,
        boundaryBase,
        (
            const dictionary& dict,
			const boundaryBase& boundary,
			const contactSearch& cSearch
        ),
        (dict, boundary, cSearch)
    );

    add_vCtor
    (
        searchBoundary,
        searchBoundary,
        boundaryBase
    );

    
	void fill(const std::any& val)override
    {
        return;
    }

    bool hearChanges
	(
		real t,
		real dt,
		uint32 iter,
		const message& msg, 
    	const anyList& varList
	) override
    {
		
		if(msg.equivalentTo(message::BNDR_RESET))
		{
			//do nothing
		}
		return true;
	}

	static
	uniquePtr<searchBoundary> create(
		const dictionary& dict,
		const boundaryBase& boundary,
		const contactSearch& cSearch);

};

}

#endif //__searchBoundary_hpp__

