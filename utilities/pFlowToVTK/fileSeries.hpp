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

#ifndef __fileSeries_hpp__
#define __fileSeries_hpp__

#include "fileStream.hpp"
#include "Map.hpp"


namespace pFlow::PFtoVTK
{

class fileSeries
{
private:

    using TimeFileType = Map<real, word>; 
    
    Map<word, TimeFileType>     timeFiles_;

    fileSystem          destDir_;

    void writeOneBaseName(const word& baseName);

public:

    fileSeries(const fileSystem dest);
        
    ~fileSeries();


    bool addTimeFile(const word& baseName, real time, const word& fileName);

    bool addTimeFile(const wordList& baseNames, real time, const wordList& fileNames);
};

}


#endif //__fileSeries_hpp__