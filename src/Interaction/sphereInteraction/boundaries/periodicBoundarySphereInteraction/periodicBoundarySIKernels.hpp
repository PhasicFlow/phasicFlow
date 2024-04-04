
namespace pFlow::periodicBoundarySIKernels
{

template<typename ContactListType, typename ContactForceModel>
void sphereSphereInteraction
(
    real dt,
    const ContactListType&              cntctList,
    const ContactForceModel&            forceModel,
    const realx3&                       transferVec,
    const deviceScatteredFieldAccess<realx3>& thisPoints,
    const deviceScatteredFieldAccess<realx3>& mirrorPoints,
    const deviceViewType1D<real>& diam,
    const deviceViewType1D<uint32>& propId,
    const deviceViewType1D<realx3>& vel,
    const deviceViewType1D<realx3>& rVel,
    const deviceViewType1D<realx3>& cForce,
    const deviceViewType1D<realx3>& cTorque
)
{
    
    using ValueType = typename ContactListType::ValueType;
    uint32 ss = cntctList.size();
    uint32 lastItem = cntctList.loopCount();
    if(lastItem == 0u)return;

    Kokkos::parallel_for(
        "pFlow::periodicBoundarySIKernels::sphereSphereInteraction",
        deviceRPolicyDynamic(0,lastItem),
        LAMBDA_HD(uint32 n)
        {

        if(!cntctList.isValid(n))return;

            auto [i,j] = cntctList.getPair(n);
            uint32 ind_i = thisPoints.index(i);
            uint32 ind_j = mirrorPoints.index(j);

            real Ri = 0.5*diam[ind_i];
            real Rj = 0.5*diam[ind_j];
            realx3 xi = thisPoints.field()[ind_i];
            realx3 xj = mirrorPoints.field()[ind_j]+transferVec;
            real dist = length(xj-xi);
            real ovrlp = (Ri+Rj) - dist;
            
            if( ovrlp >0.0 )
            {
                
                /*auto Vi = thisVel[i];
                auto Vj = mirrorVel[j];
                auto wi = thisRVel[i];
                auto wj = mirrorRVel[j];
                auto Nij = (xj-xi)/dist;
                auto Vr = Vi - Vj + cross((Ri*wi+Rj*wj), Nij);*/

                auto Nij = (xj-xi)/dist;
                auto wi = rVel[ind_i];
                auto wj = rVel[ind_j];
                auto Vr = vel[ind_i] - vel[ind_j] + cross((Ri*wi+Rj*wj), Nij);
                
                auto history = cntctList.getValue(n);

                int32 propId_i = propId[ind_i];
                int32 propId_j = propId[ind_j];

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
                

                Kokkos::atomic_add(&cForce[ind_i].x_,FC.x_);
                Kokkos::atomic_add(&cForce[ind_i].y_,FC.y_);
                Kokkos::atomic_add(&cForce[ind_i].z_,FC.z_);

                Kokkos::atomic_add(&cForce[ind_j].x_,-FC.x_);
                Kokkos::atomic_add(&cForce[ind_j].y_,-FC.y_);
                Kokkos::atomic_add(&cForce[ind_j].z_,-FC.z_);

                Kokkos::atomic_add(&cTorque[ind_i].x_, Mij.x_);
                Kokkos::atomic_add(&cTorque[ind_i].y_, Mij.y_);
                Kokkos::atomic_add(&cTorque[ind_i].z_, Mij.z_);

                Kokkos::atomic_add(&cTorque[ind_j].x_, Mji.x_);
                Kokkos::atomic_add(&cTorque[ind_j].y_, Mji.y_);
                Kokkos::atomic_add(&cTorque[ind_j].z_, Mji.z_);
                

                cntctList.setValue(n,history);

            }
            else
            {
                cntctList.setValue(n, ValueType());
            }

        });
    Kokkos::fence();
}


} //pFlow::periodicBoundarySIKernels