#include "regionPoints.hpp"
#include "fieldsDataBase.hpp"
#include "Time.hpp"

namespace pFlow::postprocessData
{

regionPoints::regionPoints
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDataBase
)
:
    fieldsDataBase_(fieldsDataBase)
{
    precision_ = dict.getValOrSet<int>("precision", 6);
    scientific_ = dict.getValOrSet<Logical>("scientific", Logical(true));
}

const Time& regionPoints::time() const
{
    return fieldsDataBase_.time();
}

const fieldsDataBase & regionPoints::database() const
{
    return fieldsDataBase_;
}

fieldsDataBase& regionPoints::database()
{
    return fieldsDataBase_;
}

} // namespace pFlow::postprocessData

