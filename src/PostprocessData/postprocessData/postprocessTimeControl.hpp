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

#ifndef __postprocessTimeControl_hpp__
#define __postprocessTimeControl_hpp__

#include "baseTimeControl.hpp"  
#include "dictionary.hpp"

namespace pFlow::postprocessData
{

class postprocessTimeControl 
: 
    public baseTimeControl
{
public:
postprocessTimeControl(
        const dictionary&          dict,
        const baseTimeControl&     defaultTimeControl,
        const baseTimeControl&     settingTimeControl
    )
    : 
        baseTimeControl(0, 1, 1)
    {
        auto tControl = dict.getValOrSet<word>("timeControl", "default");
        if(tControl == "default")
        {
            baseTimeControl::operator=(
                defaultTimeControl
            );
        }
        else if(tControl == "settingsDict")
        {
            baseTimeControl::operator=(
                settingTimeControl
            );
        }
        else
        {
            baseTimeControl::operator=( baseTimeControl(dict, "execution") );
        }
    }

    // Additional methods and members can be added here
};

} // namespace pFlow::postprocessData

#endif // __postprocessTimeControl_hpp__