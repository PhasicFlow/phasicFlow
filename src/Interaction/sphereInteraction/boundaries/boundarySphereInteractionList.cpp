

template <typename CFModel, typename gMModel>
pFlow::boundarySphereInteractionList<CFModel, gMModel>::boundarySphereInteractionList
(
    const sphereParticles &sphPrtcls, 
    const gMModel &geomMotion
)
:
	boundaryListPtr<boundarySphereInteraction<CFModel,gMModel>>(),
	boundaries_(sphPrtcls.pStruct().boundaries())
{
	output<<boundaries_.size()<<endl;
	ForAllBoundariesPtr(i, this)
	{
		this->set(
			i,
			boundarySphereInteraction<CFModel, gMModel>::create(
				boundaries_[i],
				sphPrtcls, 
				geomMotion));
	}	
}