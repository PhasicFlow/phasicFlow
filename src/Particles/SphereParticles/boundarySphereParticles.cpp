#include "boundarySphereParticles.hpp"
#include "boundaryBase.hpp"
#include "sphereParticles.hpp"


pFlow::boundarySphereParticles::boundarySphereParticles(
    const boundaryBase &boundary, 
    sphereParticles &prtcls
)
:
    generalBoundary(boundary, prtcls.pStruct(), "", ""),
    particles_(prtcls)
{

}

pFlow::sphereParticles &pFlow::boundarySphereParticles::Particles()
{
    return particles_;
}

const pFlow::sphereParticles &pFlow::boundarySphereParticles::Particles() const
{
    return particles_;
}

pFlow::uniquePtr<pFlow::boundarySphereParticles> pFlow::boundarySphereParticles::create(
    const boundaryBase &boundary, 
    sphereParticles &prtcls
)
{
	
	word bType = angleBracketsNames2( 
		"boundarySphereParticles",
		pFlowProcessors().localRunTypeName(), 
		boundary.type());

	word altBType{"boundarySphereParticles<none>"};

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
