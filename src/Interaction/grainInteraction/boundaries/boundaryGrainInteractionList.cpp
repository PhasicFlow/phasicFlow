

template <typename CFModel, typename gMModel>
pFlow::boundaryGrainInteractionList<CFModel, gMModel>::boundaryGrainInteractionList
(
    const grainParticles &grnPrtcls, 
    const gMModel &geomMotion
)
:
	boundaryListPtr<boundaryGrainInteraction<CFModel,gMModel>>(),
	boundaries_(grnPrtcls.pStruct().boundaries())
{
	//gSettings::sleepMiliSeconds(1000*pFlowProcessors().localRank());
	ForAllBoundariesPtr(i, this)
	{
		this->set(
			i,
			boundaryGrainInteraction<CFModel, gMModel>::create(
				boundaries_[i],
				grnPrtcls, 
				geomMotion));
	}	
}