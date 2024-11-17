#ifndef __boundaryGrainInteractionList_hpp__
#define __boundaryGrainInteractionList_hpp__


#include "boundaryList.hpp"
#include "ListPtr.hpp"
#include "boundaryGrainInteraction.hpp"


namespace pFlow
{


template<typename contactForceModel,typename geometryMotionModel>
class boundaryGrainInteractionList
:
    public ListPtr<boundaryGrainInteraction<contactForceModel,geometryMotionModel>>
{
private:

    const boundaryList&         boundaries_;

public:

    boundaryGrainInteractionList(
        const grainParticles& grnPrtcls,
		const geometryMotionModel& geomMotion
    );

    ~boundaryGrainInteractionList()=default;

};



}

#include "boundaryGrainInteractionList.cpp"

#endif //__boundaryGrainInteractionList_hpp__