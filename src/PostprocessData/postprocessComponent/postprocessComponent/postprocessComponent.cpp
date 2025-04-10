#include "postprocessComponent.hpp"
#include "fieldsDataBase.hpp"
#include "Time.hpp"


pFlow::postprocessComponent::postprocessComponent
( 
    const dictionary &dict,
    fieldsDataBase &fieldsDB, 
    const baseTimeControl &defaultTimeControl
)
:
    timeControl_(
        dict,
        defaultTimeControl,
        baseTimeControl(
            fieldsDB.time().startTime(),
            fieldsDB.time().endTime(),
            fieldsDB.time().saveInterval())
    ),
    fieldsDataBase_(fieldsDB)
{
}


pFlow::uniquePtr<pFlow::postprocessComponent> pFlow::postprocessComponent::create
(
    const dictionary& dict,
    fieldsDataBase& fieldsDB,
    const baseTimeControl& defaultTimeControl
)
{
    word method = dict.getVal<word>("processMethod");
    word region = dict.getVal<word>("processRegion");
    
    auto compModel = angleBracketsNames2("PostprocessComponent", region, method);

    if( dictionaryvCtorSelector_.search(compModel) )
    {
        REPORT(2)<<"Creating postprocess component "<< Green_Text(compModel)<<" ..."<<endl;
        return dictionaryvCtorSelector_[compModel](dict, fieldsDB, defaultTimeControl);
    }
    else
    {
        printKeys
		( 
			fatalError << "Ctor Selector "<< Yellow_Text(compModel) << " dose not exist. \n"
			<<"Avaiable ones are: \n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
        return nullptr;
    }
}