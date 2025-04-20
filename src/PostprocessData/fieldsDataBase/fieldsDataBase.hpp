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

#ifndef __fieldsDataBase_hpp__
#define __fieldsDataBase_hpp__

#include "fieldsDataBaseDclr.hpp"

#include "pointStructure.hpp"
#include "pointFields.hpp"
#include "anyList.hpp"
#include "Map.hpp"
#include "shape.hpp"


namespace pFlow
{

class systemControl;
class Time;


class fieldsDataBase
{
private:

    // - Typedefs

        /// Point position data type
        using PointsTypeHost    = typename pointStructure::PointsTypeHost;
        
        /// Point field data type 
        template<typename T>
        using FieldTypeHost     = typename internalField<T>::FieldTypeHost;

        /// @brief Enumeration of functions that can be applied to point fields.
        enum class Functions
        {
            None,       /// no function
            ComponentX, /// component(u,x)
            ComponentY, /// component(u,y)
            ComponentZ, /// component(u,z)
            ComponentW, /// component(u,w)
            Abs,        /// abs(s)
            Square,     /// square(s)
            Cube,       /// cube(s)
            SqureRoot,  /// sqrt(s)
            Magnitude,          /// mag(u)
            MagnitudeSquare,    /// magSquare(u)
            MagnitudeCube,      /// magCube(u)
            MagnitudeSquareRoot /// magSqrt(u)
        };


    // - Member variables

        /// List to store all the point fields 
        anyList             allFields_;

        /// Map to store the last capture time of each field
        wordMap<timeValue>  captureTime_;

        /// Reference to the Time object
        Time&               time_; 

        /// Flag indicating if we're in simulation mode
        bool                inSimulation_ = false;

protected:

        /// Map of reserved field names with their corresponding types
        static const inline std::map<word, word> reservedFieldNames_
        {
            {"position", "realx3"},
            {"one", "real"},
            {"volume", "real"},
            {"density", "real"}
        };

        /// check if pointField name exists in Time or time folder 
        virtual 
        bool pointFieldNameExists(const word& name)const = 0;
        
        /// Loads a pointField with a given name to the Time object 
        virtual 
        bool loadPointFieldToTime(const word& name)= 0;

        virtual 
        bool loadPointStructureToTime()=0;

        /// get the type name of the pointField in the Time object 
        word getPointFieldType(const word& name)const;

        /// Checks if a field needs to be updated based on capture time
        bool checkForUpdate(const word& compoundName, bool forceUpdate = false);

        template<ValidFieldType T>
        span<T> updateField(const word& name, bool forceUpdate = false);

        template<ValidFieldType T>
        span<T> updateReservedField(const word& name, bool forceUpdate = false);
        
        span<real> createOrGetRealField(const word& name);

        span<real> createOrGetVolume(bool forceUpdate=false);

        span<real> createOrGetDensity(bool forceUpdate=false);

        span<real> createOrGetOne(bool forceUpdate=false);

        static 
        bool findFunction(
            const word& compoundFieldName, 
            word& fieldName, 
            fieldsDataBase::Functions& func );
        
        static
        bool inputOutputType(
            fieldsDataBase::Functions func, 
            const word& inputType, 
            word& outputType);


protected:
    
    // - protected member functions
    

    virtual 
    bool checkTimeFolder(const word& fieldName) const = 0;
    
    virtual 
    const shape& getShape() const= 0;


    /// @brief return the size of pointStructure 
    uint32 pointFieldSize()
    {
        auto s = updatePoints();
        return s.size();
    }

public:
    
    // - Type info
            
        TypeInfo("fieldsDataBase");

    // - constructors 

        fieldsDataBase(systemControl& control, bool inSimulation);

        /// no copy constructor
        fieldsDataBase(const fieldsDataBase&) = delete;
        
        /// no move constructor
        fieldsDataBase(fieldsDataBase&&) = delete;
        
        /// no copy assignment
        fieldsDataBase& operator=(const fieldsDataBase&) = delete;
        
        /// no move assignment
        fieldsDataBase& operator=(fieldsDataBase&&) = delete;
        
        /// destructor
        virtual ~fieldsDataBase() = default;

        create_vCtor
        (
            fieldsDataBase,
            bool,
            (systemControl& control, bool inSimulation),
            (control, inSimulation)
        );
  
    
    // - Public Access Functions
        /// returns the current time
        timeValue currentTime()const;

        /// const ref to object Time 
        const Time& time()const 
        {
            return time_;
        }
        
        Time& time()
        {
            return time_;
        }

    // - Public Member Functions

        bool getFieldTypeNameFunction
        (
            const word& compoundName,
            word& pointFieldName, 
            word& originalType, 
            word& typeAfterFunction,
            Functions& func
        )const;

        
        bool getFieldType
        (
            const word& compoundName,
            word& originalType, 
            word& typeAfterFunction
        ) const;

        bool getFieldType
        (
            const word& compoundName,
            word& typeAfterFunction
        ) const;

        
        /// update pointStructure if necessary 
        span<realx3> updatePoints(bool forceUpdate = false);
        
        /// update a field with realx3 type 
        span<realx3> updateFieldRealx3(
            const word& compoundName, 
            bool forceUpdate = false);
        
        span<realx4> updateFieldRealx4(
            const word& compoundName, 
            bool forceUpdate = false);
        
        span<real> updateFieldReal(
            const word& compoundName, 
            bool forceUpdate = false);
        
        span<uint32> updateFieldUint32(
            const word& name,
            bool forceUpdate = false);
    
        /// Updates and retrieves a point field of any type from the database.
        /// It returns types real, realx3 and realx4 only.
        allPointFieldTypes updateFieldAll(
            const word& compoundName, 
            bool forceUpdate = false);
        
        virtual 
        const pointStructure& pStruct()const = 0;

        virtual 
        void resetTimeFolder() = 0;
    
    static
    uniquePtr<fieldsDataBase> create(
        systemControl& control, 
        bool inSimulation);
};

} // namespace pFlow

#include "fieldsDataBaseTemplates.cpp"

#endif //__fieldsDataBased_hpp__
