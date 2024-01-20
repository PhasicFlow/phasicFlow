#include "fileDictionary.hpp"


pFlow::fileDictionary::fileDictionary
(
    const objectFile & of, 
    repository* owner
)
:
    IOobject
    (
        of, 
        IOPattern::IOPattern::AllProcessorsSimilar, 
        owner
    ),
    dictionary
    (
        of.name(), 
        true
    )
{
    dictionary::name_ = IOobject::path().wordPath();
    if(!IOobject::readObject())
    {
        fatalErrorInFunction<<
        "Error in reading from dictionary "<< globalName()<<endl;
        fatalExit;
    }
}

pFlow::fileDictionary::fileDictionary(const word &keyword, const fileSystem &file)
:
    fileDictionary
    (
        objectFile
        (
            keyword,
            file,
            objectFile::READ_ALWAYS,
            objectFile::WRITE_NEVER
        ),
        nullptr
    )
{}

bool pFlow::fileDictionary::read(iIstream &is, const IOPattern &iop)
{
    return dictionary::read(is);
}

bool pFlow::fileDictionary::write(iOstream &os, const IOPattern &iop) const
{
    return dictionary::write(os);
}
