#ifndef __datIOMPI_hpp__
#define __datIOMPI_hpp__

#include "dataIO.hpp"
#include "pFlowProcessors.hpp"

#ifdef pFlow_Build_MPI
    #include "gatherMaster.hpp"
#endif

namespace pFlow
{

template<typename T>
class dataIOMPI
:
	public dataIO<T>
{
protected:
    
    bool gatherData(span<T> data ) override
    {
        
        if(this->ioPattern_.isAllProcessorsDifferent())
        {
            this->bufferSpan_ = data;
            return true;
        }
        
        if( this->ioPattern_.isMasterProcessorDistribute())
        {

#ifdef pFlow_Build_MPI
            
            auto gatherT = pFlow::MPI::gatherMaster<T>(pFlowProcessors());
            
            if(!gatherT.gatherData(data))
            {
                fatalErrorInFunction<<"Error in gathering data to master"<<endl;
                return false;
            }

            this->buffer_ = gatherT.moveData();

            this->bufferSpan_ = makeSpan(this->buffer_);

            return true;
#else
            notImplementedFunction;
            fatalExit;
            return false;
#endif //pFlow_Build_MPI
        
        }

        if( this->ioPattern_.isMasterProcessorOnly() || this->ioPattern_.isAllProcessorSimilar() )
        {
            if( this->ioPattern_.isMaster() )
            {
                this->bufferSpan_ = data;
            }
            else
            {
                this->bufferSpan_ = span<T>(nullptr, 0);
                return true;
            }
        }

        return false;
    }
public:

	TypeInfo("dataIO<MPI>");

	dataIOMPI(const IOPattern& iop)
    :
        dataIO<T>(iop)
    {}

	dataIOMPI(const dataIOMPI&) = default;

	dataIOMPI(dataIOMPI&&) = default;


	dataIOMPI& operator=(const dataIOMPI&) = default;

	dataIOMPI& operator=(dataIOMPI&&) = default;

	~dataIOMPI() = default;

};


}


#endif