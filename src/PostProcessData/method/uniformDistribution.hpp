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

#ifndef __uniformDistribution_hpp__
#define __uniformDistribution_hpp__

#include "types.hpp"
#include "typeInfo.hpp"
#include "span.hpp"

namespace pFlow
{

class dictionary;

class uniformDistribution
{
private:

    std::vector<real>      weight_;
    
public:

    // type info
    TypeInfoNV("uniformDistribution");

    uniformDistribution()
    {}

    uniformDistribution(const uniformDistribution&) = default;

    uniformDistribution(uniformDistribution&&) = default;

    uniformDistribution& operator=(const uniformDistribution&) = default;

    uniformDistribution& operator=(uniformDistribution&&) = default;

    ~uniformDistribution()=default;


    bool updateWeights(const realx3& center, const span<realx3>& points)
    {
        uint32 n = max(points.size(), 1u);
        weight_.assign(n, 1.0/n);
        return true;
    }

    bool updateWeights(uint32 n)
    {
        n = max(n, 1u);
        weight_.assign(n, 1.0/n);
        return true;
    }

    real getWeight(uint32 i)const
    {
        return weight_[i];
    }

    span<real> getWeights()
    {
        return span<real>(weight_.data(), weight_.size());
    }

};

}

#endif //__uniformDistribution_hpp__