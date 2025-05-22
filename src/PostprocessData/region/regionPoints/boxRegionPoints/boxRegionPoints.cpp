#include "boxRegionPoints.hpp"
#include "fieldsDataBase.hpp"
#include "numericConstants.hpp"

namespace pFlow::postprocessData
{

boxRegionPoints::boxRegionPoints
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDataBase
)
:
    regionPoints(dict, fieldsDataBase),
    boxRegion_(dict.subDict("boxInfo")),
    volume_
    (
        (boxRegion_.maxPoint().x() - boxRegion_.minPoint().x()) * 
        (boxRegion_.maxPoint().y() - boxRegion_.minPoint().y()) * 
        (boxRegion_.maxPoint().z() - boxRegion_.minPoint().z())
    ),
    diameter_(2 * pow(3 * volume_ / 4.0 / Pi, 1.0 / 3.0)),
    selectedPoints_("selectedPoints")
{
}

bool boxRegionPoints::update()
{
    const auto points = database().updatePoints();
    selectedPoints_.clear();
    for(uint32 i = 0; i < points.size(); ++i)
    {
        if( boxRegion_.isInside(points[i]))
        {
            selectedPoints_.push_back(i);
        }
    }
 
    return true;
}

bool boxRegionPoints::write(iOstream &os) const
{
    os <<"# Single box\n";
    os <<"# min point: "<< boxRegion_.minPoint() <<endl;
    os <<"# max point: "<< boxRegion_.maxPoint() << endl;
    os <<"time"<< tab <<"value"<<endl;
    
    return true;
}

} // End namespace pFlow::postprocessData
