
#ifndef __dataReciever_hpp__
#define __dataReciever_hpp__


#include "span.hpp"
#include "localProcessors.hpp"
#include "mpiCommunication.hpp"

namespace pFlow::MPI
{

template<typename T, typename MemorySpace=void>
class dataReciever
{
public:
    
    using BufferVectorType = VectorSingle<T, MemorySpace>;

	using BufferVectorTypeHost = VectorSingle<T, HostSpace>;

	using memory_space = typename BufferVectorType::memory_space;

	using execution_space = typename BufferVectorType::execution_space;

private:	
    
    BufferVectorType buffer_;

	int              fromProc_;

	int              tag_;

	mutable Request  recvRequest_ = RequestNull;

public:

    dataReciever(const word& name, int from, int tag)
    :
        buffer_(name),
        fromProc_(from),
        tag_(tag)
    {}

    ~dataReciever()=default;

    uint32 waitBufferForUse()const
    {
        if(recvRequest_ != RequestNull)
        {
            Status status;
            MPI_Wait(&recvRequest_, &status);
            int count;
            CheckMPI(getCount<T>(&status, count), true);

            return static_cast<uint32>(count);
        }
        else
            return buffer_.size();
    }

    void sendBackData(
        const localProcessors& processors)const
    {
        CheckMPI(
            Isend(
                buffer_.getSpan(),
                fromProc_,
                tag_,
                processors.localCommunicator(),
                &recvRequest_
            ),
            true
        );
    }

    void recieveData(
        const localProcessors&      processors,
        uint32 numToRecv,
        const word& name = "dataReciver"
    )
    {   
        resize(numToRecv);
        
        CheckMPI(
            Irecv(
                buffer_.getSpan(), 
                fromProc_, 
                tag_, 
                processors.localCommunicator(), 
                &recvRequest_
            ),
            true
        );
    }

    inline
    auto& buffer()
    {
        return buffer_;
    }

    inline
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

    inline
    uint32 size()const
    {
        return buffer_.size();
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


#endif //__dataReciever_hpp__
