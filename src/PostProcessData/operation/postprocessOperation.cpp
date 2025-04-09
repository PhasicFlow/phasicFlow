/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------
Licence:
  This file is part of phasicFlow code. It is a free software for simulating
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions.

  phasicFlow is distributed to help others in their research in the field of
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

#include "postprocessOperation.hpp"
#include "regionPoints.hpp"
#include "fieldsDataBase.hpp"

pFlow::postprocessOperation::postprocessOperation
(
    const dictionary &opDict, 
    const regionPoints& regPoints,
    fieldsDataBase &fieldsDB
)
:
    operationDict_(opDict),
    threshold_
    (
        opDict.getValOrSet<int>("threshold", 1)
    ),
    divideByVolume_
    (
        opDict.getValOrSet<Logical>("dividedByVolume", Logical(false))
    ),
    regionPoints_
    (
        regPoints
    ),
    database_
    (
        fieldsDB
    ),
    fieldName_
    (
        opDict.getValOrSet<word>("field", "one")
    ),
    phiFieldName_
    (
        opDict.getValOrSet<word>("phi", "one")
    ),
    includeMask_
    (
        includeMask::create(opDict, fieldsDB)
    )
{

    if(!fieldsDB.getPointFieldType(fieldName_, fieldType_))
    {
        fatalErrorInFunction;
        fatalExit;
    }
}

pFlow::uniquePtr<pFlow::postprocessOperation> 
    pFlow::postprocessOperation::create
    (
        const dictionary &opDict, 
        const regionPoints& regPoints,
        fieldsDataBase &fieldsDB
    )
{
    word func = opDict.getVal<word>("function");
    word method = angleBracketsNames("PostprocessOperation", func);
    if( dictionaryvCtorSelector_.search(method) )
    {
        REPORT(3)<<"Operation "<< Green_Text(opDict.name())<<" with function "<< Green_Text(func)<<endl;
        auto objPtr = 
            dictionaryvCtorSelector_[method]
            (opDict, regPoints, fieldsDB);
        return objPtr;
    }
    else
    {
        printKeys
        ( 
            fatalError << "Ctor Selector "<< 
            method << " dose not exist. \n"
            <<"Avaiable ones are: \n\n"
            ,
            dictionaryvCtorSelector_
        );
        fatalExit;
        return nullptr;
    }
}
