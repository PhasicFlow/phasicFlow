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

#include "fileSeries.hpp"
#include "oFstream.hpp"
#include "streams.hpp"

void pFlow::PFtoVTK::fileSeries::writeOneBaseName(const word &baseName)
{
    auto& fList = timeFiles_[baseName];
    word fileName = baseName + ".vtk.series";
    oFstream os(destDir_+fileName);
    
    REPORT(1)<<"Writing to "<<os.name()<<END_REPORT;

    os<<"{\n";
    os<<"    \"file-series-version\" : \"1.0\",\n";
    os<<"    \"files\" : [\n";
    const auto lastItem = std::prev(fList.end());
    for(auto iter = fList.begin(); iter != fList.end(); ++iter)
    {
        os<<"        {\"name\" : \"";
        os<< iter->second;
        os<<"\", \"time\" :";
        os<<iter->first;
        if(iter == lastItem)
            os<<"}\n";
        else
            os<<"},\n";
    }
    os<<"    ]\n";
    os<<"}\n";
   
}

pFlow::PFtoVTK::fileSeries::fileSeries(const fileSystem dest)
    : destDir_(dest)
{
    

}

pFlow::PFtoVTK::fileSeries::~fileSeries()
{
    REPORT(0)<<"Writing time series files..."<<END_REPORT;
    for(const auto& fSeries:timeFiles_)
    {
        writeOneBaseName(fSeries.first);
    }
}

bool pFlow::PFtoVTK::fileSeries::addTimeFile(const word& baseName, real time, const word &fileName)
{
    fileSystem fs(fileName);
    word relFileName = fs.relative(destDir_).wordPath();

    if( auto [iter, found]=timeFiles_.findIf(baseName); found )
    {
        return iter->second.insertIf(time, relFileName);
    }
    else
    {   
        TimeFileType newList;
        newList.insertIf(time, relFileName);
        return timeFiles_.insertIf(baseName, newList);
    }
    
}

bool pFlow::PFtoVTK::fileSeries::addTimeFile(const wordList &baseNames, real time, const wordList &fileNames)
{
    if(baseNames.size()!= fileNames.size() )
    {
        fatalErrorInFunction<<"sizes of base names and file names are not equal\n";
        return false;
    }

    for(size_t i=0; i<baseNames.size(); i++)
    {
        if(!addTimeFile(baseNames[i], time, fileNames[i]))
        {
            fatalErrorInFunction<<"Error in adding multiple time files\n";
            return false;
        }
    }

    return true;
}
