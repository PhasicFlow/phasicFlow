#include "rectMeshRegionPoints.hpp"
#include "fieldsDataBase.hpp"
#include "numericConstants.hpp"

namespace pFlow::postprocessData
{

rectMeshRegionPoints::rectMeshRegionPoints
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDataBase
)
:
    regionPoints(dict, fieldsDataBase)
{
    const auto& rectMeshInfo = dict.subDict("rectMeshInfo");

    auto min = rectMeshInfo.getVal<realx3>("min");
    auto max = rectMeshInfo.getVal<realx3>("max");
    auto nx = rectMeshInfo.getVal<uint32>("nx");
    auto ny = rectMeshInfo.getVal<uint32>("ny");
    auto nz = rectMeshInfo.getVal<uint32>("nz");
    
    uint32 nMeshes = nx * ny * nz;
    
    // Initialize data structures
    boxRegions_.resize(nMeshes, box(realx3(0.0, 0.0, 0.0), realx3(1.0, 1.0, 1.0)));
    minPoints_.resize(nMeshes);
    maxPoints_.resize(nMeshes);
    volumes_.resize(nMeshes);
    selectedPoints_.resize(nMeshes);

    // Setup each sphere
    for (uint32 i = 0; i < nx; ++i)
    {
        for(uint32 j = 0; j < ny; ++j)
        {
            for(uint32 k = 0; k < nz; ++k)
            {
                auto tempMinX = min.x() + i * (max.x() - min.x()) / nx;
                auto tempMinY = min.y() + j * (max.y() - min.y()) / ny; 
                auto tempMinZ = min.z() + k * (max.z() - min.z()) / nz; 

                auto tempMaxX = min.x() + (i + 1) * (max.x() - min.x()) / nx;
                auto tempMaxY = min.y() + (j + 1) * (max.y() - min.y()) / ny; 
                auto tempMaxZ = min.z() + (k + 1) * (max.z() - min.z()) / nz;

                minPoints_[i * nx + j * ny + k] = realx3(tempMinX, tempMinY, tempMinZ);
                maxPoints_[i * nx + j * ny + k] = realx3(tempMaxX, tempMaxY, tempMaxZ);
                boxRegions_[i * nx + j * ny + k] = pFlow::box(minPoints_[i * nx + j * ny + k], maxPoints_[i * nx + j * ny + k]);
                volumes_[i * nx + j * ny + k] = (tempMaxX - tempMinX) * (tempMaxY - tempMinY) * (tempMaxZ - tempMinZ);
            }
        }
    }
}

bool rectMeshRegionPoints::update()
{
    const auto points = database().updatePoints();
    for (auto& elem : selectedPoints_)
    {
        elem.clear();
    }

    for (uint32 i = 0; i < points.size(); ++i)
    {
        for (uint32 j = 0; j < boxRegions_.size(); ++j)
        {
            if (boxRegions_[j].isInside(points[i]))
            {
                selectedPoints_[j].push_back(i);
            }
        }
    }
    return true;
}

bool rectMeshRegionPoints::write(iOstream &os) const
{
    os << "# rect mesh region points\n";
    os << "#  No." << tab << "minPoint" << tab << "maxPoint" << endl;
    for (uint32 i = 0; i < boxRegions_.size(); ++i)
    {
        os << "# " << i << tab << boxRegions_[i].minPoint() << tab << boxRegions_[i].maxPoint() << '\n';
    }
    os << "time/No. ";
    for (uint32 i = 0; i < boxRegions_.size(); ++i)
    {
        os << i << "  ";
    }
    os << endl;
    return true;
}

} // End namespace pFlow::postprocessData
