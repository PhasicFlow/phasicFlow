#ifndef __dataSender_hpp__
#define __dataSender_hpp__

#include "VectorSingles.hpp"
#include "localProcessors.hpp"
#include "mpiCommunication.hpp"

namespace pFlow::MPI
{

template<typename T, typename MemorySpace=void>
class dataSender
{
public:

	using BufferVectorType = VectorSingle<T, MemorySpace>;

	using BufferVectorTypeHost = VectorSingle<T, HostSpace>;

	using memory_space = typename BufferVectorType::memory_space;

	using execution_space = typename BufferVectorType::execution_space;

	// This is device vector 
    

private:

	//BufferVectorType buffer_;

    std::vector<T> buffer_;

	int              toProc_;

	int              tag_;

	Request          sendRequest_ = RequestNull;

public:

    dataSender(const word& name, int toProc, int tag)
    :
        toProc_(toProc),
        tag_(tag)
    {}

    ~dataSender()=default;

    void sendData(
        const localProcessors&      processors,
        const scatteredFieldAccess<T, memory_space>&  scatterField
    )
    {
        using RPolicy = Kokkos::RangePolicy<
                DefaultExecutionSpace,
                Kokkos::Schedule<Kokkos::Static>,
                Kokkos::IndexType<pFlow::uint32>>;

        uint32 n = scatterField.size();

        // clear the buffer to prevent data copy if capacity increases 
        buffer_.clear();
        buffer_.resize(n);
        
        auto* buffView = buffer_.data();

        Kokkos::parallel_for(
            "dataSender::sendData",
            RPolicy(0,n),
            LAMBDA_HD(uint32 i)
            {
                buffView[i] = scatterField[i];
            }
        );
        Kokkos::fence();
        auto req = MPI_REQUEST_NULL;

        MPI_Isend(
            buffer_.data(),
            buffer_.size(),
            realx3Type__,
            toProc_,
            tag_,
            processors.localCommunicator(),
            &req);
        
        /*CheckMPI(send(
            buffer_.getSpan(), 
            toProc_, 
            tag_, 
            processors.localCommunicator(), 
            MPI_STATUS_IGNORE), true);*/
    }

    /*auto& buffer()
    {
        return buffer_;
    }

    const auto& buffer()const
    {
        return buffer_;
    }*/

    bool sendComplete()
    {
        return true;
        /*int test;   
        MPI_Test(&sendRequest_, &test, StatusIgnore);
        if(test) 
            return true;
        else
            return false;*/
    }

};

}

#endif //__dataSender_hpp__