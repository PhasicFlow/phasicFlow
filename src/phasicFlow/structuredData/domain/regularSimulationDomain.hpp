
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

	uint32 		thisNumPoints_=0;

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

	bool updateDomains( 
		const realx3Vector& pointPos)override;

	uint32 thisNumPoints()const override;

	 
	bool transferBlockData(
		span<char> src, 
		span<char> dst,
		uint32 sizeOfBlock) override;
	
	const dictionary& thisBoundaryDict()const override;

	bool requiresDataTransfer() const override;
};

}

#endif //__regularSimulationDomain_hpp__
