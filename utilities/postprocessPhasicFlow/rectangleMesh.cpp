#include "rectangleMesh.hpp"


pFlow::rectangleMesh::rectangleMesh
(
    const box& mshBox, 
    int32 nx, 
    int32 ny, 
    int32 nz,
    repository* rep
)
:
    IOobject(
        objectFile
        (
            "rectMesh",
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER
        ),
        IOPattern::MasterProcessorOnly,
        rep
    ),
    meshBox_(mshBox),
    numCells_(nx, ny, nz)
{
    if(mshBox.minPoint()>= mshBox.maxPoint())
    {
        fatalErrorInFunction<<"Lower corner point of mesh "<<mshBox.minPoint()<<
        " confilicts with upper corner point of mesh "<<mshBox.maxPoint()<<endl;
        fatalExit;
    }

    numCells_ = max( numCells_ , int32x3(1) );

    dx_ = (mshBox.maxPoint() - mshBox.minPoint())/
        realx3(numCells_.x_, numCells_.y_, numCells_.z_);

}

pFlow::rectangleMesh::rectangleMesh(const dictionary &dict, repository* rep)
:
    rectangleMesh(
        box(dict),
        dict.getVal<int32>("nx"),
		dict.getVal<int32>("ny"),
		dict.getVal<int32>("nz"),
        rep
    )
{

}
