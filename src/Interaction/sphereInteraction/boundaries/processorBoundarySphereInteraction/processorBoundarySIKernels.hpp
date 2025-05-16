
#ifndef __processorBoundarySIKernels_hpp__
#define __processorBoundarySIKernels_hpp__

namespace pFlow::MPI::processorBoundarySIKernels
{

template<typename ContactListType, typename ContactForceModel>
inline
void sphereSphereInteraction
(
    const word& kernalName,
    real dt,
    const ContactListType&              cntctList,
    const ContactForceModel&            forceModel,
    const deviceScatteredFieldAccess<realx3>& thisPoints,
    const deviceViewType1D<real>& thisDiam,
    const deviceViewType1D<uint32>& thisPropId,
    const deviceViewType1D<realx3>& thisVel,
    const deviceViewType1D<realx3>& thisRVel,
    const deviceViewType1D<realx3>& thisCForce,
    const deviceViewType1D<realx3>& thisCTorque,
    const deviceViewType1D<realx3>& neighborPoints,
    const deviceViewType1D<real>& neighborDiam,
    const deviceViewType1D<uint32>& neighborPropId,
    const deviceViewType1D<realx3>& neighborVel,
    const deviceViewType1D<realx3>& neighborRVel,
    const deviceViewType1D<realx3>& neighborCForce,
    const deviceViewType1D<realx3>& neighborCTorque
)
{
    
    using ValueType = typename ContactListType::ValueType;
    uint32 ss = cntctList.size();
    if(ss == 0u)return;

    uint32 lastItem = cntctList.loopCount();

    Kokkos::parallel_for(
        kernalName,
        deviceRPolicyDynamic(0,lastItem),
        LAMBDA_HD(uint32 n)
        {

        if(!cntctList.isValid(n))return;

            auto [i,j] = cntctList.getPair(n);
            uint32 ind_i = thisPoints.index(i);
            uint32 ind_j = j;

            real Ri = 0.5*thisDiam[ind_i];
            real Rj = 0.5*neighborDiam[ind_j];
            realx3 xi = thisPoints.field()[ind_i];
            realx3 xj = neighborPoints[ind_j];

            real dist = length(xj-xi);
            real ovrlp = (Ri+Rj) - dist;
            
            if( ovrlp >0.0 )
            {
                auto Nij = (xj-xi)/max(dist,smallValue);
                auto wi = thisRVel[ind_i];
                auto wj = neighborRVel[ind_j];
                auto Vr = thisVel[ind_i] - neighborVel[ind_j] + cross((Ri*wi+Rj*wj), Nij);
                
                auto history = cntctList.getValue(n);

                int32 propId_i = thisPropId[ind_i];
                int32 propId_j = neighborPropId[ind_j];

                realx3 FCn, FCt, Mri, Mrj, Mij, Mji;

                // calculates contact force 
                forceModel.contactForce(
                    dt, i, j,
                    propId_i, propId_j,
                    Ri, Rj,
                    ovrlp,
                    Vr, Nij,
                    history,
                    FCn, FCt);

                forceModel.rollingFriction(
                    dt, i, j,
                    propId_i, propId_j,
                    Ri, Rj,
                    wi, wj,
                    Nij,
                    FCn,
                    Mri, Mrj);

                auto M = cross(Nij,FCt);
                Mij = Ri*M+Mri;
                Mji = Rj*M+Mrj;
                
                auto FC = FCn + FCt;
                

                Kokkos::atomic_add(&thisCForce[ind_i].x_,FC.x_);
                Kokkos::atomic_add(&thisCForce[ind_i].y_,FC.y_);
                Kokkos::atomic_add(&thisCForce[ind_i].z_,FC.z_);

                Kokkos::atomic_add(&neighborCForce[ind_j].x_,-FC.x_);
                Kokkos::atomic_add(&neighborCForce[ind_j].y_,-FC.y_);
                Kokkos::atomic_add(&neighborCForce[ind_j].z_,-FC.z_);

                Kokkos::atomic_add(&thisCTorque[ind_i].x_, Mij.x_);
                Kokkos::atomic_add(&thisCTorque[ind_i].y_, Mij.y_);
                Kokkos::atomic_add(&thisCTorque[ind_i].z_, Mij.z_);

                Kokkos::atomic_add(&neighborCTorque[ind_j].x_, Mji.x_);
                Kokkos::atomic_add(&neighborCTorque[ind_j].y_, Mji.y_);
                Kokkos::atomic_add(&neighborCTorque[ind_j].z_, Mji.z_);
                

                cntctList.setValue(n,history);

            }
            else
            {
                cntctList.setValue(n, ValueType());
            }

        });
    Kokkos::fence();
}


} //pFlow::MPI::processorBoundarySIKernels


#endif //__processorBoundarySIKernels_hpp__