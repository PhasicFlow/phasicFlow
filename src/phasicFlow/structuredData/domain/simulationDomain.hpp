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
#ifndef __simulationDomain_hpp__
#define __simulationDomain_hpp__

#include <array>

#include "domain.hpp"
#include "span.hpp"
#include "streams.hpp"
#include "virtualConstructor.hpp"
#include "pointFlag.hpp"

namespace pFlow
{

//class pFlagTypeHost;

class simulationDomain
{
private:

static inline constexpr uint32 sizeOfBoundaries_ = 6;

static 
inline const std::array<word,6>  boundaryNames_ = 
{
	"left", "right", 
	"top", "bottom",
	"rear", "front"
};

protected:	

	/// @brief acutal limits of the simulation domain
	box 			globalBox_;

	/// @brief the actual limits of this processor domain 
	domain 			thisDomain_;

	dictionary 		globalDomainDict_ ;

	bool 			boundariesSet_ = false;
	
	virtual bool    createBoundaryDicts() = 0;

	virtual bool    setThisDomain() = 0;

public:

	// - type info
	TypeInfo("simulationDomain");

	/// Constrcut from components 
	simulationDomain(const dictionary& dict);
	
	/// Destructor 
	virtual 
	~simulationDomain() = default;
	

	create_vCtor
	(
		simulationDomain,
		dictionary,
		(const dictionary& dict),
		(dict)	
	);

	virtual 
	const dictionary& thisBoundaryDict()const = 0;
	
    virtual 
    bool initialUpdateDomains(span<realx3> pointPos) = 0;

    virtual
    uint32 initialNumberInThis()const = 0;

    virtual 
	bool initialTransferBlockData(
		span<char> src, 
		span<char> dst,
		size_t sizeOfElement) = 0;
    
    virtual 
    bool initialTransferBlockData(
        span<realx3> src,
        span<realx3> dst) = 0;
    
    virtual
    bool initialTransferBlockData(
        span<real> src,
        span<real> dst) = 0;

    virtual
    bool initialTransferBlockData(
        span<uint32> src,
        span<uint32> dst) = 0;
    
    virtual 
    bool initialTransferBlockData(
        span<int32> src,
        span<int32> dst) = 0;
    
    
    /*template<typename T>
    bool initialTransferBlockData(
        span<T> src,
        span<T> dst )
    {
        size_t el=sizeof(T);
        return initialTransferBlockData
        (
            charSpan(src), 
            charSpan(dst), 
            el
        );
    }*/  



    /// @brief Number of points to be imported after updating domains 
	/// @return number of points
	virtual
	uint32 numberToBeImported()const = 0;

    virtual 
    uint32 numberToBeExported()const = 0;

	

	virtual 
	bool requiresDataTransfer() const = 0;

	inline
	const dictionary& boundaryDict(uint32 i)const
	{
		return thisBoundaryDict().subDict(bundaryName(i));
	}
	
	inline
	const auto& boundaryPlane(uint32 i)const
	{
		return thisDomain_.boundaryPlane(i);
	}

	static inline
	const word& bundaryName(uint32 i)
	{
		return boundaryNames_[i];
	}

	static inline 
	uint32 sizeOfBoundaries() 
	{
		return boundaryNames_.size();
	}
	
	static
	uniquePtr<simulationDomain> create(const dictionary& dict);

}; // simulationDomain



}


#endif //__simulationDomain_hpp__
