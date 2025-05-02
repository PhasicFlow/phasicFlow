#include "postSimulationFieldsDataBase.hpp"
#include "vocabs.hpp"

namespace pFlow::postprocessData
{



bool postSimulationFieldsDataBase::pointFieldNameExists(const word& name) const
{
    if(currentFileFields_.contains(name)) return true;
    if(time().lookupObjectName(name)) return true;
    return false;
}

bool postSimulationFieldsDataBase::loadPointFieldToTime(const word &name)
{
    if(time().lookupObjectName(name)) return true;
    if(auto [iter, success]=currentFileFields_.findIf(name); success)
    {
        if(iter->second == "real")
        {
            allPointFields_.push_back
            (
                makeUnique<pointField_H<real>>
                (
                    objectFile
                    (
                        name,
                        allValidFolders_.folder(),
                        objectFile::READ_ALWAYS,
                        objectFile::WRITE_NEVER    
                    ),
                    pStructPtr_(),
                    0.0
                )
            );
        }
        else if(iter->second=="realx3")
        {
            allPointFields_.push_back
            (
                makeUnique<pointField_H<realx3>>
                (
                    objectFile
                    (
                        name,
                        allValidFolders_.folder(),
                        objectFile::READ_ALWAYS,
                        objectFile::WRITE_NEVER    
                    ),
                    pStructPtr_(),
                    0.0
                )
            );
        }
        else if(iter->second=="realx4")
        {
            allPointFields_.push_back
            (
                makeUnique<pointField_H<realx4>>
                (
                    objectFile
                    (
                        name,
                        allValidFolders_.folder(),
                        objectFile::READ_ALWAYS,
                        objectFile::WRITE_NEVER    
                    ),
                    pStructPtr_(),
                    0.0
                )
            );
        }
        else if(iter->second == "uint32")
        {
            allPointFields_.push_back
            (
                makeUnique<pointField_H<uint32>>
                (
                    objectFile
                    (
                        name,
                        allValidFolders_.folder(),
                        objectFile::READ_ALWAYS,
                        objectFile::WRITE_NEVER    
                    ),
                    pStructPtr_(),
                    0u
                )
            );
        }
        else
        {
            fatalErrorInFunction
                <<"Field "<<name<<" has an invalid type: "
                << iter->second<<endl;
            return false;
        }
    }
    else
    {
        fatalErrorInFunction
            <<"Filed "<<name<<" not found in the current time folder: "
            << allValidFolders_.folder()<<endl;
        return false;
    }
    
    return true;
}

bool postSimulationFieldsDataBase::loadPointStructureToTime()
{
    if(!pStructPtr_)
    {
        // create pointStructure 
        pStructPtr_ = makeUnique<pointStructure>(control_, 0.0005);
    }
    return true;
}

const shape &postSimulationFieldsDataBase::getShape() const
{
    if(!shapePtr_)
    {
        word shapeType = shapeTypeName();
        if(shapeType.empty())
        {
            fatalErrorInFunction
                << "shapeType is not set in the postprocess dictionary"<<endl;
            fatalExit;
        }
        propertyPtr_ = makeUnique<property>(pFlow::propertyFile__, control_.caseSetup().path());
        shapePtr_ = shape::create(shapeType, pFlow::shapeFile__, &control_.caseSetup(), propertyPtr_());
    }
    return shapePtr_();
}

word postSimulationFieldsDataBase::getPointFieldType(const word &name) const
{
    if(auto [iter, success]=currentFileFields_.findIf(name); success)
    {
        return iter->second;
    }
    else
    {
        fatalErrorInFunction
            <<"Field "<<name<<" not found in the current time folder: "
            << allValidFolders_.folder()<<endl;
        fatalExit;
    }
    return "";
}

bool postSimulationFieldsDataBase::setToCurrentFolder()
{
    allPointFields_.clear();
    pStructPtr_.reset(nullptr);
    

    if( !allValidFolders_.containsPointStructure(allValidFolders_.folder()) )
    {
        fatalErrorInFunction
            <<"Folder "<<allValidFolders_.folder()
            <<" does not contain a valid point structure file."<<endl;
        return false;
    }

    time().setTime(allValidFolders_.currentTime());

    loadPointStructureToTime();

    auto files = allValidFolders_.currentFolderFiles();

    currentFileFields_.clear();

    word type, space;
    for(auto& [name, objectType]: files)
    {
        if(fieldsDataBase::pointFieldGetType(objectType, type, space))
        {
            if(name == pointStructureFile__) continue;
            currentFileFields_.insertIf(name, type);
        }
    }

    return true;
}

postSimulationFieldsDataBase::postSimulationFieldsDataBase
(
    systemControl &control,
    const dictionary& postDict,
    bool inSimulation,
    timeValue startTime
)
: 
    fieldsDataBase(control, postDict, inSimulation, startTime),
    control_(control),
    allValidFolders_(control, true)
{
    if(allValidFolders_.empty())
    {
        fatalErrorInFunction
            << "No time folders found in the path: " << control.path()
            << " or no time folder with valid file content found in the path." 
            << endl;
            fatalExit;
    }
    if( ! allValidFolders_.setTime(startTime))
    {
        fatalErrorInFunction
            <<"The start time: "
            << startTime<<" for postprocessing is not valid."<<endl
            <<" valid range is ["<< allValidFolders_.startTime()
            <<","<<allValidFolders_.endTime()<<"]"<<endl;
            fatalExit;
    }
    
    if(!setToCurrentFolder())
    {
        fatalErrorInFunction
            <<"Error in setting the current folder to: "
            << allValidFolders_.folder()<<endl;
        fatalExit;
    }
}

const pointStructure& postSimulationFieldsDataBase::pStruct()const
{
    return pStructPtr_();
}

timeValue postSimulationFieldsDataBase::getNextTimeFolder() const
{
    return allValidFolders_.nextTime();
}

timeValue postSimulationFieldsDataBase::setToNextTimeFolder()
{
    timeValue nextTime = allValidFolders_.nextTime();
    if(nextTime < 0.0) return nextTime;

    allValidFolders_++;

    if(!setToCurrentFolder())
    {
        fatalErrorInFunction
            <<"Error in setting the current folder to the next time folder."<<endl;
        fatalExit;
        return -1;
    }

    return nextTime;
}

timeValue postSimulationFieldsDataBase::skipNextTimeFolder()
{
    timeValue nextTime = allValidFolders_.nextTime();
    if(nextTime < 0.0) return nextTime;

    allValidFolders_++;
    return nextTime;
}

} // namespace pFlow::postprocessData