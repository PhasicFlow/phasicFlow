
#include "searchBoundary.hpp"
#include "contactSearch.hpp"


pFlow::searchBoundary::searchBoundary
(
    const dictionary& dict,
	const boundaryBase& boundary,
	const contactSearch& cSearch
)
:
    generalBoundary
    (
        boundary,
        cSearch.pStruct(),
        "",
        ""
    ),
    contactSearch_(cSearch),
    updateInterval_(dict.getVal<uint32>("updateInterval"))
{

}

pFlow::uniquePtr<pFlow::searchBoundary> 
    pFlow::searchBoundary::create
    (
        const dictionary &dict, 
        const boundaryBase &boundary, 
        const contactSearch &cSearch
    )
{
    return nullptr;    
}

