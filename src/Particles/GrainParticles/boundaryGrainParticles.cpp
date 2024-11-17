#include "boundaryGrainParticles.hpp"
#include "boundaryBase.hpp"
#include "grainParticles.hpp"


pFlow::boundaryGrainParticles::boundaryGrainParticles(
    const boundaryBase &boundary, 
    grainParticles &prtcls
)
:
    generalBoundary(boundary, prtcls.pStruct(), "", ""),
    particles_(prtcls)
{

}

pFlow::grainParticles &pFlow::boundaryGrainParticles::Particles()
{
    return particles_;
}

const pFlow::grainParticles &pFlow::boundaryGrainParticles::Particles() const
{
    return particles_;
}

pFlow::uniquePtr<pFlow::boundaryGrainParticles> pFlow::boundaryGrainParticles::create(
    const boundaryBase &boundary, 
    grainParticles &prtcls
)
{
	
	word bType = angleBracketsNames2( 
		"boundaryGrainParticles",
		pFlowProcessors().localRunTypeName(), 
		boundary.type());

	word altBType{"boundaryGrainParticles<none>"};

	if( boundaryBasevCtorSelector_.search(bType) )
	{
		pOutput.space(4)<<"Creating boundary "<< Green_Text(bType)<<
		" for "<<boundary.name()<<endl;
		return boundaryBasevCtorSelector_[bType](boundary, prtcls);
	}
	else if(boundaryBasevCtorSelector_.search(altBType))
	{
		pOutput.space(4)<<"Creating boundary "<< Green_Text(altBType)<<
		" for "<<boundary.name()<<endl;
		return boundaryBasevCtorSelector_[altBType](boundary, prtcls);
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
