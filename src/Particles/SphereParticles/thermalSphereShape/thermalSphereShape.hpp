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

/**
 * @class thermalSphereShape
 * @brief Maps global material thermal properties to specific discrete 
 *        particle shapes.
 *
 * @details
 * While `thermalProperty` acts as a global database of material properties
 * (e.g., "Steel", "Glass"), this class assigns those macroscopic properties
 * to specific geometric entities (e.g., "Small_Steel", "Large_Glass").
 * It inherits from the mechanical `sphereShape` and adds thermodynamic data 
 * arrays (Cp, k, emissivity, E0, nu) tailored to the number of defined shapes 
 * in the simulation.
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

        /**
         * @brief Initial temperature [K] assigned to newly inserted particles.
         *
         * When a batch of particles is inserted and no existing particles are
         * present to sample from, this value is used as the initial 
         * temperature.
         *
         * Read from the property dictionary key 'insertionTemperature', which
         * is mandatory: the case file must state this value explicitly rather
         * than relying on a built-in default.
         */
        real        insertionTemperature_ = real(300);

    //- private methods

        /**
         * @brief Populates the shape-specific thermal arrays.
         * @details Reads the master material properties from the simulation 
         * dictionary and maps them to the local shape arrays using the 
         * shape-to-material ID index.
         * @return True if mapping is successful.
         */
        bool readThermalProperties();

protected:

    //- protected methods

        /**
         * @brief Serializes both mechanical and thermal shape data to a 
         * dictionary.
         * @param dict The target phasicFlow dictionary object.
         * @return True if all data is successfully written.
         */
        bool writeToDict(dictionary& dict) const override;

public:

    //- constructors

        thermalSphereShape(
            const word&     fileName,
            repository*     owner,
            const property& prop);

        thermalSphereShape(
            const word&     shapeType,
            const word&     fileName,
            repository*     owner,
            const property& prop);

        ~thermalSphereShape() override = default;

    //- public methods

        // ================================================================= //
        // Accessor Methods (Vector Level)
        // ================================================================= //

        inline
        realVector heatCapacities() const
        {
            return cp_;
        }
        
        inline
        realVector heatConductivities() const
        {
            return k_;
        }
        
        inline
        realVector emissivities() const
        {
            return emissivity_;
        }
        
        inline
        realVector realYoungsModuli() const
        {
            return E0_;
        }
        
        inline
        realVector poissonRatios() const
        {
            return nu_;
        }

        // ================================================================= //
        // Accessor Methods (Scalar Level for specific shape indices)
        // ================================================================= //

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

        /**
         * @brief Initial temperature for newly inserted particles [K].
         *
         * Used by thermalSphereParticles::insertParticles() when the 
         * temperature field is empty (first insertion event) and no existing 
         * particle can be sampled from. Configured via the mandatory 
         * 'insertionTemperature' key in the property dictionary.
         */
        inline
        real insertionTemperature() const
        {
            return insertionTemperature_;
        }

        add_vCtor(shape, thermalSphereShape, word);

}; // thermalSphereShape

} // pFlow

#endif // pFlow_thermalSphereShape_hpp



