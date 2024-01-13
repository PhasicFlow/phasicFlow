#ifndef __createDataIO_hpp__
#define __createDataIO_hpp__

#include "uniquePtr.hpp"
#include "dataIO.hpp"
#include "dataIORegular.hpp"
#include "dataIOMPI.hpp"

namespace pFlow
{

template<typename T>
uniquePtr<dataIO<T>> createDataIO(
    const word& runType, const IOPattern& iop)
{
    if(runType == "regular")
    {
        return makeUnique<dataIORegular<T>>(iop);
    }
    else if(runType == "MPI")
    {
        return makeUnique<dataIOMPI<T>>(iop);
    }
    else
    {
        fatalErrorInFunction<< "not dataIO pattern is specified for this "<<
        runType<<endl;
        fatalExit;
    }
    return nullptr;
}

}

#endif