#include "processorBoundarySphereParticles.hpp"
#include "sphereParticles.hpp"
#include "boundaryProcessor.hpp"

pFlow::processorBoundarySphereParticles::processorBoundarySphereParticles(
    const boundaryBase &boundary, 
    sphereParticles &prtcls
)
:
    boundarySphereParticles(boundary, prtcls)
{

}

bool pFlow::processorBoundarySphereParticles::acceleration(const timeInfo &ti, const realx3& g)
{
    

#ifndef BoundaryModel1
    

    if(isBoundaryMaster())
    {
        auto thisIndex = thisBoundaryIndex();
        auto mass = Particles().mass().BoundaryField(thisIndex).neighborProcField().deviceView();
        auto I = Particles().I().BoundaryField(thisIndex).neighborProcField().deviceView();
        auto cf = Particles().contactForce().BoundaryField(thisIndex).neighborProcField().deviceView();
        auto ct = Particles().contactTorque().BoundaryField(thisIndex).neighborProcField().deviceView();
        auto acc = Particles().accelertion().BoundaryField(thisIndex).neighborProcField().deviceView();
        auto rAcc = Particles().rAcceleration().BoundaryField(thisIndex).neighborProcField().deviceView();

        Kokkos::parallel_for(
            "processorBoundary::acceleration."+this->boundaryName(),
            deviceRPolicyStatic(0,mass.size()),
            LAMBDA_HD(uint32 i){
                acc[i] = cf[i]/mass[i] + g;
                rAcc[i] = ct[i]/I[i];
            });
        Kokkos::fence();    
    }

#endif
    

    return true;
}
