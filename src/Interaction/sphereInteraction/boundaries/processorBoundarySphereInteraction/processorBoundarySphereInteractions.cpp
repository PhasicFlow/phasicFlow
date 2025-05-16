
#include "processorBoundarySphereInteraction.hpp"
#include "geometryMotions.hpp"
#include "contactForceModels.hpp"


template class pFlow::MPI::processorBoundarySphereInteraction
<
    pFlow::cfModels::limitedNonLinearNormalRolling,
    pFlow::rotationAxisMotionGeometry
>;

template class pFlow::MPI::processorBoundarySphereInteraction
<
    pFlow::cfModels::nonLimitedNonLinearNormalRolling,
    pFlow::rotationAxisMotionGeometry
>;