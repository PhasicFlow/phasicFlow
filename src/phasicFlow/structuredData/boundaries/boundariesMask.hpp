
#ifndef __boundariesMask_hpp__
#define __boundariesMask_hpp__

#include <array>

namespace pFlow
{

template<size_t N>
class boundariesMask
:
    public std::array<bool,N> 
{
public:
    
    boundariesMask()=default;

    boundariesMask(bool val)
    {
        this->fill(val);
    }

    boundariesMask(std::initializer_list<bool> l)
    :
        std::array<bool,N>(l)
    {}

    inline
    bool allElements(bool val)
    {
        return std::all_of(this->begin(), this->end(), [val](bool v) { return v==val;} );
    }

    inline
    bool anyElement(bool val)
    {
        return std::any_of(this->begin(), this->end(), [val](bool v) { return v==val;} );
    }
    
    inline
    bool noElement(bool val)
    {
        return std::none_of(this->begin(), this->end(), [val](bool v) { return v==val;} );
    }
};

}

#endif //__boundariesMask_hpp__