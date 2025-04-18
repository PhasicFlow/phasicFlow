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

#ifndef __PostprocessOperationSum_hpp__
#define __PostprocessOperationSum_hpp__

/*!
 * @class PostprocessOperationSum
 * @brief A class for summing field values within specified regions during post-processing.
 * 
 * @details
 * The PostprocessOperationSum class is a specialized post-processing operation that 
 * calculates the sum of field values within specified regions. It inherits from the 
 * postprocessOperation base class and implements a weighted summation operation that 
 * can be applied to scalar (real), vector (realx3), and tensor (realx4) fields.
 * 
 * The sum operation follows the mathematical formula:
 * \f[
 *    \text{result} = \sum_{i \in \text{processRegion}} w_i \cdot \phi_i \cdot \text{field}_i
 * \f]
 * 
 * Where:
 * - \f$ i \f$ represents particles within the specified processing region
 * - \f$ w_i \f$ is the weight factor for particle \f$ i \f$
 * - \f$ \phi_i \f$ is the value from the phi field for particle \f$ i \f$
 * - \f$ \text{field}_i \f$ is the value from the      target field for particle \f$ i \f$
 * 
 * The calculation can optionally be divided by the region volume (when divideByVolume is set to yes),
 * which allows calculating density-like quantities:
 * \f[
 *    \text{result} = \frac{1}{V_{\text{region}}} \sum_{i \in \text{processRegion}} w_i \cdot \phi_i \cdot \text{field}_i
 * \f]
 * 
 * The summation can be further filtered using an includeMask to selectively include only
 * specific particles that satisfy certain criteria.
 * 
 * This class supports the following field types:
 * - real (scalar values)
 * - realx3 (vector values)
 * - realx4 (tensor values)
 * 
 * @section usage Usage
 * 
 * To use the PostprocessOperationSum class in a postprocessDataDict file, the following
 * parameters can be specified:
 * 
 * - function: Must be set to "sum" to use this operation
 * - field: The name of the field to process (e.g., "velocity", "diameter", "one")
 *   - Special fields like "one" (constant value 1) are also supported
 *   - Expressions like "cube(diameter)" can be used for mathematical operations
 * - dividedByVolume: Whether to divide the sum by the region volume (yes/no, default: no)
 * - includeMask: Optional mask to filter which particles to include in the calculation
 * 
 * @section example Example Configuration
 * 
 * Here is an example configuration in the postprocessDataDict file:
 * 
 * @code
 * {
 *     processMethod    arithmetic; 
 *     processRegion    line;
 *     
 *     // the time interval for executing the post-processing 
 *     // other options: timeStep, default, and settings
 *     timeControl simulationTime; 
 *     startTime   1.0;
 *     endTime     3.0;
 *     executionInterval 0.1;
 *      
 *     // 10 spheres with radius 0.01 along the straight line defined by p1 and p2 
 *     lineInfo
 *     {
 *         p1 (0 0 0);
 *         p2 (0 0.15 0.15);
 *         numPoints 10;
 *         radius    0.01;
 *     }
 *     
 *     operations
 *     (
 *         // computes the number density (particles per unit volume)
 *         numberDensity
 *         {
 *             function       sum;
 *             field          one;        // constant field with value 1.0
 *             dividedByVolume yes;       // divide by region volume
 *         }
 *         
 *         // computes an approximation of volume fraction
 *         volumeDensity
 *         {
 *             function        sum;
 *             field           cube(diameter); // d^3, although it differs by pi/6
 *             dividedByVolume yes;
 *         }
 *     );
 * }
 * @endcode
 * 
 * In this example:
 * - numberDensity: Calculates the number of particles per unit volume
 * - volumeDensity: Calculates an approximation of the volume fraction using d³
 * 
 * @see postprocessOperation
 * @see executeSumOperation
 */

#include <variant>
#include <vector>

#include "postprocessOperation.hpp"
#include "regionField.hpp"
#include "includeMask.hpp"

namespace pFlow
{


class PostprocessOperationSum
:
    public postprocessOperation
{
private:
    /// Result field containing sums for each region (real, realx3, or realx4)
    uniquePtr<processedRegFieldType>  processedRegField_ = nullptr;

public:

    TypeInfo("PostprocessOperation<sum>");

    /// @brief Constructs sum operation processor
    /// @param opDict Operation parameters dictionary
    /// @param regPoints Region points data
    /// @param fieldsDB Fields database
    PostprocessOperationSum(
        const dictionary& opDict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB);
    
    /// destructor 
    ~PostprocessOperationSum() override = default;

    /// add this virtual constructor to the base class
    add_vCtor
    (
        postprocessOperation,
        PostprocessOperationSum,
        dictionary
    );

    /// @brief Get the processed field containing regional sums
    /// @return Const reference to sum results
    const processedRegFieldType& processedField()const override
    {
        return processedRegField_();
    }

    /// @brief Execute sum operation on field values
    /// @param weights Weight factors for particles
    /// @return True if successful
    bool execute(
        const std::vector<span<real>>& weights,
        const regionField<real>& volFactor) override;

}; 


}

#endif //__PostprocessOperationSum_hpp__