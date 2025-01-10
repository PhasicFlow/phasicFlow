#include "shape.hpp"


bool pFlow::shape::findPropertyIds()
{	
	shapePropertyIds_.resize(numShapes());

	ForAll( i, materialNames_)
	{	
		if(uint32 propId; property_.nameToIndex(materialNames_[i], propId) )
		{
			shapePropertyIds_[i] = propId;
		}
		else
		{
			fatalErrorInFunction<<"Material name "<< materialNames_[i]<< 
			" is not valid in dictionary "<<globalName()<<endl;
			return false;
		}
	}
    return true;
}

bool pFlow::shape::readFromDictionary2()
{

	materialNames_ = getVal<wordVector>("materials");

	if(materialNames_.size() != numShapes() )
	{
		fatalErrorInFunction<<
		"  number of elements in materials and names are not the same in "<< globalName()<<endl;
		return false;
	}

	return true;
}


pFlow::shape::shape
(
    const word &fileName, 
    repository *owner, 
    const property &prop
)
:
   baseShapeNames(fileName,owner),
    property_(prop)
{
	
	if( !readFromDictionary2() )
	{
		fatalErrorInFunction;
		fatalExit;
	}

	if(!findPropertyIds())
	{
		fatalErrorInFunction;
		fatalExit;
	}

}

bool pFlow::shape::writeToDict(dictionary &dict)const
{
	if(!baseShapeNames::writeToDict(dict))return false;

	if( !dict.add("materials", materialNames_) )
	{
		fatalErrorInFunction<<
		"  Error in writing materials to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	return true;
}