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

#include "boundaryContactSearchList.hpp"
#include "boundaryList.hpp"

void pFlow::boundaryContactSearchList::setList(
    const dictionary &dict, 
    const contactSearch &cSearch)
{
    ForAllBoundariesPtr(i, this)
    {
        this->set
        (
            i, 
            boundaryContactSearch::create(dict, boundaries_[i], cSearch)
        );
    }
}

pFlow::boundaryContactSearchList::boundaryContactSearchList(
    const dictionary &dict,
    const boundaryList& bndrs,
    const contactSearch &cSearch)
    :
    boundaryListPtr(), 
    boundaries_(bndrs)
{
    setList(dict, cSearch);
}

bool pFlow::boundaryContactSearchList::boundariesUpdated() const
{
    return boundaries_.boundariesUpdated();
}
