
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

    std::vector<T>  buffer0_;

	int              fromProc_;

	int              tag_;

	Request          recvRequest_;

public:

    dataReciever(const word& name, int from, int tag)
    :
        buffer_(name),
        fromProc_(from),
        tag_(tag)
    {}

    ~dataReciever()=default;

    void recieveData(
        const localProcessors&      processors,
        uint32 numToRecv
    )
    {   
        
        buffer0_.clear();
		buffer0_.resize(numToRecv);
        MPI_Status status;

        /*CheckMPI(recv(
            buffer_.getSpan(), 
            fromProc_, 
            tag_, 
            processors.localCommunicator(), 
            &status), true);*/
        MPI_Recv(
            buffer0_.data(),
            buffer0_.size(),
            realx3Type__,
            fromProc_,
            tag_,
            processors.localCommunicator(),
            &status
        );
        int c;
        getCount<realx3>(&status, c);
        pOutput<<"Number of data recieved "<<c<<endl;
    }

    auto& buffer()
    {
        return buffer_;
    }

    const auto& buffer()const
    {
        return buffer_;
    }

    uint32 waitComplete()
    {
        
        /*Status status;   
        
        CheckMPI(MPI_Wait(&recvRequest_, &status), true);

        int count;
        CheckMPI(getCount<T>(&status, count), true);

        return static_cast<uint32>(count);*/
        return buffer_.size();
    }

};

}


#endif //__dataReciever_hpp__
