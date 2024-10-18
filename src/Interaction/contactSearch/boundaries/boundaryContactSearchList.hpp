#include "ListPtr.hpp"
#include "boundaryContactSearch.hpp"

namespace pFlow
{

class boundaryList;
class contactSearch;

class boundaryContactSearchList
:
    public ListPtr<boundaryContactSearch>
{
private:

    const boundaryList&         boundaries_;

    void setList(
        const dictionary& dict,
        const contactSearch& cSearch);
public:

    TypeInfoNV("boundaryContactSearchList");

    boundaryContactSearchList(
        const dictionary& dict,
        const boundaryList& bndrs,
        const contactSearch& cSearch);
    
    ~boundaryContactSearchList()=default;

    inline
    const boundaryList& boundaries()const
    {
        return boundaries_;
    } 

    bool boundariesUpdated()const;
    
};


}