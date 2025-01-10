#include "boundaryIntegration.hpp"
#include "pointStructure.hpp"


pFlow::boundaryIntegration::boundaryIntegration(
    const boundaryBase &boundary, 
    const pointStructure &pStruct,  
    const word &method,
    integration& intgrtn
)
:
    generalBoundary(boundary, pStruct, "", method),
    integration_(intgrtn)
{}

pFlow::uniquePtr<pFlow::boundaryIntegration> pFlow::boundaryIntegration::create(
    const boundaryBase &boundary, 
    const pointStructure &pStruct,  
    const word &method,
    integration& intgrtn
)
{

    word bType = angleBracketsNames2( 
		"boundaryIntegration",
		boundary.type(),
        method);

	word altBType{"boundaryIntegration<none>"};

	if( boundaryBasevCtorSelector_.search(bType) )
	{
		pOutput.space(4)<<"Creating boundary "<< Green_Text(bType)<<
		" for "<<boundary.name()<<endl;
		return boundaryBasevCtorSelector_[bType](boundary, pStruct, method, intgrtn);
	}
	else if(boundaryBasevCtorSelector_.search(altBType))
	{
		pOutput.space(4)<<"Creating boundary "<< Green_Text(altBType)<<
		" for "<<boundary.name()<<endl;
		return boundaryBasevCtorSelector_[altBType](boundary, pStruct, method, intgrtn);
	}
	else
	{
		printKeys( 
			fatalError << "Ctor Selector "<< bType<<
			" and "<< altBType << " do not exist. \n"
			<<"Avaiable ones are: \n",
			boundaryBasevCtorSelector_
		);
		fatalExit;
	}
	
	return nullptr;
}
