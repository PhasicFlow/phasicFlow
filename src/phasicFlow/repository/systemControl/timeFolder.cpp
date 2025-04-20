/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

#include "timeFolder.hpp"
#include "vocabs.hpp"


bool pFlow::timeFolder::validateForPointStructure()
{
    std::erase_if
    (
        folders_, 
        [this](const auto& folder)
        {
            return !containsPointStructure(folder.second);
        }
    );
    return true;
}

bool pFlow::timeFolder::validFieldFile(const fileSystem &filePath) const
{
    IOfileHeader fieldHeader(
		objectFile(
			filePath.fileName(),
			filePath.dirPath(),
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER)
		);
    return fieldHeader.headerOk(true);
}

bool pFlow::timeFolder::validFieldFile
(
    const fileSystem &filePath, 
    word &fieldName, 
    word &objectType
) const
{
    IOfileHeader fieldHeader(
		objectFile(
			filePath.fileName(),
			filePath.dirPath(),
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER)
		);

    if(!fieldHeader.headerOk(true))
    {
        return false;
    }
    else
    {
        fieldName = fieldHeader.objectName();
        objectType = fieldHeader.objectType();
        return true;
    }
}

pFlow::timeFolder::timeFolder
(
    const systemControl& control,
    bool pointStructureOnly
)
:
    timeFolder(control.path(), pointStructureOnly)
{}


pFlow::timeFolder::timeFolder
(
    const fileSystem& path,
    bool pointStructureOnly
)
:
    folders_(getTimeFolders(path)),
    currentFolder_(folders_.begin())
{
    if(pointStructureOnly)
    {
        validateForPointStructure();
    }
    currentFolder_ = folders_.begin();
}

bool pFlow::timeFolder::containsPointStructure(const fileSystem &dirPath) const
{
    auto files = containingFiles(dirPath);
    for (const auto &file : files)
    {
        if (file.fileName() == pointStructureFile__)
        {
            return validFieldFile(file);
        }
    }
    return false;
}

pFlow::Map<pFlow::word, pFlow::word> pFlow::timeFolder::currentFolderFiles() const
{
    Map<word,word> fileList;
    if( folders_.empty()) return fileList;

    auto files = containingFiles(folder());
    word name, objectType;
    for (const auto &file : files)
    {
        if(validFieldFile(file, name, objectType))
        {
            fileList.insertIf(name, objectType);
        }
    }
    return fileList;
}
