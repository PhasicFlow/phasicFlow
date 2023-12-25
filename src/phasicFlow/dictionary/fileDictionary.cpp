#include "fileDictionary.hpp"
#include "IOPattern.hpp"


pFlow::fileDictionary::fileDictionary(const word &keyword)
:
    dictionary(keyword, true)
{
}

pFlow::fileDictionary::fileDictionary(const word &keyword, const fileSystem &file)
:
    dictionary(keyword, file)
{
}

bool pFlow::fileDictionary::read(iIstream &is, const IOPattern &iop)
{
    
    return dictionary::read(is);
}

bool pFlow::fileDictionary::write(iOstream &os, const IOPattern &iop) const
{
    return dictionary::write(os);
}
