#include "rectMeshRegionPoints.hpp"
#include "fieldsDataBase.hpp"
#include "numericConstants.hpp"

void pFlow::postprocessData::rectMeshRegionPoints::findPointsBeyoundCells()
{
    // check if pointsBeyoundCells_ is initialized
    if(!pointsBeyoundCells_)
    {
        pointsBeyoundCells_ = makeUnique<decltype(pointsOnCells_)>
        (
            "selectedPoints2",
            this->size()
        );
    }

    // get the reference to pointsBeyoundCells_ and clear it
    auto& selectedPoints = pointsBeyoundCells_();

    // point positions are obtained from the database
    const auto points = database().updatePoints();

    // iterate through all cells to find points that are within the search radius
    for(int32 i=0; i<mapper_.nx(); i++)
    {
        for(int32 j=0; j<mapper_.ny(); j++)
        {
            for(int32 k=0; k<mapper_.nz(); k++)
            {
                uint32 cellIndex = mapper_(i,j,k);

                // copy the points in the center cell
                auto& cellIndices = selectedPoints[cellIndex];
                cellIndices.clear();
                
                
                if(pointsOnCells_[cellIndex].empty())
                    continue;

                const auto cellCenter = centerPoints_[cellIndex];
                const auto rad = 0.5*diameter_[cellIndex];
                
                for(int32 ii=-2; ii <= 2; ++ii)
                {
                    for(int32 jj=-2; jj <= 2; ++jj)
                    {
                        for(int32 kk=-2; kk <= 2; ++kk)
                        {

                            int32 ni = i + ii;
                            int32 nj = j + jj;
                            int32 nk = k + kk;
                            if(ni < 0 || nj < 0 || nk < 0)
                                continue;
                            
                            if(ni >= mapper_.nx() || nj >= mapper_.ny() || nk >= mapper_.nz())
                                continue;
                            
                            uint32 neighborIndex = mapper_(ni, nj, nk);

                            const auto& neighborPoints = pointsOnCells_[neighborIndex];

                            for(auto nIndx : neighborPoints)
                            {
                                if( (points[nIndx]-cellCenter).length() < rad )
                                {
                                    cellIndices.push_back(nIndx);
                                }
                            }

                        }
                    }
                }
            }
        }
    }
}


pFlow::postprocessData::rectMeshRegionPoints::rectMeshRegionPoints
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDataBase
)
:
    regionPoints(dict, fieldsDataBase),
    boxRegion_(dict.subDict("rectMeshInfo")),
    cellExtension_(dict.subDict("rectMeshInfo").getValOrSet<real>("cellExtension", 2.0)),
    pointsOnCells_("selectedPoints"),
    selectedPoints_(pointsOnCells_)
{
    cellExtension_ = max(cellExtension_, one);

    const auto& rectMeshInfo = dict.subDict("rectMeshInfo");
    
    auto nx = rectMeshInfo.getValMax<uint32>("nx", 1);
    auto ny = rectMeshInfo.getValMax<uint32>("ny", 1);
    auto nz = rectMeshInfo.getValMax<uint32>("nz", 1);
    
    mapper_ = cellMapper(uint32x3(nx, ny, nz));
    uint32 nCells = mapper_.nx() * mapper_.ny() * mapper_.nz();
    real vol = boxRegion_.volume() / nCells;
    volumes_.resize(nCells, vol);
    diameter_.resize(nCells, 2 * pow(3 * vol / 4.0 / Pi, 0.3333333));
    pointsOnCells_.resize(nCells);
    centerPoints_.resize(nCells);

    real dx = (boxRegion_.maxPoint().x() - boxRegion_.minPoint().x()) / mapper_.nx();
    real dy = (boxRegion_.maxPoint().y() - boxRegion_.minPoint().y()) / mapper_.ny();
    real dz = (boxRegion_.maxPoint().z() - boxRegion_.minPoint().z()) / mapper_.nz();
    
    for(uint32 i = 0; i < nx; ++i)
    {
        for(uint32 j = 0; j < ny; ++j)
        {
            for(uint32 k = 0; k < nz; ++k)
            {
                realx3 center = boxRegion_.minPoint() + 
                    realx3(
                        ( static_cast<real>(i) + 0.5) * dx,
                        ( static_cast<real>(j) + 0.5) * dy,
                        ( static_cast<real>(k) + 0.5) * dz
                    );
                centerPoints_[mapper_(i, j, k)] = center;
            }
        }
    }
}

void pFlow::postprocessData::rectMeshRegionPoints::applyRegionExtension()
{
    // it cannot be lower than 1
    cellExtension_ = max(one, cellExtension_);

    real vf = pow(cellExtension_, 3);
    for(auto& v:volumes_)
    {
        v *= vf;
    }

    for(auto& d:diameter_)
    {
        d *= cellExtension_;
    }
}

bool pFlow::postprocessData::rectMeshRegionPoints::update()
{
    const auto points = database().updatePoints();
    for (auto& elem : pointsOnCells_)
    {
        elem.clear();
    }
 
    real dx = (boxRegion_.maxPoint().x() - boxRegion_.minPoint().x()) / mapper_.nx();
    real dy = (boxRegion_.maxPoint().y() - boxRegion_.minPoint().y()) / mapper_.ny();
    real dz = (boxRegion_.maxPoint().z() - boxRegion_.minPoint().z()) / mapper_.nz();

    for (uint32 i = 0; i < points.size(); ++i)
    {
        if(boxRegion_.isInside(points[i]))
        {
            uint32 indexX = (points[i] - boxRegion_.minPoint()).x() / dx;
            uint32 indexY = (points[i] - boxRegion_.minPoint()).y() / dy;
            uint32 indexZ = (points[i] - boxRegion_.minPoint()).z() / dz;
            pointsOnCells_[mapper_(indexX, indexY, indexZ)].push_back(i);
        }
    }

    // search beyound cells is not required
    if( equal(cellExtension_,one))
    {
        selectedPoints_ = pointsOnCells_;
        return true;
    }

    // search beyound cells is required
    findPointsBeyoundCells();
    selectedPoints_ = pointsBeyoundCells_();

    return true;
}

bool pFlow::postprocessData::rectMeshRegionPoints::write(iOstream &os) const
{

    auto [x, y , z] = boxRegion_.minPoint();
    auto [nx, ny, nz] = mapper_.cells_;
    real dx = (boxRegion_.maxPoint().x() - boxRegion_.minPoint().x()) / mapper_.nx();
    real dy = (boxRegion_.maxPoint().y() - boxRegion_.minPoint().y()) / mapper_.ny();
    real dz = (boxRegion_.maxPoint().z() - boxRegion_.minPoint().z()) / mapper_.nz();


    os << "# vtk DataFile Version 3.0" << endl;
    os << "postProcessData" << endl;
    os << "ASCII" << endl;
    os << "DATASET RECTILINEAR_GRID" << endl;
	os << "DIMENSIONS " << nx + 1 << " " << ny + 1 << " " << nz + 1 << endl;
	
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
