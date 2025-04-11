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

#include <variant>
#include <concepts>


#include "pointStructure.hpp"
#include "pointFields.hpp"
#include "anyList.hpp"
#include "Map.hpp"
#include "span.hpp"

namespace pFlow
{

class Time;

bool pointFieldGetType(const word& TYPENAME, word& fieldType, word& fieldSpace);

template<typename T>
concept ValidFieldType = 
    std::same_as<T, real> || 
    std::same_as<T, realx3> || 
    std::same_as<T, realx4> || 
    std::same_as<T, uint32>;

template<typename T> 
concept VectorType = 
    std::same_as<T, realx3> || 
    std::same_as<T, realx4>;

template<typename T>
concept ScalarType = 
    std::same_as<T, real>;


template<typename T>
concept ValidRegionFieldType = 
    std::same_as<T, real> || 
    std::same_as<T, realx3> || 
    std::same_as<T, realx4> ;


using allPointFieldTypes = std::variant<span<real>, span<realx3>, span<realx4>>; 


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
    
        /// const reference to the Time object
        const Time&         time_;

        /// List to store all the point fields 
        anyList             allFields_;

        /// Map to store the last capture time of each field
        wordMap<timeValue>  captureTime_;

        /// 
        bool                inSimulation_ = false;
        
        static const inline std::map<word, word> reservedFieldNames_
        {
            {"position", "realx3"},
            {"one", "real"}
        };
        
    // - Private member functions

        uint32 pointFieldSize()
        {
            auto s = updatePoints();
            return s.size();
        }
        
        /// @brief Checks if a field needs to be updated.
        /// @param name The name of the field to check.
        /// @param forceUpdate Forces an update if true. Defaults to `false`.
        /// @return `true` if the field needs updating or `forceUpdate` is true, `false` otherwise.
        bool checkForUpdate(const word& name, bool forceUpdate = false);

        /**
         * @brief Updates and retrieves a point field of a specified type from the database.
         * 
         * This is a template function that updates and retrieves a point field of type `T`
         * from the database. It checks if the field needs to be updated based on the last
         * capture time or if a forced update is requested. If an update is necessary, it
         * retrieves the latest data for the field.
         * 
         * @tparam T The type of the point field to update and retrieve. Must be a ValidFieldType.
         * @param name The name of the field to update.
         * @param forceUpdate A boolean flag indicating whether to force an update of the field
         *                    regardless of its current state. Defaults to `false`.
         * 
         * @return A span of `T` representing the updated field data.
         * 
         * @throws message If the field cannot be found in the database or if there is a type mismatch.
         */
        template<ValidFieldType T>
        span<T> updateField(const word& name, bool forceUpdate = false);
        
        /// @brief Creates a new real field or retrieves an existing one.
        /// 
        /// If a field with the given name already exists, it returns a span to that field.
        /// If the field does not exist, it creates a new real field with the given name
        /// and returns a span to the newly created field.
        /// 
        /// @param name The name of the field to create or retrieve.
        /// @return span<real> of the field 
        span<real> createOrGetRealField(const word& name);

        /**
         * @brief Parses a compound field name to extract the base field name and function.
         * 
         * This function takes a compound field name, which may include a function applied
         * to a base field (e.g., "mag(velocity)"), and parses it to extract the base field
         * name (e.g., "velocity") and the function to be applied (e.g., `Functions::Magnitude`).
         * 
         * The function supports the following syntax for compound field names:
         * - `fieldName` (no function applied)
         * - `functionName(fieldName)`
         * 
         * Supported function names are defined in the `Functions` enum.
         * 
         * @param compoundFieldName The compound field name to parse.
         * @param fieldName A reference to a `word` where the extracted base field name
         *                  will be stored.
         * @param func A reference to a `Functions` enum where the identified function
         *             will be stored. If no function is applied, this will be set to
         *             `Functions::None`.
         * 
         * @return `true` if the compound field name was successfully parsed and the base
         *         field name and function were extracted, `false` otherwise.
         * 
         * @note The function modifies the `fieldName` and `func` parameters to return the
         *       extracted information.
         */
        static 
        bool findFunction(
            const word& compoundFieldName, 
            word& fieldName, 
            fieldsDataBase::Functions& func );
        
        /**
         * @brief Determines the input and output types for a given function.
         * 
         * This function takes a `Functions` enum value and an input type as a string
         * and determines the corresponding output type based on the function being applied.
         * 
         * @param func The function for which to determine the input and output types.
         * @param inputType The input type as a string.
         * @param outputType A reference to a string where the determined output type will be stored.
         * 
         * @return `true` if the input and output types were successfully determined, `false` otherwise.
         */
        static
        bool inputOutputType(
            fieldsDataBase::Functions func, 
            const word& inputType, 
            word& outputType);

public:
    
    // - constructors 

        fieldsDataBase(const Time& time, bool inSimulation);

        /// no copy constructor
        fieldsDataBase(const fieldsDataBase&) = delete;
        
        /// no move constructor
        fieldsDataBase(fieldsDataBase&&) = delete;
        
        /// no copy assignment
        fieldsDataBase& operator=(const fieldsDataBase&) = delete;
        
        /// no move assignment
        fieldsDataBase& operator=(fieldsDataBase&&) = delete;
        
        /// destructor
        ~fieldsDataBase() = default;
    
    // - Public Access Functions
        /// returns the current time
        timeValue currentTime()const;

        /// const ref to object Time
        const Time& time()const
        {
            return time_;
        }
    
    // - Public Member Functions

        /**
         * @brief Retrieves the type of a point field based on its compound name.
         * 
         * This function attempts to extract the type of a point field from its compound name.
         * The compound name may include additional information such as a function or operation
         * applied to the field, ie. mag(velcoty). If the type is successfully determined, it  
         * is stored in the provided `typeName` parameter.
         * 
         * @param compoundName The compound name of the field, which may include additional
         *                     information about operations or functions applied to the field.
         * @param originalType A reference to a `word` where the original type name is obtained.
         *                     This will be set to the type of the field before any function is applied.
         * @param typeAfterFunction A reference to a `word` where the type name after applying                   
         *                          the function is obtained.
         * @param func the applied function to the field.
         *  
         * @return `true` if the type was successfully determined and stored in `typeName`,
         *         `false` otherwise.
         */
        bool getPointFieldType(
            const word& compoundName,
            word& fieldName, 
            word& originalType, 
            word& typeAfterFunction,
            Functions& func);   
        
        /// overload for the function getPointFieldType without `func` argument
        bool getPointFieldType(
            const word& compoundName, 
            word& originalType, 
            word& typeAfterFunction);
        
        /// overload for function getPointFieldType without `originalType` argument
        bool getPointFieldType(
            const word& compoundName, 
            word& typeAfterFunction);

        /**
         * @brief Updates the points data and returns a span to the updated points.
         * 
         * This function ensures that the points data is up-to-date by checking if an update
         * is necessary. If the data is outdated or if a forced update is requested, it retrieves
         * the latest points data and stores it in the internal fields database. The function
         * then returns a span to the updated points data for further use.
         * 
         * @param forceUpdate A boolean flag indicating whether to force an update of the points
         *                    data regardless of its current state. Defaults to `false`.
         * 
         * @return A span of `realx3` representing the updated points data.
         */
        span<realx3> updatePoints(bool forceUpdate = false);
        
        /**
         * @brief Updates and retrieves a realx3 point field from the database.
         * 
         * This function retrieves or updates a realx3 field based on its compound name.
         * The compound name cannot include any function operation.
         * If the field needs to be updated or if forceUpdate is true, the method will
         * fetch the latest data from the database.
         * 
         * @param compoundName The name of the field, possibly including a function operation
         * @param forceUpdate If true, forces an update of the field regardless of its current state.
         *                    Defaults to false.
         * 
         * @return A span containing the updated realx3 field data
         * 
         * @throws message If the field type is not compatible with realx3 or if the field
         *                 cannot be found in the database
         */
        span<realx3> updateFieldRealx3(
            const word& compoundName, 
            bool forceUpdate = false);
        
        /**
         * @brief Updates and retrieves a realx4 point field from the database.
         * 
         * This function retrieves or updates a realx4 field based on its compound name.
         * The compound name cannot include any function operation.
         * If the field needs to be updated or if forceUpdate is true, the method will
         * fetch the latest data from the database.
         * 
         * @param compoundName The name of the field, possibly including a function operation
         * @param forceUpdate If true, forces an update of the field regardless of its current state.
         *                    Defaults to false.
         * 
         * @return A span containing the updated realx3 field data
         * 
         * @throws message If the field type is not compatible with realx4 or if the field
         *                 cannot be found in the database
         */
        span<realx4> updateFieldRealx4(
            const word& compoundName, 
            bool forceUpdate = false);
        
        /**
         * @brief Updates and retrieves a real point field from the database.
         * 
         * This function retrieves or updates a real field based on its compound name.
         * The compound name may include a function operation (e.g., abs, square, etc.).
         * If the field needs to be updated or if forceUpdate is true, the method will
         * fetch the latest data from the database and apply the specified function.
         * 
         * Supported functions include:
         * - None: Retrieves the field as is.
         * - abs: Computes the absolute value of the field.
         * - square: Computes the square of the field.
         * - cube: Computes the cube of the field.
         * - sqrt: Computes the square root of the field.
         * - mag, magSquare, magCube, magSqrt: Compute magnitude-related operations for vector fields.
         * - component(x, y, z): Extracts a specific component from a vector field.
         * 
         * @param compoundName The name of the field, possibly including a function operation.
         * @param forceUpdate If true, forces an update of the field regardless of its current state.
         *                    Defaults to false.
         * 
         * @return A span containing the updated real field data.
         * 
         * @throws message If the field type is not compatible with real or if the field
         *                 cannot be found in the database.
         */
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

        const pointStructure& pStruct()const;
};

} // nampespace pFlow

#include "fieldsDataBaseTemplates.cpp"

#endif //__fieldsDataBased_hpp__
