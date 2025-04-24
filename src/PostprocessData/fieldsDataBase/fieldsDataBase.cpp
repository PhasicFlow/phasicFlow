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

#include <regex>

#include "vocabs.hpp"
#include "systemControl.hpp"
#include "fieldsDataBase.hpp"
#include "fieldFunctions.hpp"
#include "dictionary.hpp"


bool pFlow::postprocessData::fieldsDataBase::loadPointStructureToTime()
{
    return false;
}


bool pFlow::postprocessData::fieldsDataBase::checkForUpdate
(
    const word &compoundName, 
    bool forceUpdate
)
{
    auto t = currentTime();
    bool shouldUpdate = false;
    
    if(auto [iter, found]= captureTime_.findIf(compoundName); found)
    {
        shouldUpdate = iter->second < t || forceUpdate;
        iter->second = t;
    }
    else
    {
        shouldUpdate = true;
        captureTime_.insertIf(compoundName, t);
    }

    return shouldUpdate;
}

pFlow::span<pFlow::real> pFlow::postprocessData::fieldsDataBase::createOrGetRealField
(
    const word &name
)
{
    
    bool shouldUpdate = checkForUpdate(name);
        
    if(shouldUpdate)
    {
        allFields_.emplaceBackOrReplace<FieldTypeHost<real>>
        (
            name,
            FieldTypeHost<real>
            (
                name, 
                "value",
                pointFieldSize()
            )
        );
    }

    auto& field = allFields_.getObject<FieldTypeHost<real>>(name);
    return span<real>(
        field.data(), 
        field.size());
}

pFlow::span<pFlow::real> pFlow::postprocessData::fieldsDataBase::createOrGetVolume
(
    bool forceUpdate
)
{
    const word fName = "volume";
    bool shouldUpdate = checkForUpdate(fName, forceUpdate);
        
    if(shouldUpdate)
    {
        const auto index = updateFieldUint32("shapeIndex", true);
        auto vols = getShape().volume();
        
        FieldTypeHost<real> volField
        (
            fName, 
            "value",
            pointFieldSize()
        );

        for(uint32 i=0; i< volField.size(); i++)
        {
            volField[i] = vols[index[i]];
        }

        allFields_.emplaceBackOrReplace<FieldTypeHost<real>>
        (
            fName,
            std::move(volField)  
        );
    }

    auto& field = allFields_.getObject<FieldTypeHost<real>>(fName);
    return span<real>(
        field.data(), 
        field.size());

}

pFlow::span<pFlow::real> pFlow::postprocessData::fieldsDataBase::createOrGetDensity(bool forceUpdate)
{
    const word fName = "density";
    
    bool shouldUpdate = checkForUpdate(fName, forceUpdate);

    if(shouldUpdate)
    {
        const auto index = updateFieldUint32("shapeIndex", true);
        const auto dens = getShape().density();

        FieldTypeHost<real> denFeild
        (
            fName, 
            "value",
            pointFieldSize()
        );

        for(uint32 i=0; i< denFeild.size(); i++)
        {
            denFeild[i] = dens[index[i]];
        }

        allFields_.emplaceBackOrReplace<FieldTypeHost<real>>
        (
            fName,
            std::move(denFeild)  
        );
    }

    auto& field = allFields_.getObject<FieldTypeHost<real>>(fName);
    return span<real>(
        field.data(), 
        field.size());
}

pFlow::span<pFlow::real> pFlow::postprocessData::fieldsDataBase::createOrGetOne(bool forceUpdate)
{
    const word fName = "one";
    
    bool shouldUpdate = checkForUpdate(fName, forceUpdate);

    if(shouldUpdate)
    {
        allFields_.emplaceBackOrReplace<FieldTypeHost<real>>
        (
            fName,
            FieldTypeHost<real> 
            (
                fName, 
                "value",
                pointFieldSize(),
                1.0
            )  
        );
    }

    auto& field = allFields_.getObject<FieldTypeHost<real>>(fName);
    return span<real>(
        field.data(), 
        field.size());
}

pFlow::span<pFlow::real> pFlow::postprocessData::fieldsDataBase::createOrGetMass(bool forceUpdate)
{
    const word fName = "mass";
    
    bool shouldUpdate = checkForUpdate(fName, forceUpdate);

    if(shouldUpdate)
    {
        const auto index = updateFieldUint32("shapeIndex", true);
        const auto ms = getShape().mass();

        FieldTypeHost<real> massField
        (
            fName, 
            "value",
            pointFieldSize()
        );

        for(uint32 i=0; i< massField.size(); i++)
        {
            massField[i] = ms[index[i]];
        }

        allFields_.emplaceBackOrReplace<FieldTypeHost<real>>
        (
            fName,
            std::move(massField)  
        );
    }

    auto& field = allFields_.getObject<FieldTypeHost<real>>(fName);
    return span<real>(
        field.data(), 
        field.size());
}

pFlow::span<pFlow::real> pFlow::postprocessData::fieldsDataBase::createOrGetI(bool forceUpdate)
{
    const word fName = "I";
    
    bool shouldUpdate = checkForUpdate(fName, forceUpdate);

    if(shouldUpdate)
    {
        const auto index = updateFieldUint32("shapeIndex", true);
        const auto Is = getShape().Inertia();

        FieldTypeHost<real> IField
        (
            fName, 
            "value",
            pointFieldSize()
        );

        for(uint32 i=0; i< IField.size(); i++)
        {
            IField[i] = Is[index[i]];
        }

        allFields_.emplaceBackOrReplace<FieldTypeHost<real>>
        (
            fName,
            std::move(IField)  
        );
    }

    auto& field = allFields_.getObject<FieldTypeHost<real>>(fName);
    return span<real>(
        field.data(), 
        field.size());
}

bool pFlow::postprocessData::fieldsDataBase::findFunction(
    const word &compoundFieldName,
    word &fieldName,
    fieldsDataBase::Functions &func)
{

    std::regex pattern(R"((\w+)?\((\w+)(?:,([xyzw]))?\)|(\w+))");
    std::smatch match;

    if (std::regex_match(compoundFieldName, match, pattern))
    {
        if (match[1].matched) // Function is present
        {
            word functionName = match[1].str();
            fieldName = match[2].str();

            // Map the function name to the enum value
            if(functionName=="component")
            {
                if (!match[3].matched) // Component is not present
                {
                    fatalErrorInFunction << 
                        "Component (x, y, z, or w) is not specified in the function component: " << compoundFieldName <<
                        " the format is component(u,x), where u is the vector field name and x is the compoenent." << endl;
                    return false;
                }

                switch (match[3].str()[0])
                {
                    case 'x': func = fieldsDataBase::Functions::ComponentX; break;
                    case 'y': func = fieldsDataBase::Functions::ComponentY; break;
                    case 'z': func = fieldsDataBase::Functions::ComponentZ; break;
                    case 'w': func = fieldsDataBase::Functions::ComponentW; break;
                    default:
                        fatalErrorInFunction << 
                            "Invalid component specified: " << match[3].str() << endl;
                        return false;
                }
                return true;       
            }
            else if (functionName == "abs")
            {
                func = fieldsDataBase::Functions::Abs;
            }
            else if (functionName == "square")
            {
                func = fieldsDataBase::Functions::Square;
            }
            else if (functionName == "cube")
            {
                func = fieldsDataBase::Functions::Cube;
            }
            else if (functionName == "sqrt")
            {
                func = fieldsDataBase::Functions::SqureRoot;
            }
            else if (functionName == "mag")
            {
                func = fieldsDataBase::Functions::Magnitude;
            }
            else if (functionName == "magSquare")
            {
                func = fieldsDataBase::Functions::MagnitudeSquare;
            }
            else if (functionName == "magCube")
            {
                func = fieldsDataBase::Functions::MagnitudeCube;
            }
            else if (functionName == "magSqrt")
            {
                func = fieldsDataBase::Functions::MagnitudeSquareRoot;
            }
            else
            {
                fatalErrorInFunction << 
                    "Unknown function specified: " << functionName<< 
                    " in: "<<compoundFieldName << endl;
                return false;
            }

            if(match[3].matched)
            {
                fatalErrorInFunction << 
                    "The function: " << functionName << 
                    " does not accept component (x, y, z, or w) as the second argument (in: "
                    << compoundFieldName<<" )." << endl;
                return false;
            }
            else
            {
                return true;    
            }
            
        }
        else if (match[4].matched) // Only fieldName is present
        {
            fieldName = match[4].str();
            func = fieldsDataBase::Functions::None; // No function
            return true;
        }
        
    }

    return false; // No match
}

bool pFlow::postprocessData::fieldsDataBase::inputOutputType
(
    fieldsDataBase::Functions func, 
    const word &inputType, 
    word &outputType
)
{
    switch (func)
    {
        case Functions::ComponentX:
        case Functions::ComponentY:
        case Functions::ComponentZ:
            if(inputType == getTypeName<realx3>() ||
               inputType == getTypeName<realx4>() )
            {
                outputType = getTypeName<real>();
                return true;
            }
            else
            {
                fatalErrorInFunction<<"Wrong function: component(u,comp), for input field type: "<<inputType<<endl; 
                return false;
            }
        case Functions::ComponentW:
            if(inputType == getTypeName<realx4>() )
            {
                outputType = getTypeName<real>();
                return true;
            }
            else
            {
                fatalErrorInFunction<<"Wrong function: component(u,w), for input field type: "<<inputType<<endl; 
                return false;
            }
            break;
        case Functions::Abs:
        case Functions::Square:
        case Functions::Cube:
        case Functions::SqureRoot:
            if(inputType == getTypeName<real>() || 
               inputType == getTypeName<uint32>() ||
               inputType == getTypeName<int32>() ||
               inputType == getTypeName<int64>() )
            {
                outputType = getTypeName<real>();
                return true;
            }
            else
            {
                fatalErrorInFunction<<"Wrong input field type for functions abs, squqre, cube, and sqrt."<<
                    " field type is "<< inputType<<endl;
                return false;
            }
            break;
        case Functions::Magnitude:
        case Functions::MagnitudeSquare:
        case Functions::MagnitudeCube:
        case Functions::MagnitudeSquareRoot:
            if(inputType == getTypeName<realx3>() ||
               inputType == getTypeName<realx4>() )
            {
                outputType = getTypeName<real>();
                return true;
            }
            else
            {
                fatalErrorInFunction<<"Wroing input field type for functions mag, magSquare, magCube, magSqrt. "<<
                    " Input field type is "<< inputType<<endl;
                return false;
            }
            break;
        case Functions::None:
            if(inputType == getTypeName<realx3>() ||
               inputType == getTypeName<realx4>() ||
               inputType == getTypeName<real>())
            {
                outputType = inputType;
                return true;
            }
            else if( inputType == getTypeName<uint32>() ||
                     inputType == getTypeName<int32>() )
            {
                outputType = getTypeName<real>();
                return true;
            }
            else
            {
                fatalErrorInFunction<< "Wroing input field type "<< inputType<<endl;
                return false;
            }
            break;
        default:
            fatalErrorInFunction<<"Wroing function"<<endl;

    }
    return false;
}

pFlow::postprocessData::fieldsDataBase::fieldsDataBase
(
    systemControl& control,
    const dictionary& postDict,
    bool inSimulation,
    timeValue startTime
)
:
    time_(control.time()),
    inSimulation_(inSimulation)
{
    if(!inSimulation_)
    {
        shapeType_ = postDict.getValOrSet<word>("shapeType", "");

        if(shapeType_.empty())
        {
            WARNING
            << "shapeType is not set in dictionary: "
            << postDict.globalName()
            << " and you may need to set for some postprocess operations"<<END_WARNING;
        }
    }
    else
    {
        // this is not required for execution during simulation 
        shapeType_ = "";
    }
}

pFlow::timeValue pFlow::postprocessData::fieldsDataBase::currentTime() const
{
    return time_.currentTime();
}

bool pFlow::postprocessData::fieldsDataBase::getFieldTypeNameFunction
(
    const word& compoundName,
    word& pointFieldName, 
    word& originalType, 
    word& typeAfterFunction,
    Functions& func
)const
{
    if( !findFunction(compoundName, pointFieldName, func))
    {
        fatalErrorInFunction
            <<"Error in processing function for field: "
            <<compoundName<<endl;;
        return false;
    }

    if( reservedFieldNames_.contains(pointFieldName))
    {
        // The name is in the reserved fields 
        originalType = reservedFieldNames_.find(pointFieldName)->second;
    }
    else
    {
        // the name is in the Time object 
        if(pointFieldNameExists(pointFieldName))
        {
            originalType = getPointFieldType(pointFieldName);
        }
        else
        {
            fatalErrorInFunction
                << "The field name: "<< pointFieldName
                << " is not in the Time object.\n"
                <<" Avaiable ones are: \n"<< time().objectNames()<<endl;
            return false;
        }
    }

    if(!inputOutputType(func, originalType, typeAfterFunction))
    {
        fatalErrorInFunction
            <<"Cannnot determine the input and output types for: "
            << compoundName<<endl;
        return false;
    }

    return true;
}

bool pFlow::postprocessData::fieldsDataBase::getFieldType
(
    const word& compoundName,
    word& originalType, 
    word& typeAfterFunction
) const
{
    Functions func;
    word fieldName;
    if( !getFieldTypeNameFunction(compoundName, fieldName, originalType, typeAfterFunction, func))
    {
        return false;
    }    
    return true;
}

bool pFlow::postprocessData::fieldsDataBase::getFieldType
(
    const word &compoundName, 
    word &typeAfterFunction
) const
{
    Functions func;
    word originalType;
    word fieldName;
    if( !getFieldTypeNameFunction(compoundName, fieldName, originalType, typeAfterFunction, func))
    {
        return false;
    }    
    return true;
}

pFlow::span<pFlow::realx3> pFlow::postprocessData::fieldsDataBase::updatePoints(bool forceUpdate)
{
    const word fName = "position";
    bool shouldUpdate = checkForUpdate(fName, forceUpdate);
        
    if(shouldUpdate)
    {
        // load pointStructure 
        if(!loadPointStructureToTime())
        {
            fatalErrorInFunction<< "Error in loading pointStructure to Time object."<<endl;
            fatalExit;
        }
        const auto& pstruct = pStruct(); 
        allFields_.emplaceBackOrReplace<PointsTypeHost>
        (
            fName, 
            pstruct.activePointsHost()
        );
    }
   
    auto& points = allFields_.getObject<PointsTypeHost>(fName);
    
    return span<realx3>(
        points.data(), 
        points.size());

}

pFlow::span<pFlow::realx3> pFlow::postprocessData::fieldsDataBase::updateFieldRealx3
(
    const word &compoundName, 
    bool forceUpdate
)
{
    word originalType, typeAfterFunction, fieldName;
    Functions func;
    
    if( !getFieldTypeNameFunction(
        compoundName, 
        fieldName, 
        originalType, 
        typeAfterFunction, 
        func) )
    {
        fatalErrorInFunction<< "Error in getting the type name of field: "<<
            compoundName<<", with type name: "<< originalType <<endl;
        fatalExit;
        return span<realx3>(nullptr, 0);
    }

    if( originalType == getTypeName<realx3>() && func == Functions::None )
    {
        return updateField<realx3>(fieldName, forceUpdate);
    }
    else
    {
        fatalErrorInFunction<< "Error in getting the type name of field: "<<
            compoundName<<", with type name: "<< originalType <<endl;
        fatalExit;
        return span<realx3>(nullptr, 0);
    }
        
}

pFlow::span<pFlow::realx4> pFlow::postprocessData::fieldsDataBase::updateFieldRealx4
(
    const word &compoundName, 
    bool forceUpdate
)
{
    word originalType, typeAfterFunction, fieldName;
    Functions func;
    
    if( !getFieldTypeNameFunction(
        compoundName, 
        fieldName, 
        originalType, 
        typeAfterFunction, 
        func))
    {
        fatalErrorInFunction<< "Error in getting the type name of field: "<<
            compoundName<<", with type name: "<< originalType <<endl;
        fatalExit;
        return span<realx4>(nullptr, 0);
    }

    if( originalType == getTypeName<realx4>() && func == Functions::None )
    {
        return updateField<realx4>(fieldName, forceUpdate);
    }
    else
    {
        fatalErrorInFunction<< "Error in getting the type name of field: "<<
            compoundName<<", with type name: "<< originalType <<endl;
        fatalExit;
        return span<realx4>(nullptr, 0);
    }
    
}

pFlow::span<pFlow::real> pFlow::postprocessData::fieldsDataBase::updateFieldReal
(
    const word &compoundName, 
    bool forceUpdate
)
{
    word originalType, typeAfterFunction, fieldName;
    Functions func;
    
    if( !getFieldTypeNameFunction(
        compoundName, 
        fieldName, 
        originalType, 
        typeAfterFunction, 
        func) )
    {
        fatalErrorInFunction<< "Error in getting the type name of field: "<<
            compoundName<<", with type name: "<< originalType <<endl;
        fatalExit;
        return span<real>(nullptr, 0);
    }

    // if the output type is not real, then it is not supported yet
    if(typeAfterFunction != getTypeName<real>())
    {
        fatalErrorInFunction<< "The output type of field "<< compoundName<<
            " is not real, it is: "<< typeAfterFunction<<endl;
        fatalExit;
        return span<real>(nullptr, 0);
    }

    // if the orginal type is real and no function, then it is a normal field
    if( originalType == getTypeName<real>() && func == Functions::None )
    {
        return updateField<real>(fieldName, forceUpdate);
    }
    
    // if the original type is uint32, and no funciton, cast to real 
    if( originalType == getTypeName<uint32>())
    {
        if(func == Functions::None)
        {
            auto sp = updateField<uint32>(fieldName, forceUpdate);
            auto spReal = createOrGetRealField(fieldName+".real");
            funcCast(sp, spReal);
            return spReal;
        }
        else
        {
            fatalErrorInFunction<<"No function can be applied to field of type uint32. "<<
                " The field is: "<< compoundName<<endl;
            return span<real>(nullptr, 0);
            fatalExit;
        }
    }

    if( originalType == getTypeName<real>() )
    {
        switch(func)
        {
            case Functions::Abs:
            {
                auto sp = updateField<real>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName);
                funcAbs(sp, spReal);
                return spReal;
            }
            case Functions::Square:
            {
                auto sp = updateField<real>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName);
                funcSquare(sp, spReal);
                return spReal;
            }
            case Functions::Cube:
            {
                auto sp = updateField<real>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName);
                funcCube(sp, spReal);
                return spReal;
            }
            case Functions::SqureRoot:
            {
                auto sp = updateField<real>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName+".sqrt");
                funcSquareRoot(sp, spReal);
                return spReal;
            }
            default:
            {
                fatalErrorInFunction<< "Wrong function for field type real in :"<<
                    compoundName<<endl;
                fatalExit;
                return span<real>(nullptr, 0);
            }
        }
    }

    if( originalType == getTypeName<realx3>())
    {
        switch(func)
        {
            case Functions::Magnitude:
            {
                auto sp = updateField<realx3>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName);
                funcMagnitude(sp, spReal);
                return spReal;
            }
            case Functions::MagnitudeSquare:
            {
                auto sp = updateField<realx3>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName);
                funcMagnitudeSquare(sp, spReal);
                return spReal;
            }
            case Functions::MagnitudeCube:
            {
                auto sp = updateField<realx3>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName);
                funcMagnitudeCube(sp, spReal);
                return spReal;
            }
            case Functions::MagnitudeSquareRoot:
            {
                auto sp = updateField<realx3>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName);
                funcMagnitudeSquareRoot(sp, spReal);
                return spReal;
            }
            case Functions::ComponentX:
            {
                auto sp = updateField<realx3>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName);
                funcComponent(sp, spReal, 'x');
                return spReal;
            }
            case Functions::ComponentY:
            {
                auto sp = updateField<realx3>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName);
                funcComponent(sp, spReal, 'y');
                return spReal;
            }
            case Functions::ComponentZ:
            {
                auto sp = updateField<realx3>(fieldName, forceUpdate);
                auto spReal = createOrGetRealField(compoundName);
                funcComponent(sp, spReal, 'z');
                return spReal;
            }
            default:
            {
                fatalErrorInFunction<< "Wrong function for field type realx3 in :"<<
                    compoundName<<endl;
                fatalExit;
                return span<real>(nullptr, 0);
            }
        }
    }

    fatalErrorInFunction<<"NOT SUPPORTED "<<compoundName<<endl;
    fatalExit;
    // This line should never be reached
    return span<real>(nullptr, 0);
}

pFlow::span<pFlow::uint32> pFlow::postprocessData::fieldsDataBase::updateFieldUint32
(
    const word& name,
    bool forceUpdate
)
{
    return updateField<uint32>(name, forceUpdate);
}

pFlow::postprocessData::allPointFieldTypes pFlow::postprocessData::fieldsDataBase::updateFieldAll
(
    const word &compoundName, 
    bool forceUpdate
)
{
    word typeAfterFunction, originalType;
    
    if( !getFieldType(compoundName, originalType, typeAfterFunction))
    {
        fatalErrorInFunction<< "Error in getting the type name of field: "<<
            compoundName<<", with type name: "<< originalType <<endl;
        fatalExit;
        return span<real>(nullptr, 0);
    }

    if(  typeAfterFunction == getTypeName<realx3>()  )
    {
        return updateFieldRealx3(compoundName, forceUpdate);
    }
    else if( typeAfterFunction == getTypeName<realx4>() )
    {
        return updateFieldRealx4(compoundName, forceUpdate);
    }
    else if( typeAfterFunction == getTypeName<real>() )
    {
        return updateFieldReal(compoundName, forceUpdate);
    }
    else
    {
        fatalErrorInFunction<< "Invalid feild "<< compoundName<<endl;
        fatalExit;
        return span<real>(nullptr, 0);
    }
}

bool pFlow::postprocessData::fieldsDataBase::pointFieldGetType
(
    const word& TYPENAME, 
    word& fieldType, 
    word& fieldSpace
)
{
	std::regex match("pointField\\<([A-Za-z1-9_]*)\\,([A-Za-z1-9_]*)\\>");
    std::smatch search;
	if(!std::regex_match(TYPENAME, search, match)) return false;
    if(search.size()!= 3) return false;
    fieldType  = search[1];
    fieldSpace = search[2];
	return true;
}

pFlow::uniquePtr<pFlow::postprocessData::fieldsDataBase> 
    pFlow::postprocessData::fieldsDataBase::create
(
    systemControl& control, 
    const dictionary& postDict, 
    bool inSimulation,
    timeValue startTime
)
{
    word dbType;
    if(inSimulation)
    {
        dbType = "fieldsDataBase<simulation>";
    }
    else
    {
        dbType = "fieldsDataBase<postSimulation>";
    }
    
    if( boolvCtorSelector_.search(dbType) )
    {
        auto objPtr = 
            boolvCtorSelector_[dbType](control, postDict, inSimulation, startTime);
        return objPtr;
    }
    else
    {
        printKeys
        ( 
            fatalError << "Ctor Selector "<< 
            dbType << " does not exist. \n"
            <<"Available ones are: \n\n"
            ,
            boolvCtorSelector_
        );
        fatalExit;
    }
    
    return nullptr;
}



