#include "regionPoints.hpp"
#include "fieldsDataBase.hpp"
#include "Time.hpp"

pFlow::regionPoints::regionPoints
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDataBase
)
:
    fieldsDataBase_(fieldsDataBase)
{}

const pFlow::Time& pFlow::regionPoints::time() const
{
    return fieldsDataBase_.time();
}

const pFlow::fieldsDataBase & pFlow::regionPoints::database() const
{
    return fieldsDataBase_;
}

pFlow::fieldsDataBase& pFlow::regionPoints::database()
{
    return fieldsDataBase_;
}

