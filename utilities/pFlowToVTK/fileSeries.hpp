


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