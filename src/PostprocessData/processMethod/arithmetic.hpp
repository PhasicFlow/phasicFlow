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

#ifndef __arithmetic_hpp__
#define __arithmetic_hpp__

#include <vector>

#include "types.hpp"
#include "typeInfo.hpp"
#include "span.hpp"

namespace pFlow
{

class dictionary;

namespace postprocessData
{

class arithmetic
{
private:

    std::vector<real>      weight_;
    
public:

    // type info
    TypeInfoNV("arithmetic");

    arithmetic()
    {}

    arithmetic(const arithmetic&) = default;

    arithmetic(arithmetic&&) = default;

    arithmetic& operator=(const arithmetic&) = default;

    arithmetic& operator=(arithmetic&&) = default;

    ~arithmetic()=default;


    bool updateWeights
    (
        const realx3& center, 
        const span<uint32>& indices,  
        const span<realx3>& points
    )
    {
        return updateWeights(indices.size());
    }

    bool updateWeights(uint32 n)
    {
        n = max(n, 1u);
        weight_.assign(n, 1);
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

} // namespace postprocessData

} // namespace pFlow

#endif //__arithmetic_hpp__