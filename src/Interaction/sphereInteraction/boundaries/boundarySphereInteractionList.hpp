#ifndef __boundarySphereInteractionList_hpp__
#define __boundarySphereInteractionList_hpp__


#include "boundaryList.hpp"
#include "boundaryListPtr.hpp"
#include "boundarySphereInteraction.hpp"


namespace pFlow
{


template<typename contactForceModel,typename geometryMotionModel>
class boundarySphereInteractionList
:
    public boundaryListPtr<boundarySphereInteraction<contactForceModel,geometryMotionModel>>
{
private:

    const boundaryList&         boundaries_;

public:

    boundarySphereInteractionList(
        const sphereParticles& sphPrtcls,
		const geometryMotionModel& geomMotion
    );

    ~boundarySphereInteractionList()=default;

};



}

#include "boundarySphereInteractionList.cpp"

#endif //__boundarySphereInteractionList_hpp__