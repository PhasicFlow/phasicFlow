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

#include "types.hpp"
#include "List.hpp"

namespace pFlow
{

class subSurface
{
private:
    uint32  start_= 0;

    uint32  end_ =  0;

    uint32  pointStart_ = 0;

    uint32  pointEnd_=0;

    word    name_;

public:
    subSurface()=default;

    subSurface(const word& name, uint32 start, uint32 end, uint32 pointStart, uint32 pointEnd)
    :
        start_(start),
        end_(end),
        pointStart_(pointStart),
        pointEnd_(pointEnd),
        name_(name)
    {}

    subSurface(const subSurface&) = default;

    subSurface& operator = (const subSurface&) = default;

    ~subSurface()=default;

    uint32 operator()(uint32 i)const
    {
        return start_ + i;
    }
    uint32 start()const
    {
        return start_;
    }

    uint32 end()const
    {
        return end_;
    }

    uint32 pointStart()const
    {
        return pointStart_;
    }

    uint32 pointEnd()const
    {
        return pointEnd_;
    }
    
    const auto& name()const
    {
        return name_;
    }

    uint32 size()const
    {
        return end_-start_;
    }

    friend iOstream& operator<< (iOstream& str, const subSurface & sub);
		
		/// >> operator 
	friend iIstream& operator >> (iIstream & str, subSurface & sub);
};

iOstream& operator<< (iOstream& str, const subSurface & sub);

iIstream& operator >> (iIstream & str, subSurface & sub);

using subSurfaceList = List<subSurface>;


}