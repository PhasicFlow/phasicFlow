#include <regex>

#include "vtkFile.hpp"
#include "vocabs.hpp"
#include "pointFieldToVTK.hpp"


bool pFlow::PFtoVTK::convertTimeFolderPointFields(
    systemControl &control,
    const fileSystem &destPath,
    const word &bName,
    word& filename)
{

    fileSystem timeFolder = control.time().path();
    // check if pointStructure exist in this folder
    IOfileHeader pStructHeader(
        objectFile(
            pointStructureFile__,
            timeFolder,
            objectFile::READ_ALWAYS,
            objectFile::WRITE_ALWAYS));

    if (!pStructHeader.headerOk(true))
    {
        output << yellowColor << "Time folder " << 
        control.time().path() << " does not contain any pStructure data file."<<
        " Skipping this folder . . ."<< defaultColor << nl;
        return true;
    }

    vtkFile vtk(destPath, bName, control.time().currentTime(), bindaryOutput__);

    if (!vtk)
        return false;

    filename = vtk.fileName().wordPath();

    REPORT(1);
    auto pStruct = pointStructure(control);

    // get a list of files in this timeFolder;

    auto posVec = pStruct.pointPositionHost();
    auto *pos = posVec.data();

    REPORT(2) << ">>> Writing pointStructure to vtk file with " << 
    Yellow_Text(pStruct.numActive())<< 
    " active particles" << END_REPORT;

    addUndstrcuturedGridField(
        vtk(),
        pos,
        pStruct.numActive());

    auto fileList = containingFiles(timeFolder);

    for (const auto &file : fileList)
    {

        IOfileHeader fieldHeader(
            objectFile(
                file.fileName(),
                file.dirPath(),
                objectFile::READ_ALWAYS,
                objectFile::WRITE_ALWAYS));

        if (fieldHeader.headerOk(true))
        {
            // 64-bit intergers are not supported for convertion 
            if(
                convertRealx3TypePointField(vtk(), fieldHeader, pStruct) ||
                convertRealTypePointField(vtk(), fieldHeader, pStruct) ||
                convertIntPointField<uint32>(vtk(), fieldHeader, pStruct) ||
                //convertIntPointField<uint64>(vtk(), fieldHeader, pStruct) ||
                convertIntPointField<int32>(vtk(), fieldHeader, pStruct) ||
                //convertIntPointField<int64>(vtk(), fieldHeader, pStruct)||
                fieldHeader.objectName() == pointStructureFile__ )
            {
                continue;
            }
            else
            {
                WARNING << " This object type, " << 
                fieldHeader.objectType() << " is not supported" <<
                END_WARNING;
            }
        }
        output << endl;
    }

    return true;
}

bool pFlow::PFtoVTK::convertTimeFolderPointFieldsSelected(
    systemControl &control,
    const fileSystem &destPath,
    const word &bName,
    const wordVector &fieldsName,
    bool mustExist,
    word& filename)
{
    fileSystem timeFolder = control.time().path();
    // check if pointStructure exist in this folder
    IOfileHeader pStructHeader(
        objectFile(
            pointStructureFile__,
            timeFolder,
            objectFile::READ_ALWAYS,
            objectFile::WRITE_ALWAYS));

    if (!pStructHeader.headerOk(true))
    {
        output << yellowColor << "Time folder " << 
        control.time().path() << 
        " does not contain any pStructure data file."<<
        " Skipping this folder . . ."<< defaultColor << nl;
        return true;
    }

    vtkFile vtk(destPath, bName, control.time().currentTime(), bindaryOutput__);

    if (!vtk)
        return false;
    
    filename = vtk.fileName().wordPath();

    REPORT(1);
    auto pStruct = pointStructure(control);

    // get a list of files in this timeFolder;

    auto posVec = pStruct.pointPositionHost();
    auto *pos = posVec.data();

    REPORT(2) << ">>> Writing pointStructure to vtk file with " << 
    Yellow_Text(pStruct.numActive())
        << " active particles" << END_REPORT;

    addUndstrcuturedGridField(
        vtk(),
        pos,
        pStruct.numActive());

    auto fileList = containingFiles(timeFolder);

    for (const auto &fname : fieldsName)
    {
        fileSystem fieldAddress = timeFolder + fname;

        IOfileHeader fieldHeader(
            objectFile(
                fname,
                timeFolder,
                objectFile::READ_ALWAYS,
                objectFile::WRITE_ALWAYS));

        if (fieldHeader.headerOk(true))
        {
            if (
                // 64-bit intergers are not supported for convertion 
                convertRealx3TypePointField(vtk(), fieldHeader, pStruct) ||
                convertRealTypePointField(vtk(), fieldHeader, pStruct) ||
                convertIntPointField<uint32>(vtk(), fieldHeader, pStruct) ||
               // convertIntPointField<uint64>(vtk(), fieldHeader, pStruct) ||
                convertIntPointField<int32>(vtk(), fieldHeader, pStruct) ||
                //convertIntPointField<int64>(vtk(), fieldHeader, pStruct) ||
                fieldHeader.objectName() == pointStructureFile__ )
            {
                continue;
            }
            else
            {
                
                WARNING << " This object type, " << 
                fieldHeader.objectType() << " is not supported" << 
                END_WARNING;
            }
        }
        else
        {
            if (mustExist)
            {
                fatalErrorInFunction << "Field " << fieldAddress << 
                " does not exist." << endl;
                return false;
            }
            else
            {
                REPORT(2) << "Could not find " << Yellow_Text(fieldAddress) <<
                 ". Skipping this field . . ." << END_REPORT;
            }
        }
    }

    return true;
}


bool pFlow::PFtoVTK::addUndstrcuturedGridField(
    Ostream &os,
    realx3 *position,
    uint32 numPoints)
{

    os << "DATASET UNSTRUCTURED_GRID\n";
    
    if (numPoints == 0)
        return true;

    os << "POINTS " << numPoints << " float"<<'\n';
    if(bindaryOutput__)
    {    
        for(uint32 i=0; i<numPoints; i++)
        {           
            float x =  byteSwaper(static_cast<float>(position[i].x()));
            float y =  byteSwaper(static_cast<float>(position[i].y()));
            float z =  byteSwaper(static_cast<float>(position[i].z()));
            os.stdStream().write(reinterpret_cast<const char*>(&x), sizeof(x));
            os.stdStream().write(reinterpret_cast<const char*>(&y), sizeof(y));
            os.stdStream().write(reinterpret_cast<const char*>(&z), sizeof(z));
        }
        os<<'\n';
        os << "CELLS " << numPoints << ' ' << 2 * numPoints<<'\n';
        
        const int32 one_ro = byteSwaper(1);
        for (int i = 0; i < numPoints; i++)
        {
            int pN = byteSwaper(i);
            os.stdStream().write(reinterpret_cast<const char*>(&one_ro), sizeof(one_ro));
            os.stdStream().write(reinterpret_cast<const char*>(&pN), sizeof(pN));
        }
        os<<'\n';
        os << "CELL_TYPES " << numPoints<<'\n';
        for (int32 i = 0; i < numPoints; i++)
        {
            os.stdStream().write(reinterpret_cast<const char*>(&one_ro), sizeof(one_ro));
        }
        os<<'\n';
    }
    else
    {
        for (uint32 i = 0; i < numPoints; i++)
        {
            os << position[i].x() << 
            ' ' << position[i].y() << 
            ' ' << position[i].z() << '\n';
        }

        os << "CELLS " << numPoints << ' ' << 2 * numPoints << '\n';
        for (uint32 i = 0; i < numPoints; i++)
        {
            os << 1 << ' ' << i << '\n';
        }

        os << "CELL_TYPES " << numPoints << '\n';

        for (int32 i = 0; i < numPoints; i++)
        {
            os << 1 << '\n';
        }

    }
    
    os << "POINT_DATA " << numPoints << endl;

    return true;
}

bool pFlow::PFtoVTK::convertRealTypePointField(
    Ostream &os,
    const IOfileHeader &header,
    pointStructure &pStruct)
{
    word objectType = header.objectType();

    if (!checkFieldType<real>(objectType))
        return false;

    REPORT(1);
    auto field = realPointField_H(
        header,
        pStruct,
        static_cast<real>(0));

    real const *data = field.deviceViewAll().data();

    REPORT(2) << ">>> Writing " << Green_Text(header.objectName()) << 
    " field to vtk." << END_REPORT;

    return addRealPointField(
        os,
        header.objectName(),
        data,
        pStruct.numActive());
}

bool pFlow::PFtoVTK::convertRealx3TypePointField(
    Ostream &os,
    const IOfileHeader &header,
    pointStructure &pStruct)
{
    word objectType = header.objectType();

    if (!checkFieldType<realx3>(objectType))
        return false;

    REPORT(1);
    auto field = realx3PointField_H(
        header,
        pStruct,
        {0.0, 0.0, 0.0});

    realx3 const *data = field.deviceViewAll().data();

    REPORT(2) << ">>> Writing " << Green_Text(header.objectName()) << 
    " field to vtk." << END_REPORT;

    return addRealx3PointField(
        os,
        header.objectName(),
        data,
        pStruct.numActive());
}

bool pFlow::PFtoVTK::addRealPointField(
    Ostream &os,
    const word &fieldName,
    const real *field,
    uint32 numData)
{
    if (numData == 0)
        return true;

    os << "FIELD FieldData 1\n"
       << fieldName << " 1 " << numData << " float\n";
    if(bindaryOutput__)
    {
        for (uint32 i = 0; i < numData; ++i)
        {
            float x =  byteSwaper(static_cast<float>(field[i]));
            os.stdStream().write(reinterpret_cast<const char*>(&x), sizeof(x));
        }
    }
    else
    {
        for (uint32 i = 0; i < numData; ++i)
        {
            os << field[i] << '\n';
        }

    }
    
    return true;
}

bool pFlow::PFtoVTK::addRealx3PointField(
    Ostream &os,
    const word &fieldName,
    const realx3 *field,
    uint32 numData)
{
    if (numData == 0)
        return true;

    os << "FIELD FieldData 1\n"
       << fieldName << " 3 " << numData << " float\n";

    if(bindaryOutput__)
    {
        for(uint32 i=0; i<numData; i++)
        {
            float x =  byteSwaper(static_cast<float>(field[i].x()));
            float y =  byteSwaper(static_cast<float>(field[i].y()));
            float z =  byteSwaper(static_cast<float>(field[i].z()));
            os.stdStream().write(reinterpret_cast<const char*>(&x), sizeof(x));
            os.stdStream().write(reinterpret_cast<const char*>(&y), sizeof(y));
            os.stdStream().write(reinterpret_cast<const char*>(&z), sizeof(z));
        }
        os<<'\n';
    }
    else
    {
        for (uint32 i = 0; i < numData; ++i)
        {
            os << field[i].x() << 
            ' ' << field[i].y() << 
            ' ' << field[i].z() << '\n';
        }
    }
    

    return true;
}

bool pFlow::PFtoVTK::regexCheck(const word &TYPENAME, const word &fieldType)
{
    std::regex match("pointField\\<([A-Za-z1-9_]*)\\,([A-Za-z1-9_]*)\\>");
    std::smatch search1;
    std::smatch search2;
    if (!std::regex_match(fieldType, search1, match))
        return false;
    if (!std::regex_match(TYPENAME, search2, match))
        return false;
    if (search1.size() != 3)
        return false;
    if (search1.size() != search2.size())
        return false;
    return search1[1] == search2[1];
}