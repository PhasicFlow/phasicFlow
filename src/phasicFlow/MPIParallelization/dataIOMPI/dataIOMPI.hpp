#ifndef __datIOMPI_hpp__
#define __datIOMPI_hpp__

#include "dataIO.hpp"
#include "pFlowProcessors.hpp"
#include "gatherMaster.hpp"


namespace pFlow::MPI
{

template<typename T>
class dataIOMPI
:
	public dataIO<T>
{
public:

    using DataIOType = dataIO<T>;

    using DataIOMPIType = dataIOMPI<T>;

protected:
    
    bool gatherData(span<T> data ) override;
    
public:

	TypeInfoTemplate111("dataIO",T,"MPI");

	explicit dataIOMPI(const IOPattern& iop);
    
	dataIOMPI(const dataIOMPI&) = default;

	dataIOMPI(dataIOMPI&&) = default;


	dataIOMPI& operator=(const dataIOMPI&) = default;

	dataIOMPI& operator=(dataIOMPI&&) = default;

	~dataIOMPI() = default;

    add_vCtor
    (
        DataIOType,
        DataIOMPIType,
        IOPattern
    );

}; //dataIOMPI


} //namespace pFlow::MPI

#include "dataIOMPI.cpp"

#endif //__datIOMPI_hpp__