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

#include "particleProbePostprocessComponent.hpp"
#include "Time.hpp"

namespace pFlow::postprocessData
{
    
template<typename T>
inline
regionField<T> porbeExecute
(
    const word&          regFieldName,
    const span<T>&       field,
    const regionPoints&  regPoints
)
{
    regionField<T> processedField(regFieldName, regPoints, T{});
    auto partIndices = regPoints.indices(0);

    uint n = 0;
    for(auto index:partIndices)
    {
        if(index != -1)
        {
            processedField[n] =  field[index];
        }
        n++;
    }
    return processedField;
}

template<typename T>
inline bool writeField
(
    iOstream&             os,
    timeValue             t,
    const regionField<T>& field,
    const regionPoints&   regPoints,
    const T&              invalidVal = T{}
)
{
    auto indices = regPoints.indices(0);
    const uint32 s= field.size();
    os<< t <<tab;
    for(uint32 i=0; i<s; i++)
    {
        if constexpr(std::is_same_v<T,realx3>)
        {
            os<<field[i].x()<<' '<<field[i].y()<<' '<<field[i].z()<<tab;
        }
        else if constexpr( std::is_same_v<T,realx4> )
        {
            os<<field[i].x()<<' '<<field[i].y()<<' '<<field[i].z()<<tab<<field[i].w()<<tab;
        }
        else
        {
            os<<field[i]<<tab;
        }
    }
    os<<endl;
    return true;
}

}

pFlow::postprocessData::particleProbePostprocessComponent::particleProbePostprocessComponent
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDB, 
    const baseTimeControl &defaultTimeControl
)
:
    postprocessComponent(dict, fieldsDB, defaultTimeControl),
    regionPointsPtr_
    (
        makeUnique<centerPointsRegionPoints>(dict, fieldsDB)
    ),
    fieldName_
    (
        dict.getVal<word>("field")
    ),
    name_(dict.name())
{}

bool pFlow::postprocessData::particleProbePostprocessComponent::execute
(
    const timeInfo &ti, 
    bool forceExecute
)
{
    if( !forceExecute && !timeControl().eventTime(ti))
    {
        executed_ = false;
        return true;
    }

    REPORT(1)<<"Executing postprocess component ("
        <<Blue_Text(ti.timeName())<<" s) : "
        << name()
        <<END_REPORT;

    if(!regionPointsPtr_().update())
    {
        fatalErrorInFunction
            << "regionPoints update for "<< name_ << " failed. \n";
        return false;
    }

    auto field = database().updateFieldAll(fieldName_);
    auto pFieldName = name_;
    
    processedField_ = makeUnique<processedRegFieldType>
    (
        std::visit(
            [&](auto&& f) -> processedRegFieldType
            {
                return porbeExecute(
                    pFieldName,
                    f,
                    regionPointsPtr_());
            },
            field)
    );

    executed_ = true;
    return true;
}


bool pFlow::postprocessData::particleProbePostprocessComponent::write(const fileSystem& parDir)const
{
    if(! executed_ ) return true;
    
    const auto ti = database().time().TimeInfo();
    
    if( !osPtr_)
    {
        // file is not open yet
        fileSystem path = parDir + (name_+".Start_"+ti.timeName());
        osPtr_ = makeUnique<oFstream>(path);
        regionPointsPtr_().write(osPtr_());
    }

    std::visit
    ([&](auto&& arg)->bool
        {
            return writeField(osPtr_(), ti.t(), arg, regionPointsPtr_());
        },
        processedField_()
    );

    return true;
}