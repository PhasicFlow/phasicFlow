
#ifndef __regularSimulationDomain_hpp__
#define __regularSimulationDomain_hpp__

#include "simulationDomain.hpp"

namespace pFlow
{

class regularSimulationDomain
:
	public simulationDomain
{
protected:

	uint32 		initialNumPoints_=0;

	bool createBoundaryDicts() override;

	bool setThisDomain()override;

public:

	TypeInfo("simulationDomain<regular>");

	regularSimulationDomain(const dictionary& dict);

	virtual
	~regularSimulationDomain()=default;
	
	add_vCtor
	(
		simulationDomain,
		regularSimulationDomain,
		dictionary
	);

    bool initialUpdateDomains(span<realx3> pointPos) override;

    uint32 initialNumberInThis()const override;

	/*bool updateDomains(
		span<realx3> pointPos,
        pFlagTypeHost flags) override;*/
   
	uint32 numberToBeImported()const override;

	uint32 numberToBeExported()const override;

	bool initialTransferBlockData(
		span<char> src, 
		span<char> dst,
		size_t sizeOfElement) override;


    /// @brief 
    /// @param src 
    /// @param dst 
    /// @return 
    bool initialTransferBlockData(
        span<realx3> src,
        span<realx3> dst) override;

    bool initialTransferBlockData(
        span<real> src,
        span<real> dst) override;

    bool initialTransferBlockData(
        span<uint32> src,
        span<uint32> dst) override;

    bool initialTransferBlockData(
        span<int32> src,
        span<int32> dst) override;
    
	
	const dictionary& thisBoundaryDict()const override;

	bool requiresDataTransfer() const override;
};

}

#endif //__regularSimulationDomain_hpp__
