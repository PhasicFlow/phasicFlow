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
#ifndef __postprocessOperation_hpp__
#define __postprocessOperation_hpp__

/*! 
 *  @class postprocessOperation
 *  @file postprocessOperation.hpp
 *  @brief Base class for post-processing operations on particle data.
 *         This class provides the foundational structure and functionality
 *         for performing various post-processing operations on simulation data.
 *
 *  @details
 *  The postprocessOperation class operates on field data (specified in the input dictionary)
 *  and performs specific operations on that field within defined regions. It serves as
 *  part of the post-processing framework in phasicFlow to analyze particle simulation results.
 *  
 *  Operations are performed on specific subsets of particles defined by region points and
 *  can be filtered using include masks. The class supports different field types (real,
 *  realx3, realx4) through the processedRegFieldType variant.
 *  
 *  The main operations supported include:
 *
 *  1. Sum operation:
 *     - Calculates: 
 *       \f[
 *          \text{result} = \sum_{i \in \text{processRegion}} w_i \cdot \phi_i \cdot \text{field}_i
 *       \f]
 *     - Where \f$ i \f$ belongs to the particles in the specified processRegion
 *     - \f$ w_i \f$ is the weight factor for particle \f$ i \f$
 *     - \f$ \phi_i \f$ is the value from the phi field for particle \f$ i \f$
 *     - \f$ \text{field}_i \f$ is the value from the target field for particle \f$ i \f$
 *     - Implemented in the derived class PostprocessOperationSum
 *
 *  2. Average operation:
 *     - Calculates: 
 *       \f[
 *          \text{result} = \frac{\sum_{j \in \text{includeMask}} w_j \cdot \phi_j \cdot \text{field}_j}
 *                              {\sum_{i \in \text{processRegion}} w_i \cdot \phi_i}
 *       \f]
 *     - Where \f$ i \f$ belongs to all particles in the specified processRegion
 *     - \f$ j \f$ belongs to a subset of \f$ i \f$ based on an includeMask defined in the input dictionary
 *     - This allows calculating regional averages on specific subsets of particles
 *
 *  The class uses threshold values to exclude regions with insufficient particles
 *  and supports optional division by volume for density-like calculations. Results are written
 *  to files for later analysis or visualization.
 *
 *  @note The actual processing is performed by derived classes that implement
 *  the execute() method for specific operation types.
 */

#include <variant>

#include "virtualConstructor.hpp"
#include "Logical.hpp"
#include "dictionary.hpp"
#include "span.hpp"
#include "oFstream.hpp"
#include "regionField.hpp"
#include "includeMask.hpp"
#include "postprocessOperationFunctions.hpp"

namespace pFlow
{



/// - forward declaration
class fieldsDataBase;
class Time;

class postprocessOperation
{
public:

    using Mask = typename includeMask::Mask;

private:

    /// Dictionary containing operation-specific parameters.
    dictionary              operationDict_;

    /// This Threshold is used to exclude the regions which contain
    /// fewer than this value. 
    uint32                  threshold_;
    
    /// Logical flag to determine if the result is divided by volume.
    Logical                 divideByVolume_;

    /// Reference to the region points used in the operation.
    const regionPoints&     regionPoints_;

    /// Reference to the fields database containing field data.
    fieldsDataBase&         database_;

    /// Name of the field to be processed.
    word                    fieldName_;

    /// Type of the field to be processed.
    word                    fieldType_;

    /// Name of the phi field to be processed.
    word                    phiFieldName_;

    /// Pointer to the include mask used for masking operations.
    uniquePtr<includeMask>  includeMask_ = nullptr;

    mutable uniquePtr<oFstream>     osPtr_ = nullptr;
    
public:

    /// Type info
    TypeInfo("postprocessOperation");

    /// Constructor
    /// @param opDict Dictionary containing operation-specific parameters.
    /// @param regPoints Reference to the region points used in the operation.
    /// @param fieldsDB Reference to the fields database containing field data.
    postprocessOperation(
        const dictionary& opDict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB );
    
    postprocessOperation(
        const dictionary& opDict,
        const word& fieldName,
        const word& phiName,
        const word& includeName,  
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB
    );
    
    /// destructor
    virtual ~postprocessOperation()=default;
    
    /// Active the virtual constructor for creating derived classes.
    create_vCtor(
        postprocessOperation,
        dictionary,
        (
            const dictionary& opDict,
            const regionPoints& regPoints,
            fieldsDataBase& fieldsDB
        ),
        (opDict, regPoints, fieldsDB));
    
    /// Access to regionPoints instance 
    const regionPoints& regPoints()const
    {
        return regionPoints_;
    }

    /// Access to fields database instance
    const fieldsDataBase& database()const
    {
        return database_;
    }

    /// Access to fields database instance
    fieldsDataBase& database()
    {
        return database_;
    }

    /// Access to the time instance
    const Time& time()const;

    /// Return the name of the processed field.
    word processedFieldName()const
    {
        return operationDict_.name();
    }
    
    /// return the name of the field to be processed.
    const word& fieldName()const
    {
        return fieldName_;
    }

    /// return the type name of the field to be processed.
    const word& fieldType()const
    {
        return fieldType_;
    }

    /// return the name of the phi field to be processed.
    const word& phiFieldName()const
    {
        return phiFieldName_;
    }

    /// Access to the operation dictionary 
    const dictionary& operationDict()const
    {
        return operationDict_;
    }

    /// return threshold value 
    /// which is used to exclude the regions which contain
    /// particles fewer than this value.
    const uint32 threshold()const
    {
        return threshold_;
    }

    /// whether the result is divided by volume of the region 
    bool divideByVolume()const
    {
        return divideByVolume_();
    }

    /// return the include mask
    Mask getMask()
    {
        return includeMask_().getMask();
    }

    /// return the processed field
    virtual
    const processedRegFieldType& processedField()const=0;

    /// execute the operation
    /// @param weights Vector of weights for the operation.
    /// @param volFactor a factor to be multiplied by the volume of the region
    virtual bool execute(
        const std::vector<span<real>>& weights, 
        const regionField<real>& volFactor) = 0;

    /// write the result to a file 
    /// @param parDir Parent directory for the output file.
    virtual 
    bool write(const fileSystem &parDir)const;

    /// write the result to output stream (possibly a file)
    /// @param os Output stream to write the result.
    virtual 
    bool write(iOstream& os)const {return true;}

    /// Create the polymorphic object using the virtual constructor.
    /// @param opDict Dictionary containing operation-specific parameters.
    /// @param regPoints Reference to the region points used in the operation.
    /// @param fieldsDB Reference to the fields database containing field data.
    static
    uniquePtr<postprocessOperation> create(
        const dictionary& opDict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB);
    
};

}

#endif //__postprocessOperation_hpp__