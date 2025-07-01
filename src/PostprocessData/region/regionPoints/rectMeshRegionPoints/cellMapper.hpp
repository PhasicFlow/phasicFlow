
#ifndef __cellMapper_hpp__
#define __cellMapper_hpp__

#include "types.hpp"

namespace pFlow::postprocessData
{

struct cellMapper
{
    uint32x3 cells_;

    cellMapper()
    :
        cells_()
    {}

    explicit cellMapper(uint32x3 cells)
    :
        cells_(cells)
    {}

    cellMapper(const cellMapper&) = default;
    cellMapper(cellMapper&&) = default;
    cellMapper& operator=(const cellMapper&) = default;
    cellMapper& operator=(cellMapper&&) = default;
    ~cellMapper() = default;

    inline
    uint32 operator()(uint32 i, uint32 j, uint32 k)const
    {
        return (k*(cells_.y()*cells_.x()))+j*cells_.x() + i;
    }

    uint32 nx()const
    {
        return cells_.x();
    }

    uint32 ny()const
    {
        return cells_.y();
    }

    uint32 nz()const
    {
        return cells_.z();
    }

};


} //pFlow::postprocessData

#endif //__cellMapper_hpp__