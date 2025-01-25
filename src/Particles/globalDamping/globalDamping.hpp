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

#ifndef __globalDamping_hpp__
#define __globalDamping_hpp__

#include "systemControl.hpp"
#include "pointFields.hpp"
#include "baseTimeControl.hpp"

namespace pFlow
{


class globalDamping
{
private:

    bool            performDamping_ = false;

    real            dampingFactor_;

  //  baseTimeControl timeControl_;


public:


    globalDamping(const systemControl& control);

    ~globalDamping()=default;

    //void applyDamping( const timeInfo& ti, realx3PointField_D& velocity);

    bool performDamping()const
    {
        return performDamping_;
    }

    real dampingFactor(const timeInfo& ti)const;
    
};


}


#endif 