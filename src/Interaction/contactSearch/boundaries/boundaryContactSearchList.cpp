#include "boundaryContactSearchList.hpp"
#include "boundaryList.hpp"

void pFlow::boundaryContactSearchList::setList(
    const dictionary &dict, 
    const contactSearch &cSearch)
{
    for(auto i=0; i<boundaries_.size(); i++)
    {
        this->set
        (
            i, 
            boundaryContactSearch::create(dict, boundaries_[i], cSearch)
        );
    }
}

pFlow::boundaryContactSearchList::boundaryContactSearchList(
    const dictionary &dict,
    const boundaryList& bndrs,
    const contactSearch &cSearch)
    :
    ListPtr(bndrs.size()), 
    boundaries_(bndrs)
{
    setList(dict, cSearch);
}


