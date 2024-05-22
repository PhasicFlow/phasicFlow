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

#include "pointSorting.hpp"
#include "mortonIndexing.hpp"

pFlow::pointSorting::pointSorting(const dictionary & dict)
:
    performSorting_(dict.getValOrSet("active", Logical(false))),
    timeControl_(
        performSorting_()? 
            baseTimeControl(dict, "sorting"): 
            baseTimeControl(0,1,1, "sorting")
    ),
    dx_(
        performSorting_()?
            dict.getVal<real>("dx"):
            1.0
    )
{
    if( performSorting_() )
        REPORT(1)<<"Point sorting is "<<Yellow_Text("active")<<" in simulation"<<END_REPORT;
    else
       REPORT(1)<<"Point sorting is "<<Yellow_Text("inactive")<<" in simulation"<<END_REPORT;

}
pFlow::uint32IndexContainer 
pFlow::pointSorting::getSortedIndices(
    const box& boundingBox, 
    const ViewType1D<realx3> &pos, 
    const pFlagTypeDevice &flag
) const
{
    return pFlow::getSortedIndices(boundingBox, dx_, pos, flag);
}
