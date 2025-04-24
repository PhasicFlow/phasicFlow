#include "lineRegionPoints.hpp"
#include "fieldsDataBase.hpp"

namespace pFlow::postprocessData
{

lineRegionPoints::lineRegionPoints
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDataBase
)
:
    regionPoints(dict, fieldsDataBase),
    line_(dict.subDict("lineInfo")),
    centerPoints_("centerPoints"),
    volumes_("volumes"),
    selectedPoints_("selectedPoints")
{
    const auto& lDict = dict.subDict("lineInfo");
    uint32 nSpheres = lDict.getValMax<uint32>("nSpheres",2);
    realList raddi;

    if( lDict.containsDataEntry("radii"))
    {
        raddi = lDict.getVal<realList>("radii");
    }
    else
    {
        auto r = lDict.getVal<real>("radius");
        raddi = realList(nSpheres, r);
    }

    if(raddi.size() != nSpheres)
    {
        fatalErrorInFunction
            << "The number of elements in the radii list should be equal to the "
            << "nSpheres"<<endl;
            fatalExit;
    }
    
    sphereRegions_.resize(nSpheres, sphere(realx3(0,0,0),1));
    centerPoints_.resize(nSpheres);
    volumes_.resize(nSpheres);
    diameters_.resize(nSpheres);
    selectedPoints_.resize(nSpheres);
    real dt = 1.0/(nSpheres-1);
    for(uint32 i = 0; i < nSpheres; ++i)
    {
        centerPoints_[i] = line_.point(i*dt);
        sphereRegions_[i] = pFlow::sphere(centerPoints_[i], raddi[i]);
        volumes_[i] = sphereRegions_[i].volume();
        diameters_[i] = 2*sphereRegions_[i].radius();
    }
}

pFlow::span<const pFlow::uint32> lineRegionPoints::indices(uint32 elem) const
{
    if(elem >= size())
    {
        fatalErrorInFunction
            << "The element index is out of range. elem: " << elem
            << " size: " << size() << endl;
        fatalExit;
    }
    
    return span<const uint32>(
        selectedPoints_[elem].data(), 
        selectedPoints_[elem].size());
}

pFlow::span<pFlow::uint32> lineRegionPoints::indices(uint32 elem)
{
    if(elem >= size())
    {
        fatalErrorInFunction
            << "The element index is out of range. elem: " << elem
            << " size: " << size() << endl;
        fatalExit;
    }
    
    return span<uint32>(
        selectedPoints_[elem].data(), 
        selectedPoints_[elem].size());
}

bool lineRegionPoints::update()
{
    const auto points = database().updatePoints();
    for(auto& elem : selectedPoints_)
    {
        elem.clear();
    }

    for(uint32 i = 0; i < points.size(); ++i)
    {
        for(uint32 j = 0; j < sphereRegions_.size(); ++j)
        {
            if( sphereRegions_[j].isInside(points[i]))
            {
                selectedPoints_[j].push_back(i);
            }
        }
    }
    return true;
}

bool lineRegionPoints::write(iOstream &os) const
{
    os << "# Spheres along a straight line \n";
    os << "#  No." << tab << "centerPoint" << tab << "diameter" << endl;
    for(uint32 i=0; i < sphereRegions_.size(); ++i)
    {
        os << "# " << i << tab << sphereRegions_[i].center() 
           << tab << diameters_[i] << '\n';
    }
    os << "time/No. ";
    for(uint32 i=0; i < sphereRegions_.size(); ++i)
    {
        os << i << tab;
    }
    os << endl;
    return true;
}

} // End namespace pFlow::postprocessData
