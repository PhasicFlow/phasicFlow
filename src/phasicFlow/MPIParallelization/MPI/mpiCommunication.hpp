/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

#ifndef __mpiCommunication_H__
#define __mpiCommunication_H__


#include "mpiTypes.hpp"
#include "types.hpp"
#include "span.hpp"



namespace pFlow::MPI
{

extern DataType realx3Type__;

extern DataType realx4Type__;

extern DataType int32x3Type__;

template<typename T> 
auto constexpr Type()
{
	return MPI_BYTE;
}

template<typename T>
auto constexpr sFactor()
{
	return sizeof(T);
}

template<char> 
auto constexpr Type()
{
	return MPI_CHAR;
}
template<char>
auto constexpr sFactor()
{
	return 1;
}

template<short>
auto constexpr Type()
{
	return MPI_SHORT;
}
template<short>
auto constexpr sFactor()
{
	return 1;
}

template<unsigned short>
auto constexpr Type()
{
	return MPI_UNSIGNED_SHORT;
}
template<unsigned short>
auto constexpr sFactor()
{
	return 1;
}

template<int>
auto constexpr Type()
{
	return MPI_INT;
}
template<int>
auto constexpr sFactor()
{
	return 1;
}

template<>
auto constexpr Type<unsigned int>()
{
	return MPI_UNSIGNED;
}
template<>
auto constexpr sFactor<unsigned int>()
{
	return 1;
}

template<>
auto constexpr Type<long>()
{
	return MPI_LONG;
}
template<>
auto constexpr sFactor<long>()
{
	return 1;
}

template<>
auto constexpr Type<unsigned long>()
{
	return MPI_UNSIGNED_LONG;
}
template<>
auto constexpr sFactor<unsigned long>()
{
	return 1;
}


template<>
auto constexpr Type<float>()
{
	return MPI_FLOAT;
}
template<>
auto constexpr sFactor<float>()
{
	return 1;
}

template<>
auto constexpr Type<double>()
{
	return MPI_DOUBLE;
}
template<>
auto constexpr sFactor<double>()
{
	return 1;
}

template<>
inline
auto Type<realx3>()
{
	return realx3Type__;
}

template<>
auto constexpr sFactor<realx3>()
{
	return 1;
}

template<>
inline
auto Type<realx4>()
{
	return realx4Type__;
}

template<>
auto constexpr sFactor<realx4>()
{
	return 1;
}


template<>
inline
auto Type<int32x3>()
{
	return int32x3Type__;
}


template<>
auto constexpr sFactor<int32x3>()
{
	return 1;
}

/*inline 
auto createByteSequence(int sizeOfElement)
{
    DataType newType;
    MPI_Type_contiguous(sizeOfElement, MPI_CHAR, &newType);
	MPI_Type_commit(&newType);
    return newType;
}*/

inline 
auto TypeCommit(DataType* type)
{
	return MPI_Type_commit(type);
}

inline 
auto TypeFree(DataType* type)
{
    return MPI_Type_free(type);

}
template<typename T>
inline auto getCount(Status* status, int& count)
{
	int lCount;
	auto res = MPI_Get_count(status, Type<T>(), &lCount);
	count = lCount/sFactor<T>();
	return res;
}

template<typename T>
inline int convertIndex(const int& ind)
{
	return ind*sFactor<T>();
}

template<typename T> 
inline auto send(span<T> data, int dest, int tag, Comm comm)
{
	return MPI_Send(
        data.data(), 
        sFactor<T>()*data().size(), 
        Type<T>(), 
        dest, 
        tag, 
        comm);
}

template<typename T>
inline auto Isend(span<T> data, int dest, int tag, Comm comm, Request* req)
{
	return MPI_Isend(
		data.data(), 
		sFactor<T>()*data.size(), 
		Type<T>(), 
		dest, 
		tag, 
		comm, 
		req);
}

template<typename T>
inline auto Isend(const T& data, int dest, int tag, Comm comm, Request* req)
{
	return MPI_Isend(
		&data, 
		sFactor<T>(), 
		Type<T>(), 
		dest, 
		tag, 
		comm, 
		req);
}

template<typename T>
inline auto recv(span<T> data, int source, int tag, Comm comm, Status *status)
{
	return MPI_Recv(
        data.data(), 
        sFactor<T>()*data.size(), 
        Type<T>(), 
        source, 
        tag, 
        comm, 
        status);
}

template<typename T>
inline auto Irecv(T& data, int source, int tag, Comm comm, Request* req)
{
	return MPI_Irecv(
		&data,
		sFactor<T>(),
		Type<T>(),
		source,
		tag, 
		comm,
		req);
}

template<typename T>
inline auto Irecv(span<T> data, int source, int tag, Comm comm, Request* req)
{
	return MPI_Irecv(
		data.data(),
		sFactor<T>()*data.size(),
		Type<T>(),
		source,
		tag, 
		comm,
		req);
}

template<typename T>
inline auto scan(T sData, T& rData, Comm comm, Operation op = SumOp)
{
	return MPI_Scan(&sData, &rData, sFactor<T>()*1, Type<T>(), op , comm );
}

// gathering one scalar data to root processor 
template<typename T>
inline auto gather(T sendData, span<T>& recvData, int root, Comm comm)
{
	return MPI_Gather(
		&sendData, 
		sFactor<T>()*1, 
		Type<T>(), 
		recvData.data(),
		sFactor<T>()*1,
		Type<T>(),
		root,
		comm);
}

template<typename T>
inline auto allGather(T sendData, span<T>& recvData, Comm comm)
{
	return MPI_Allgather(
		&sendData,
		sFactor<T>()*1,
		Type<T>(),
		recvData.data(),
		sFactor<T>()*1,
		Type<T>(),
		comm);
}

template<typename T>
inline auto scatter(span<T> sendData, T& recvData, int root, Comm comm)
{
	return MPI_Scatter(
		sendData.data(),
		sFactor<T>()*1,
		Type<T>(),
		&recvData,
		sFactor<T>()*1,
		Type<T>(),
		root,
		comm);
}

template<typename T>
inline auto Bcast(T& sendData, int root, Comm comm)
{
	return MPI_Bcast(
		&sendData, sFactor<T>()*1, Type<T>(), root, comm);

}


template<typename T> 
bool typeCreateIndexedBlock(
    span<int32> index, 
    DataType &newType)
{
    auto res =  MPI_Type_create_indexed_block(
        index.size(), 
        sFactor<T>(), 
        index.data(), 
        Type<T>(), 
        &newType);
    
    if(res == Success)
    {
        TypeCommit(&newType);
    }
    else
    {
        return false;
    }

    return true;	
}


template<typename T>
inline auto Gatherv
(
    span<T> sendData, 
    span<T>& recvData, 
    span<int> recvCounts,
    span<int> displs,
    int root, 
    Comm comm)
{
    
    return MPI_Gatherv(
        sendData.data(), 
        sendData.size()*sFactor<T>(),
        Type<T>(),
        recvData.data(),
        recvCounts.data(),
        displs.data(),
        Type<T>(),
        root,
        comm
         );

}

inline auto Wait(Request* request, Status* status)
{
	return MPI_Wait(request, status);
}

inline auto typeFree(DataType& type)
{
	return MPI_Type_free(&type);
}


}


#endif  //__mpiCommunication_H__
