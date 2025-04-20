#include "sphereRegionPoints.hpp"
#include "fieldsDataBase.hpp"

pFlow::sphereRegionPoints::sphereRegionPoints
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDataBase
)
:
    regionPoints(dict, fieldsDataBase),
    sphereRegion_(dict.subDict("sphereInfo")),
    volume_(sphereRegion_.volume()),
    diameter_(2*sphereRegion_.radius()),
    selectedPoints_("selectedPoints")
{
}

bool pFlow::sphereRegionPoints::update()
{
    const auto points = database().updatePoints();
    selectedPoints_.clear();
    for(uint32 i = 0; i < points.size(); ++i)
    {
        if( sphereRegion_.isInside(points[i]))
        {
            selectedPoints_.push_back(i);
        }
    }
 
    return true;
}

bool pFlow::sphereRegionPoints::write(iOstream &os) const
{
    os <<"# Single sphere\n";
    os <<"# center point: "<<sphereRegion_.center()<<endl;
    os <<"# diameter: "<< diameter_ << endl;
    os <<"time"<< tab <<"value"<<endl;
    
    return true;
}
