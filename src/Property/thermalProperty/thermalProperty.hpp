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

#ifndef pFlow_thermalProperty_hpp
#define pFlow_thermalProperty_hpp

#include "property.hpp"

namespace pFlow
{

/**
 * @class thermalProperty
 * @brief Manages macroscopic thermal properties of materials in the simulation.
 *
 * @details
 * Acts as the centralized database for thermodynamic material properties 
 * (e.g., heat capacity, conductivity, emissivity) loaded from the 
 * case dictionaries. It ensures that the mechanical and thermal 
 * definitions of materials remain strictly synchronized.
 */
class thermalProperty
:
    public property
{
public:

    //- Type info

        TypeInfo("thermalProperty");

private:

    //- private members

        realVector          heatCapacities_;
        
        realVector          heatConductivities_;
        
        realVector          emissivities_;
        
        realVector          realYoungsModuli_;
        
        realVector          poissonRatios_;

        /// @brief Ambient fluid thermal conductivity [W/(m.K)], used as a
        /// uniform PFP input for particles when no CFD mesh exists to
        /// sample fluidKappa from (standalone DEM-only solvers). Read
        /// from the optional 'fluidProperties' sub-dictionary in
        /// thermoPhysicalInteraction; defaults to 0 (matching the
        /// pre-existing standalone behaviour where PFP contributes
        /// nothing) when that sub-dictionary is absent.
        real                 ambientFluidKappa_ = real(0);

        /// @brief Ambient fluid volume fraction (porosity) [-], used as
        /// a uniform PFP input in standalone mode. Same source and
        /// default rules as ambientFluidKappa_.
        real                 ambientFluidAlpha_ = real(0);

    //- private methods

        bool readDictionary();
        
        bool writeDictionary();

protected:

    //- protected members

        /// @brief Non-owning pointer to the case's directory, used by
        /// readDictionary() to locate thermoPhysicalInteraction.
        /// Lifetime requirement: the fileSystem the (fileName, dir)
        /// constructor is given must outlive this thermalProperty
        /// object. Currently safe because readDictionary() only runs
        /// once, from that constructor's body, before p_dir_ could
        /// ever be read after the referenced object's lifetime ends --
        /// but this becomes a dangling-pointer risk the moment any
        /// future code (here or in a derived class, since this member
        /// is protected) calls readDictionary() again, or otherwise
        /// dereferences p_dir_, after construction.
        const fileSystem*   p_dir_ = nullptr;

public:

    //- constructors

        explicit thermalProperty(
            const word&         fileName,
            repository*         owner = nullptr);
        
        thermalProperty(
            const word&         fileName,
            const fileSystem&   dir);
        
        thermalProperty(
            const word&         fileName,
            const wordVector&   materials,
            const realVector&   densities,
            const realVector&   heatCapacities,
            const realVector&   heatConductivities,
            const realVector&   emissivities,
            const realVector&   realYoungsModuli,
            const realVector&   poissonRatios,
            repository*         owner = nullptr);

        ~thermalProperty() override = default;

    //- public methods

        //- vector accessors

        inline
        const auto& heatCapacities() const
        {
            return heatCapacities_;
        }
        
        inline
        const auto& heatConductivities() const
        {
            return heatConductivities_;
        }
        
        inline
        const auto& emissivities() const
        {
            return emissivities_;
        }
        
        inline
        const auto& realYoungsModuli() const
        {
            return realYoungsModuli_;
        }
        
        inline
        const auto& poissonRatios() const
        {
            return poissonRatios_;
        }
        
        //- scalar accessors

        inline
        real heatCapacity(uint32 i) const
        {
            return heatCapacities_[i];
        }
        
        inline
        real heatConductivity(uint32 i) const
        {
            return heatConductivities_[i];
        }
        
        inline
        real emissivity(uint32 i) const
        {
            return emissivities_[i];
        }
        
        inline
        real realYoungsModulus(uint32 i) const
        {
            return realYoungsModuli_[i];
        }
        
        inline
        real poissonRatio(uint32 i) const
        {
            return poissonRatios_[i];
        }

        /// @brief Ambient fluid thermal conductivity [W/(m.K)] for the
        /// standalone-mode PFP fallback. See ambientFluidKappa_.
        inline
        real ambientFluidKappa() const
        {
            return ambientFluidKappa_;
        }

        /// @brief Ambient fluid volume fraction [-] for the
        /// standalone-mode PFP fallback. See ambientFluidAlpha_.
        inline
        real ambientFluidAlpha() const
        {
            return ambientFluidAlpha_;
        }

}; // thermalProperty

} // pFlow

#endif // pFlow_thermalProperty_hpp


