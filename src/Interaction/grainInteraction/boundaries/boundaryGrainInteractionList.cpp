

template <typename CFModel, typename gMModel>
pFlow::boundaryGrainInteractionList<CFModel, gMModel>::boundaryGrainInteractionList
(
    const grainParticles &grnPrtcls, 
    const gMModel &geomMotion
)
:
	ListPtr<boundaryGrainInteraction<CFModel,gMModel>>(6),
	boundaries_(grnPrtcls.pStruct().boundaries())
{
	//gSettings::sleepMiliSeconds(1000*pFlowProcessors().localRank());
	for(uint32 i=0; i<6; i++)
	{
		this->set(
			i,
			boundaryGrainInteraction<CFModel, gMModel>::create(
				boundaries_[i],
				grnPrtcls, 
				geomMotion));
	}	
}