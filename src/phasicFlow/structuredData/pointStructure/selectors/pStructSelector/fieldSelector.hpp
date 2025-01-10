
#ifndef __fieldSelector_hpp__
#define __fieldSelector_hpp__

#include "pStructSelector.hpp"
#include "pointFields.hpp"
#include "Time.hpp"

namespace pFlow
{

template<typename T>
inline
Vector<T> selectedFieldVals(const pStructSelector& selector, const word& name)
{
    using FieldType =  pointField_D<T>;

    if(selector.selectedPoints().empty() )
    {
        return Vector<T>("selected-"+name);
    }

	if(!selector.pStruct().time().lookupObjectName(name))
	{
		fatalErrorInFunction<<
		"Field "<< name << " does not exist in time repository"<<endl;
		fatalExit;
	}

    if(
        selector.pStruct().time().lookupObjectTypeName(name) !=
        FieldType::TYPENAME())
    {
        fatalErrorInFunction<<
		"Type of field "<< name << " in time repository does not match with"<<
        FieldType::TYPENAME()<<endl;
		fatalExit;
    }

    const FieldType& field = selector.pStruct().time().lookupObject<FieldType>(name);

    auto fieldH = field.hostView();
    const auto& selected = selector.selectedPoints();
    Vector<T> selectedField(
        "selected-"+name, 
        selected.size(), 
        selected.size(), 
        RESERVE());

    for(uint32 i=0; i<selected.size(); i++)
    {
        selectedField[i] = fieldH[selected[i]];
    }

    return selectedField;
}

} // pFlow

#endif //__fieldSelector_hpp__