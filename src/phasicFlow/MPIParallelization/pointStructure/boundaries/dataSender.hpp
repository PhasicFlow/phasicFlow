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

	mutable BufferVectorType buffer_;

	int              toProc_;

	int              tag_;

	mutable Request  sendRequest_ = RequestNull;

public:

    dataSender(const word& name, int toProc, int tag)
    :
        toProc_(toProc),
        tag_(tag)
    {}

    ~dataSender()
    {
        if(sendRequest_ != RequestNull)
        {
            MPI_Request_free(&sendRequest_);
        }
    }

    bool waitBufferForUse()const
    {
        if(sendRequest_ != RequestNull)
        {
            MPI_Wait(&sendRequest_, StatusesIgnore);
        }
        return true;
    }

    void sendData(
        const localProcessors&      processors,
        const scatteredFieldAccess<T, memory_space>&  scatterField
    )
    {
        using RPolicy = Kokkos::RangePolicy<
                execution_space,
                Kokkos::Schedule<Kokkos::Static>,
                Kokkos::IndexType<pFlow::uint32>>;

        uint32 n = scatterField.size();
        
        // make sure the buffer is ready to be used and free 
        // the previous request (if any).
        waitBufferForUse();

        // clear the buffer to prevent data copy if capacity increases 
        buffer_.clear();
        buffer_.resize(n);
        
        const auto& buffView = buffer_.deviceViewAll();

        Kokkos::parallel_for(
            "dataSender::sendData",
            RPolicy(0,n),
            LAMBDA_HD(uint32 i)
            {
                buffView[i] = scatterField[i];
            }
        );
        Kokkos::fence();
        
        CheckMPI(
            Isend(buffer_.getSpan(),
                toProc_,
                tag_,
                processors.localCommunicator(),
                &sendRequest_
            ), 
            true
        );
                
    }

    bool recieveBackData(
        const localProcessors&      processors,
        uint32 numToRecieve
    )const
    {       
        // make sure the buffer is ready to be used and free 
        // the previous request (if any).
        waitBufferForUse();

        // clear the buffer to prevent data copy if capacity increases 
        buffer_.clear();
        buffer_.resize(numToRecieve);
        
        Status status;
        CheckMPI(
            Irecv(
                buffer_.getSpan(),
                toProc_,
                tag_,
                processors.localCommunicator(),
                &sendRequest_
            ),
            true
        );

        return true;
    }

    auto& buffer()
    {
        return buffer_;
    }

    const auto& buffer()const
    {
        return buffer_;
    }

    inline
    void fill(const T& val)
    {
        waitBufferForUse();
        buffer_.fill(val);
    }

    uint32 size()const
    {
        return buffer_.size();
    }

    bool sendComplete()
    {
        int test;
        if(sendRequest_ != RequestNull)   
        {
            MPI_Test(&sendRequest_, &test, StatusIgnore);
            return test;
        }
        else
        {
            return true;
        }
    }

    inline
    void resize(uint32 newSize)
    {
        waitBufferForUse();
        buffer_.clear();
        buffer_.resize(newSize);
    }

};

}

#endif //__dataSender_hpp__