

template <typename CFModel, typename gMModel>
pFlow::boundarySphereInteractionList<CFModel, gMModel>::boundarySphereInteractionList
(
    const sphereParticles &sphPrtcls, 
    const gMModel &geomMotion
)
:
	ListPtr<boundarySphereInteraction<CFModel,gMModel>>(6),
	boundaries_(sphPrtcls.pStruct().boundaries())
{
	//gSettings::sleepMiliSeconds(1000*pFlowProcessors().localRank());
	for(uint32 i=0; i<6; i++)
	{
		this->set(
			i,
			boundarySphereInteraction<CFModel, gMModel>::create(
				boundaries_[i],
				sphPrtcls, 
				geomMotion));
	}	
}