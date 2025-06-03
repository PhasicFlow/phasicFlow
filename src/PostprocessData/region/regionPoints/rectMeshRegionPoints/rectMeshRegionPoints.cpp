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

    auto minP = rectMeshInfo.getVal<realx3>("min");
    auto maxP = rectMeshInfo.getVal<realx3>("max");
    auto nx = rectMeshInfo.getValMax<uint32>("nx", 1);
    auto ny = rectMeshInfo.getValMax<uint32>("ny", 1);
    auto nz = rectMeshInfo.getValMax<uint32>("nz", 1);
    
    boxRegion_ = box(minP, maxP);
    cells_ = uint32x3(nx, ny, nz);
    uint32 nCells = nx * ny * nz;
    
    volumes_.resize(nCells, boxRegion_.volume() / nCells);
    diameter_.resize(nCells, 2 * pow(3 * boxRegion_.volume() / nCells / 4.0 / Pi, 1.0 / 3.0));
    selectedPoints_.resize(nCells);
    centerPoints_.resize(nCells);

    
    for(uint32 i = 0; i < nx; ++i)
    {
        for(uint32 j = 0; j < ny; ++j)
        {
            for(uint32 k = 0; k < nz; ++k)
            {
                // calculate the center point of each cell
                uint32 index = (i * ny + j) * nz + k;
                realx3 center = boxRegion_.minPoint() + 
                    realx3(
                        (i + 0.5) * (boxRegion_.maxPoint().x() - boxRegion_.minPoint().x()) / nx,
                        (j + 0.5) * (boxRegion_.maxPoint().y() - boxRegion_.minPoint().y()) / ny,
                        (k + 0.5) * (boxRegion_.maxPoint().z() - boxRegion_.minPoint().z()) / nz
                    );
                centerPoints_[index] = center;
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

    real dx = (boxRegion_.maxPoint().x() - boxRegion_.minPoint().x()) / cells_.x();
    real dy = (boxRegion_.maxPoint().y() - boxRegion_.minPoint().y()) / cells_.y();
    real dz = (boxRegion_.maxPoint().z() - boxRegion_.minPoint().z()) / cells_.z();

    for (uint32 i = 0; i < points.size(); ++i)
    {
        if(boxRegion_.isInside(points[i]))
        {
            uint indexX = (points[i] - boxRegion_.minPoint()).x() / dx;
            uint indexY = (points[i] - boxRegion_.minPoint()).y() / dy;
            uint indexZ = (points[i] - boxRegion_.minPoint()).z() / dz;
            uint cellIndex = (indexX * cells_.y() + indexY) * cells_.z() + indexZ;
            selectedPoints_[cellIndex].push_back(i);
        }
    }
    return true;
}

bool rectMeshRegionPoints::write(iOstream &os) const
{
    os << "# rect mesh region points\n";
    os <<"# min point: "<< boxRegion_.minPoint() << endl;
    os <<"# max point: "<< boxRegion_.maxPoint() << endl;
    os <<"# nx: "<< cells_.x() <<endl;
    os <<"# ny: "<< cells_.y() <<endl;
    os <<"# nz: "<< cells_.z() <<endl;
    os << "time/No. ";
    for (uint32 i = 0; i < cells_.x() * cells_.y() * cells_.z(); ++i)
    {
        os << i << "  ";
    }
    os << endl;
    return true;
}

} // End namespace pFlow::postprocessData
