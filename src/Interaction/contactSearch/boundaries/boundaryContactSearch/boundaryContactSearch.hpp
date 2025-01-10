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
#ifndef __boundaryContactSearch_hpp__
#define __boundaryContactSearch_hpp__

#include "generalBoundary.hpp"
#include "contactSearchGlobals.hpp"
#include "virtualConstructor.hpp"

namespace pFlow
{

class contactSearch;

class boundaryContactSearch
    : public generalBoundary
{
private:
    const contactSearch &contactSearch_;

    /// @brief update interval in terms of iteration numebr
    uint32 updateInterval_;

    /// @brief  last iteration number which contact search has been performed
    uint32 lastUpdated_ = 0;

    /// @brief performed search?
    bool performedSearch_ = false;

public:
    // type info
    TypeInfo("boundaryContactSearch<none>");

    boundaryContactSearch(
        const dictionary &dict,
        const boundaryBase &boundary,
        const contactSearch &cSearch);

    create_vCtor(
        boundaryContactSearch,
        boundaryBase,
        (
            const dictionary &dict,
            const boundaryBase &boundary,
            const contactSearch &cSearch),
        (dict, boundary, cSearch));

    add_vCtor(
        boundaryContactSearch,
        boundaryContactSearch,
        boundaryBase);

    const contactSearch &cSearch() const
    {
        return contactSearch_;
    }

    bool hearChanges(
        real t,
        real dt,
        uint32 iter,
        const message &msg,
        const anyList &varList) override
    {

        if (msg.equivalentTo(message::BNDR_RESET))
        {
            // do nothing
        }
        return true;
    }

    virtual bool broadSearch(
        uint32 iter,
        real t,
        real dt,
        csPairContainerType &ppPairs,
        csPairContainerType &pwPairs,
        bool force = false)
    {
        return true;
    }

    static uniquePtr<boundaryContactSearch> create(
        const dictionary &dict,
        const boundaryBase &boundary,
        const contactSearch &cSearch);
};

}

#endif //__boundaryContactSearch_hpp__
