#include "subSurface.hpp"

pFlow::iOstream& pFlow::operator<<(iOstream &str, const subSurface &sub)
{
    
    str.beginSquare()<<sub.name_<<token::SPACE<<
    sub.start_<<token::SPACE<<sub.end_<<token::SPACE<<
    sub.pointStart_<<token::SPACE<<sub.pointEnd_;
    str.endSquare();  
	str.check(FUNCTION_NAME);
    return str;

}

pFlow::iIstream& pFlow::operator>>(iIstream &str, subSurface &sub)
{
    str.readBeginSquare("subSurface");
    str >> sub.name_;
    str >> sub.start_;
    str >> sub.end_;
    str >> sub.pointStart_;
    str >> sub.pointEnd_;
    str.readEndSquare("subSurface");
    str.check(FUNCTION_NAME);
    return str;
}
