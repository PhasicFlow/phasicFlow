#include "multipleSpheresRegionPoints.hpp"
#include "fieldsDataBase.hpp"

namespace pFlow::postprocessData
{

multipleSpheresRegionPoints::multipleSpheresRegionPoints
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDataBase
)
:
    regionPoints(dict, fieldsDataBase)
{
    const auto& multiSphereInfo = dict.subDict("multipleSphereInfo");
    
    // Read centers and radii lists
    auto centers = multiSphereInfo.getVal<List<realx3>>("centers");
    auto radii = multiSphereInfo.getVal<List<real>>("radii");
    
    // Check if lists have the same length
    if(centers.size() != radii.size())
    {
        fatalErrorInFunction
            << "The number of centers (" << centers.size() 
            << ") does not match the number of radii (" << radii.size() << ")"
            << endl;
        fatalExit;
    }
    
    uint32 nSpheres = centers.size();
    
    // Initialize data structures
    sphereRegions_.resize(nSpheres, sphere(realx3(0.0, 0.0, 0.0), 1.0));
    centerPoints_.resize(nSpheres);
    diameters_.resize(nSpheres);
    volumes_.resize(nSpheres);
    selectedPoints_.resize(nSpheres);

    // Setup each sphere
    for (uint32 i = 0; i < nSpheres; ++i)
    {
        real diameter = 2.0 * radii[i]; // Convert radius to diameter
        
        sphereRegions_[i] = pFlow::sphere(centers[i], radii[i]);
        centerPoints_[i] = centers[i];
        diameters_[i] = diameter;
        volumes_[i] = sphereRegions_[i].volume();
    }
}

pFlow::span<const pFlow::uint32> multipleSpheresRegionPoints::indices(uint32 elem) const
{
    if (elem >= size())
    {
        fatalErrorInFunction
            << "The element index is out of range. elem: " << elem
            << " size: " << size() << endl;
        fatalExit;
    }

    return span<const uint32>(selectedPoints_[elem].data(), selectedPoints_[elem].size());
}

pFlow::span<pFlow::uint32> multipleSpheresRegionPoints::indices(uint32 elem)
{
    if (elem >= size())
    {
        fatalErrorInFunction
            << "The element index is out of range. elem: " << elem
            << " size: " << size() << endl;
        fatalExit;
    }

    return span<uint32>(selectedPoints_[elem].data(), selectedPoints_[elem].size());
}


bool multipleSpheresRegionPoints::update()
{
    const auto points = database().updatePoints();
    for (auto& elem : selectedPoints_)
    {
        elem.clear();
    }

    for (uint32 i = 0; i < points.size(); ++i)
    {
        for (uint32 j = 0; j < sphereRegions_.size(); ++j)
        {
            if (sphereRegions_[j].isInside(points[i]))
            {
                selectedPoints_[j].push_back(i);
            }
        }
    }
    return true;
}

bool multipleSpheresRegionPoints::write(iOstream &os) const
{
    os << "# Multiple spheres region points\n";
    os << "#  No." << tab << "centerPoint" << tab << "diameter" << endl;
    for (uint32 i = 0; i < sphereRegions_.size(); ++i)
    {
        os << "# " << i << tab << sphereRegions_[i].center() << tab << diameters_[i] << '\n';
    }
    os << "time/No. ";
    for (uint32 i = 0; i < sphereRegions_.size(); ++i)
    {
        os << i << "  ";
    }
    os << endl;
    return true;
}

} // End namespace pFlow::postprocessData
