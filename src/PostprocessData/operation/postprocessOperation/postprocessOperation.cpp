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

#include "Time.hpp"
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
    postprocessOperation
    (
        opDict,
        opDict.getVal<word>("field"),
        opDict.getValOrSet<word>("phi", "one"),
        opDict.getValOrSet<word>("includeMask", "all"),
        regPoints,
        fieldsDB
    )
{}

pFlow::postprocessOperation::postprocessOperation
(
    const dictionary &opDict, 
    const word &fieldName, 
    const word &phiName,
    const word& includeName,  
    const regionPoints &regPoints, 
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
        opDict.getValOrSet<Logical>("divideByVolume", Logical(false))
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
        fieldName
    ),
    phiFieldName_
    (
        phiName
    ),
    includeMask_
    (
        includeMask::create(includeName, opDict, fieldsDB)
    )
{

    if(!fieldsDB.getFieldType(fieldName_, fieldType_))
    {
        fatalErrorInFunction;
        fatalExit;
    }
}
const pFlow::Time& pFlow::postprocessOperation::time() const
{
    return database_.time();
}

bool pFlow::postprocessOperation::write(const fileSystem &parDir) const
{   
    auto ti = time().TimeInfo();

    if(!osPtr_)
    {
        fileSystem path = parDir+(
            processedFieldName() + ".Start_" + ti.timeName());
        osPtr_ = makeUnique<oFstream>(path);
        
        regPoints().write(osPtr_());
    }

    const auto& field = processedField();

    std::visit
    (
        [&](auto&& arg)->bool
        {
            return writeField(osPtr_(), ti.t(), arg, threshold_);
        },
        field
    );

    return true;
}

pFlow::uniquePtr<pFlow::postprocessOperation>
pFlow::postprocessOperation::create(
    const dictionary &opDict,
    const regionPoints &regPoints,
    fieldsDataBase &fieldsDB)
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
