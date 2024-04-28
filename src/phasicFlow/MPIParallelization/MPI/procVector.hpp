#ifndef __procVector_hpp__ 
#define __procVector_hpp__

// from PhasicFlow

#include "localProcessors.hpp"
#include "span.hpp"
#include "streams.hpp"
#include "IOPattern.hpp"

#include "mpiTypes.hpp"

namespace pFlow::MPI
{

template<typename T>
class procVector
:
	public std::vector<T>
{
public:

	using ProcVectorType = procVector<T>;

	using VectorType = std::vector<T>;

protected:

	int 	rank_ 	= 0;

    bool 	isMaster_ = false;

	using VectorType::reserve;

	using VectorType::resize;

	using VectorType::assign;

	using VectorType::clear;

	using VectorType::erase;

public:

	procVector(
		const localProcessors& procs,
		bool onlyMaster = false)
    :
        rank_(procs.localRank()),
        isMaster_(procs.localMaster())
	{
       
		if( onlyMaster && !isMaster_ ) return;
		this->reserve(procs.localSize());
		this->resize(procs.localSize());
	}

	procVector(
		const T& val,
		const localProcessors& procs,
		bool onlyMaster = false)
	:
		procVector(procs, onlyMaster)
	{
		std::fill(this->begin(), this->end(), val);
	}

    procVector(const T& val, const procVector& src)
    {
        this->reserve(src.size());
        this->resize(src.size());
        std::fill(this->begin(), this->end(), val);
    }

    procVector(const localProcessors& procs, const VectorType& src)
	:
		procVector(procs)
	{
		if(src.size()!= this->size())
		{
			fatalErrorInFunction<<
			"Size of std::vector and procVector does not match in construction"<<endl;
			fatalExit;
		}

        this->assign(src.begin(), src.end());
	} 
        
	procVector(const procVector&) = default;
	
	procVector(procVector&&) = default;

	procVector& operator=(const procVector&) = default;
	
	procVector& operator=(procVector&&) = default;

	procVector& operator=(const VectorType& src)
	{
		if(src.size() != this->size())
		{
			fatalErrorInFunction<<
			"Size of std::vector and procVector does not match in copy assignment"<<endl;
			fatalExit;
		}

		static_cast<VectorType&>(*this).operator=(src);
		return *this;
	}

	procVector& operator=(VectorType&& src)
	{
		if(src.size() != this->size())
		{
			fatalErrorInFunction<<
			"Size of std::vector and procVector does not match in move assignment"
			<<endl;
			fatalExit;
		}

		static_cast<VectorType&>(*this).operator=(std::move(src));
		return *this;
	}

	procVector(const localProcessors& procs, VectorType&& src)
	:
		VectorType(std::move(src))
	{
		if(this->size()!= static_cast<size_t>(procs.localSize()))
		{
			fatalErrorInFunction<<
			"Size of std::vector and procVector does not match in move"<<endl;
            fatalExit;
		}
        isMaster_ = procs.localMaster();
        rank_ = procs.localRank();
	}

	~procVector()=default;

    inline
	auto& thisValue()
	{
		return VectorType::operator[](rank_);
	}

    inline
	const auto& thisValue()const
	{
		return VectorType::operator[](rank_);
	}

    inline
	auto size()const
	{
		return VectorType::size();
	}

    inline
	auto rank()const
	{
		return rank_;
	}

	inline
	auto getSpan()
	{
		return span<T>(this->data(), this->size());
	}
	
	inline 
	auto getSpan()const
	{
		return span<T>(const_cast<T*>(this->data()), this->size());
	}

    bool write(
        iOstream& os,
        const IOPattern& iop ) const
    {
        return writeStdVector(os, *this, iop);	
    }

};

template<typename T> 
inline iOstream& operator << (iOstream& os, const procVector<T>& ovec )
{	
	if( !ovec.write(os, IOPattern::AllProcessorsDifferent) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}
	return os; 
}

}


#endif
