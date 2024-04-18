#include "vocabs.hpp"
#include "vtkFile.hpp"
#include "triSurfaceFieldToVTK.hpp"

bool pFlow::TSFtoVTK::convertTimeFolderTriSurfaceFields(
    systemControl &control,
    const fileSystem &destPath,
    const word &bName,
    bool separate)
{

    auto timeFolder = control.geometry().path();

    // check if pointStructure exist in this folder
    IOfileHeader triSurfaeHeader(
        objectFile(
            triSurfaceFile__,
            timeFolder,
            objectFile::READ_ALWAYS,
            objectFile::WRITE_ALWAYS));

    if (!triSurfaeHeader.headerOk(true))
    {
        WARNING << "Time folder " << timeFolder << " does not contain any triSurface data file."
                << " Skipping this folder . . ." << END_WARNING;
        return true;
    }

    auto triSurfaceObj = multiTriSurface(
        objectFile(
            triSurfaceFile__,
            "",
            objectFile::READ_ALWAYS,
            objectFile::WRITE_ALWAYS),
        &control.geometry());

    if(separate)
    {
        return convertTimeFolderTriSurfaceFieldsSeparate(
            triSurfaceObj,
            destPath,
            control.time().currentTime(),
            bName);
    }
    else
    {
        return convertTimeFolderTriSurfaceFieldsSingle(
            triSurfaceObj,
            destPath,
            control.time().currentTime(),
            bName );
    }
}

bool pFlow::TSFtoVTK::triSurfaceToVTK(
    iOstream &os, 
    const realx3 *points, 
    const uint32x3 *vertices, 
    const subSurface &subSurf)
{
    auto nP = subSurf.numPoints();
	
	os << "DATASET UNSTRUCTURED_GRID" << endl;
	os << "POINTS " << nP << " float" << endl;

	for ( auto i=subSurf.pointStart(); i<subSurf.pointEnd(); i++ )
	{
		os << points[i].x()  << 
        " " << points[i].y() << 
        " " << points[i].z() << endl;
		if (!os) return false;
	}

	auto nTri = subSurf.size();
	
	os << "CELLS " << nTri << " " << 4*nTri << endl;

	for(auto i=subSurf.start(); i<subSurf.end(); i++)
	{
		os<< 3 <<
        " "<< vertices[i].x()-subSurf.pointStart() << 
        " "<< vertices[i].y()-subSurf.pointStart() <<
        " "<< vertices[i].z()-subSurf.pointStart()<<endl;
		if (!os) return false;
	}

    os<<"CELL_TYPES "<< nTri<<'\n';

	for(uint32 i=0; i<nTri; i++)
	{
		os<< 5 <<'\n';
	}
	
	os << "CELL_DATA " << nTri << endl;

	return true;
}

bool pFlow::TSFtoVTK::triSurfaceToVTK(
    iOstream &os, 
    const realx3 *points, 
    const uint32x3 *vertices, 
    uint32 numPoints, 
    uint32 numTris)
{

	os << "DATASET UNSTRUCTURED_GRID" << endl;
	os << "POINTS " << numPoints << " float" << endl;

	for ( auto i=0; i<numPoints; i++ )
	{
		os << points[i].x()  << 
        " " << points[i].y() << 
        " " << points[i].z() << endl;
		if (!os) return false;
	}
	
	os << "CELLS " << numTris << " " << 4*numTris << endl;

	for(auto i=0; i<numTris; i++)
	{
		os<< 3 <<
        " "<< vertices[i].x()<< 
        " "<< vertices[i].y()<<
        " "<< vertices[i].z()<<endl;
		if (!os) return false;
	}

    os<<"CELL_TYPES "<< numTris<<'\n';

	for(uint32 i=0; i<numTris; i++)
	{
		os<< 5 <<'\n';
	}
	
	os << "CELL_DATA " << numTris << endl;

	return true;
}

bool pFlow::TSFtoVTK::convertTimeFolderTriSurfaceFieldsSingle
(
    multiTriSurface &surface, 
    const fileSystem &destPath,
    real time, 
    const word &bName
)
{
    vtkFile vtk(destPath, bName, time);

    if (!vtk)
        return false;
    
    auto hPoints = surface.points().hostView();
    auto hVertices = surface.vertices().hostView();

    realx3 const* pData = hPoints.data();
    uint32x3 const* vData = hVertices.data();

    REPORT(1) << "Wrting triSurface geometry to vtk file "<<
    Green_Text(vtk.fileName()) << END_REPORT;
    
    if (! triSurfaceToVTK(
        vtk(), 
        pData, 
        vData, 
        surface.numPoints(), 
        surface.size()))
    {
        fatalErrorInFunction << 
        "error in writing triSurface data to vtk file " << 
        vtk.fileName() << endl;
        return false;
    }

    auto fileList = containingFiles(surface.path().dirPath());

    for(const auto& file:fileList)
    {
        IOfileHeader fieldHeader(
            objectFile(
            file.fileName(),
            file.dirPath(), 
            objectFile::READ_ALWAYS,
            objectFile::WRITE_ALWAYS) );

        if( fieldHeader.headerOk(true) )
        {            
            convertRealx3TypetriSurfaceField(vtk(), fieldHeader, surface);
        }
    }

    output<<endl;
    return true;
}

bool pFlow::TSFtoVTK::convertTimeFolderTriSurfaceFieldsSeparate(
    multiTriSurface &surface, 
    const fileSystem &destPath, 
    real time, 
    const word &bName)
{
    
    auto hPoints = surface.points().hostView();
    auto hVertices = surface.vertices().hostView();

    realx3 const* pData = hPoints.data();
    uint32x3 const* vData = hVertices.data();

    REPORT(1) << "Wrting triSurface geometry to vtk file . . ."<<
    END_REPORT;

    auto nSurf = surface.numSurfaces();
    for(auto nS=0; nS<nSurf; nS++)
    {
        auto sName = surface.subSurfaceName(nS);    
        vtkFile vtk(destPath, groupNames(bName,sName), time);
        REPORT(2) << "Wrting sub-surface to "<<
        Green_Text(vtk.fileName())<<END_REPORT;

        if (!vtk)
            return false;

        if (! triSurfaceToVTK(
            vtk(), 
            pData, 
            vData, 
            surface.subSurfaces()[nS]) )
        {
            fatalErrorInFunction << 
            "error in writing triSurface data to vtk file " << 
            vtk.fileName() << endl;
            return false;
        }
    } 

    auto fileList = containingFiles(surface.path().dirPath());

    for(const auto& file:fileList)
    {
        IOfileHeader fieldHeader(
            objectFile(
            file.fileName(),
            file.dirPath(), 
            objectFile::READ_ALWAYS,
            objectFile::WRITE_ALWAYS) );

        if( fieldHeader.headerOk(true) )
        {
            convertRealx3TypetriSurfaceFieldSeparate(
                destPath,
                fieldHeader,
                surface,
                bName,
                time);
        }

    } 

    output<<endl;
    return true;
}


bool pFlow::TSFtoVTK::convertRealx3TypetriSurfaceField(
	iOstream& os,
	const IOfileHeader& header,
	multiTriSurface& tSurface)
{
	word objectType = header.objectType();
    
	if(!checkTriFieldType<realx3>(objectType))return false;

	auto field = realx3TriSurfaceField_H
	(
		header,
		tSurface,
		static_cast<real>(0)
	);

	const realx3* data = field.deviceViewAll().data();
	
	REPORT(1)<<"writing "<< greenColor <<header.objectName()<<
    defaultColor<<" field to vtk."<<END_REPORT;

    return pFlow::PFtoVTK::addRealx3PointField
    (
        os, 
        header.objectName(),
        data,
        field.size()
    );
}


bool pFlow::TSFtoVTK::convertRealx3TypetriSurfaceFieldSeparate
(
	const fileSystem& destPath,
	const IOfileHeader& header,
	multiTriSurface& tSurface,
    const word& bName,
    real time
)
{
    word objectType = header.objectType();
    
	if(!checkTriFieldType<realx3>(objectType))return false;

	auto field = realx3TriSurfaceField_H
	(
		header,
		tSurface,
		static_cast<real>(0)
	);

	const realx3* data = field.deviceViewAll().data();
	
	/*REPORT(1)<<"writing "<< greenColor <<header.objectName()<<
    defaultColor<<" field to vtk."<<END_REPORT;*/

    auto nSurf = tSurface.numSurfaces();

    for(auto nS=0; nS<nSurf; nS++)
    {
        auto& subSurf = tSurface.subSurfaces()[nS];

        auto sName = subSurf.name();    
        vtkFile vtk(destPath, groupNames(bName,sName), time, true);
        REPORT(2) << "Wrting sub-surface to "<<
        Green_Text(vtk.fileName())<<END_REPORT;

        pFlow::PFtoVTK::addRealx3PointField
        (
            vtk(), 
            header.objectName(),
            data+subSurf.start(),
            subSurf.size()
        );
    }

    return true;
    /*return pFlow::PFtoVTK::addRealx3PointField
    (
        os, 
        header.objectName(),
        data,
        field.size()
    );*/
}