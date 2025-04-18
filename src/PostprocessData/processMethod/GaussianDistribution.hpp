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

#ifndef __GaussianDistribution_hpp__
#define __GaussianDistribution_hpp__

#include <vector>

#include "typeInfo.hpp"
#include "types.hpp"
#include "span.hpp"
#include "numericConstants.hpp"

namespace pFlow
{

class GaussianDistribution
{
private:

    std::vector<real>      weight_;

    real            variance_ = 1.0;

    realx3          mean_ = {0,0,0};

public:

    TypeInfoNV("GaussianDistribution");


    GaussianDistribution() = default;

    GaussianDistribution(realx3 mean, real variance)
    :
        weight_(),
        variance_(variance),
        mean_(mean)
    {}

    GaussianDistribution(const GaussianDistribution&) = default;

    GaussianDistribution(GaussianDistribution&&) = default;

    GaussianDistribution& operator =(const GaussianDistribution&) = default;

    GaussianDistribution& operator = (GaussianDistribution&&) = default;

    ~GaussianDistribution()=default;

    bool updateWeights(const realx3& center, const span<uint32>& indices,  const span<realx3>& points)
    {
        weight_.clear();
        weight_.resize(indices.size());

        real sum = 0.0;
        for(uint32 i=0; i<indices.size(); i++)
        {
            auto x = points[indices[i]]-center;
            auto f = exp(- dot(x,x)/(2*variance_))/sqrt(2.0*Pi*variance_);
            weight_[i] = f;
            sum += f;
        }

        /*for(auto& w: weight_)
        {
            w /= sum;
        } */  
        return true;
    }

    bool updateWeights(uint32 n)
    {
        return false;
    }

    bool getWeight(uint32 i)const
    {
        return weight_[i];
    }

    span<real> getWeights()
    {
        return span<real>(weight_.data(), weight_.size());
    }

};
} // namespace pFlow

#endif // __GaussianDistribution_hpp__