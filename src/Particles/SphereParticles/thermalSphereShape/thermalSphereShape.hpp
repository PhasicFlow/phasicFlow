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

#ifndef pFlow_thermalSphereShape_hpp
#define pFlow_thermalSphereShape_hpp

#include "sphereShape.hpp"

namespace pFlow
{

class thermalProperty;

/**
 * @class thermalSphereShape
 * @brief Binds per-material thermal properties to discrete particle shapes.
 */
class thermalSphereShape
:
    public sphereShape
{
public:

    //- Type info

        TypeInfo("shape<thermalSphere>");

private:

    //- private members

        /// @brief Heat capacity mapped to each specific particle shape 
        /// [J/(kg.K)].
        realVector  cp_;

        /// @brief Thermal conductivity mapped to each specific particle shape 
        /// [W/(m.K)].
        realVector  k_;

        /// @brief Surface emissivity mapped to each specific particle shape 
        /// (dimensionless).
        realVector  emissivity_;

        /// @brief Real Young's Modulus mapped to each specific particle shape 
        /// [Pa].
        realVector  E0_;

        /// @brief Poisson's ratio mapped to each specific particle shape 
        /// (dimensionless).
        realVector  nu_;

        /// @brief Ambient fluid thermal conductivity [W/(m.K)], copied
        /// from thermalProperty for the standalone (no-CFD-mesh) PFP
        /// fallback. See thermalProperty::ambientFluidKappa_.
        real        ambientFluidKappa_ = real(0);

        /// @brief Ambient fluid volume fraction (porosity) [-], same
        /// purpose as ambientFluidKappa_.
        real        ambientFluidAlpha_ = real(0);

    //- private methods

        /**
         * @brief Populates the shape-specific thermal arrays.
         * @details Reads the master material properties directly from
         * the given thermalProperty object and maps them to the local
         * shape arrays using the shape-to-material ID index.
         * @return True if mapping is successful.
         */
        bool readThermalProperties(const thermalProperty& prop);

public:

    //- constructors

        thermalSphereShape(
            const word&             fileName,
            repository*             owner,
            const thermalProperty&  prop);

        thermalSphereShape(
            const word&             shapeType,
            const word&             fileName,
            repository*             owner,
            const thermalProperty&  prop);

        ~thermalSphereShape() override = default;

    //- public methods

        inline
        const realVector& heatCapacities() const
        {
            return cp_;
        }
        
        inline
        const realVector& heatConductivities() const
        {
            return k_;
        }
        
        inline
        const realVector& emissivities() const
        {
            return emissivity_;
        }
        
        inline
        const realVector& realYoungsModuli() const
        {
            return E0_;
        }
        
        inline
        const realVector& poissonRatios() const
        {
            return nu_;
        }

        inline
        real heatCapacity(uint32 i) const
        {
            return cp_[i];
        }
        
        inline
        real heatConductivity(uint32 i) const
        {
            return k_[i];
        }
        
        inline
        real emissivity(uint32 i) const
        {
            return emissivity_[i];
        }
        
        inline
        real realYoungsModulus(uint32 i) const
        {
            return E0_[i];
        }
        
        inline
        real poissonRatio(uint32 i) const
        {
            return nu_[i];
        }

        /// @brief Ambient fluid thermal conductivity [W/(m.K)] for the
        /// standalone-mode PFP fallback (see thermalProperty).
        inline
        real ambientFluidKappa() const
        {
            return ambientFluidKappa_;
        }

        /// @brief Ambient fluid volume fraction [-] for the
        /// standalone-mode PFP fallback (see thermalProperty).
        inline
        real ambientFluidAlpha() const
        {
            return ambientFluidAlpha_;
        }

}; // thermalSphereShape

} // pFlow

#endif // pFlow_thermalSphereShape_hpp




