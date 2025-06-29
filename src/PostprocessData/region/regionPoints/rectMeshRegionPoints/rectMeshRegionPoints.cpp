#include "rectMeshRegionPoints.hpp"
#include "fieldsDataBase.hpp"
#include "numericConstants.hpp"


pFlow::postprocessData::rectMeshRegionPoints::rectMeshRegionPoints
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDataBase
)
:
    regionPoints(dict, fieldsDataBase)
{
    const auto& rectMeshInfo = dict.subDict("rectMeshInfo");
    boxRegion_ = box(rectMeshInfo.subDict("boxInfo"));
    nx = rectMeshInfo.getValMax<uint32>("nx", 1);
    ny = rectMeshInfo.getValMax<uint32>("ny", 1);
    nz = rectMeshInfo.getValMax<uint32>("nz", 1);
    
    // boxRegion_ = box(minP, maxP);
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

bool pFlow::postprocessData::rectMeshRegionPoints::update()
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

bool pFlow::postprocessData::rectMeshRegionPoints::write(iOstream &os) const
{
    os << "# vtk DataFile Version 3.0" << endl;
    os << "postProcessData" << endl;
    os << "ASCII" << endl;
    os << "DATASET RECTILINEAR_GRID" << endl;
	os << "DIMENSIONS " << nx + 1 << " " << ny + 1 << " " << nz + 1 << endl;
		
    auto [x, y , z] = boxRegion_.minPoint();
    auto [dx, dy, dz] = (boxRegion_.maxPoint() - boxRegion_.minPoint()) / realx3(nx, ny,  nz);

    os << "X_COORDINATES " << nx + 1 << " float\n";
    for(int32 i = 0; i < nx + 1; i++)
    {
        os << x << "\n";
        x += dx;
    }

    os << "Y_COORDINATES " << ny + 1 << " float\n";
    for(int32 j = 0; j < ny + 1; j++)
    {
        os << y << "\n";
        y += dy;
    }

    os << "Z_COORDINATES " << nz + 1 << " float\n";
    for(int32 j = 0; j < nz + 1; j++)
    {
        os << z << "\n";
        z += dz;
    }

    os << "CELL_DATA " << nx * ny * nz << endl;

    return true;
}
