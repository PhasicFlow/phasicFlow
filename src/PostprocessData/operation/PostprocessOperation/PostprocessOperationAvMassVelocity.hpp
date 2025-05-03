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

#ifndef __PostprocessOperationAvMassVelocity_hpp__
#define __PostprocessOperationAvMassVelocity_hpp__

/*!
 * @class PostprocessOperationAvMassVelocity
 * @brief A class for averaging field values within specified regions during post-processing.
 * 
 * @details
 * The PostprocessOperationAvMassVelocity class is a specialized post-processing operation that 
 * calculates the average of field values within specified regions. It inherits from the 
 * postprocessOperation base class and implements a weighted averaging operation that 
 * can be applied to scalar (real), vector (realx3), and tensor (realx4) fields.
 * 
 * The average operation follows the mathematical formula:
 * \f[
 *    \text{result} = \frac{\sum_{j \in \text{includeMask}} w_j \cdot \phi_j \cdot \text{field}_j}
 *                        {\sum_{i \in \text{processRegion}} w_i \cdot \phi_i}
 * \f]
 * 
 * Where:
 * - \f$ i \f$ represents all particles within the specified processing region
 * - \f$ j \f$ belongs to a subset of \f$ i \f$ based on an includeMask
 * - \f$ w_i \f$ is the weight factor for particle \f$ i \f$
 * - \f$ \phi_i \f$ is the value from the phi field for particle \f$ i \f$
 * - \f$ \text{field}_j \f$ is the value from the target field for particle \f$ j \f$
 * 
 * The calculation can optionally be divided by the region volume (when divideByVolume is set to yes),
 * which allows calculating normalized averages:
 * \f[
 *    \text{result} = \frac{1}{V_{\text{region}}} \frac{\sum_{j \in \text{includeMask}} w_j \cdot \phi_j \cdot \text{field}_j}
 *                        {\sum_{i \in \text{processRegion}} w_i \cdot \phi_i}
 * \f]
 * 
 * The averaging can be further filtered using an includeMask to selectively include only
 * specific particles that satisfy certain criteria.
 * 
 * This class supports the following field types:
 * - real (scalar values)
 * - realx3 (vector values)
 * - realx4 (tensor values)
 * 
 * @section usage Usage Example
 * Below is a sample dictionary showing how to configure and use this class:
 *
 * ```
 * processMethod    arithmetic;     // method of performing the sum (arithmetic, uniformDistribution, GaussianDistribution)
 * processRegion    sphere;         // type of region on which processing is performed 
 * 
 * sphereInfo
 * {
 *     radius 0.01;
 *     center (-0.08 -0.08 0.015);
 * }
 * 
 * timeControl default; 
 * 
 * /// all the post process operations to be done 
 * operations
 * (
 *     // computes the arithmetic mean of particle velocity 
 *     averageVel
 *     {
 *         function         average;
 *         field            velocity;
 *         dividedByVolume  no;     // default is no
 *         threshold        3;      // default is 1
 *         includeMask      all;    // include all particles in the calculation
 *     }
 *     
 *     // computes the fraction of par1 in the region 
 *     par1Fraction
 *     {
 *         function        average;
 *         field           one;     // the "one" field is special - all members have value 1.0
 *         phi             one;     // default is "one"
 *         dividedByVolume no;
 *         includeMask     lessThan;
 *         
 *         // diameter of par1 is 0.003, so these settings 
 *         // will select only particles of type par1
 *         lessThanInfo
 *         {
 *             field      diameter;
 *             value      0.0031;  
 *         }
 *     }
 * );
 * ```
 *
 * @section defaults Default Behavior
 * - By default, `phi` is set to the field named "one" which contains value 1.0 for all entries
 * - `dividedByVolume` is set to "no" by default
 * - `threshold` is set to 1 by default
 * - `includeMask` can be set to various filters, with "all" being the default to include all particles
 *
 * @section special Special Fields
 * The field named "one" is a special field where all members have the value 1.0. This makes it
 * particularly useful for calculating:
 * 
 * 1. Volume or number fractions (as shown in the par1Fraction example)
 * 2. Simple counts when used with an appropriate mask
 * 3. Normalizing values by particle count
 *
 * @see postprocessOperation
 * @see executeAverageOperation
 */

#include <variant>
#include <vector>

#include "PostprocessOperationAverage.hpp"
#include "regionField.hpp"
#include "includeMask.hpp"

namespace pFlow::postprocessData
{


class PostprocessOperationAvMassVelocity
:
    public PostprocessOperationAverage
{
   
public:

    TypeInfo("PostprocessOperation<avMassVelocity>");

    /// @brief Constructs average operation processor
    /// @param opDict Operation parameters dictionary
    /// @param regPoints Region points data
    /// @param fieldsDB Fields database
    PostprocessOperationAvMassVelocity(
        const dictionary& opDict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB);
    
    /// destructor 
    ~PostprocessOperationAvMassVelocity() override = default;

    /// add this virtual constructor to the base class
    add_vCtor
    (
        postprocessOperation,
        PostprocessOperationAvMassVelocity,
        dictionary
    );

}; 


} // namespace pFlow::postprocessData

#endif //__PostprocessOperationAvMassVelocity_hpp__