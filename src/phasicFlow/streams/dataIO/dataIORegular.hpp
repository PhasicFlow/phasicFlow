#ifndef __datIONoMPI_hpp__
#define __datIONoMPI_hpp__

#include "dataIO.hpp"

namespace pFlow
{
template<typename T>
class dataIORegular
:
	public dataIO<T>
{
protected:

    bool gatherData(span<T> data ) override
    {
        /// in serial mode, no gathering is required. 
        this->bufferSpan_ = data;
        return true;
    }

public:

	TypeInfo("dataIO<regular>");

	dataIORegular(const IOPattern& iop)
    :
	    dataIO<T>(iop)
    {}

	dataIORegular(const dataIORegular&) = default;

	dataIORegular(dataIORegular&&) = default;

	dataIORegular& operator=(const dataIORegular&) = default;

	dataIORegular& operator=(dataIORegular&&) = default;

	~dataIORegular() = default;
	
};


}


#endif