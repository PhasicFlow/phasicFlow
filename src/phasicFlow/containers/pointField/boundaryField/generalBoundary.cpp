#include "generalBoundary.hpp"
#include "pointStructure.hpp"

pFlow::generalBoundary::generalBoundary
(
	const boundaryBase& boundary,
	const pointStructure& pStruct,
	const word&  dataType,
	const word&  option
)
:
	observer(&boundary, defaultMessage_),
	boundary_(boundary),
	pStruct_(pStruct)
{}


pFlow::Time const& pFlow::generalBoundary::time() const
{
    return pStruct_.time();
}