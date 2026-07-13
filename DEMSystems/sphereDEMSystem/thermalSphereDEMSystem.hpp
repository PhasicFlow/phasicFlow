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
-----------------------------------------------------------------------------*/

#ifndef __thermalSphereDEMSystem_hpp__
#define __thermalSphereDEMSystem_hpp__

#include "sphereDEMSystem.hpp"
#include "thermalSphereParticles.hpp"
#include "thermalInteraction.hpp"
#include "thermalProperty.hpp"
#include "thermalSphereShape.hpp"

namespace pFlow
{

/**
 * @brief Extends the base mechanical DEM solver to handle thermodynamic 
 * physics (Conduction, PFP, and Radiation).
 */
class thermalSphereDEMSystem : public sphereDEMSystem
{
protected:

    // --- Section 1: Core Physics Components ---

    /// @brief Direct host-view access for thermal particles.
    thermalSphereParticles* thermalParticles_ = nullptr;

    /// @brief Manages inter-particle conduction, fluid-bridge heat, and radiation.
    uniquePtr<thermalInteraction> thermalInteraction_ = nullptr;

    /**
     * @brief Core integration loop encompassing mechanical and thermal updates.
     * @return True upon successful execution.
     */
    bool loop();

public:

    /// @brief Runtime type identifier for factory allocation.
    TypeInfo("thermalSphereDEMSystem");

    // --- Section 2: Constructors and Destructors ---

    thermalSphereDEMSystem
    (
        word                    demSystemName,
        const std::vector<box>& domains,
        int                     argc, 
        char* argv[],
        bool                    requireRVel = false
    );

    ~thermalSphereDEMSystem() override = default;

    add_vCtor
    (
        DEMSystem,
        thermalSphereDEMSystem, 
        word
    );

    // --- Section 3: Time Integration Controls ---

    bool iterate
    (
        real upToTime, 
        real timeToWrite, 
        word timeName
    ) override;

    bool iterate(real upToTime) override;

    // --- Section 4: Thermodynamic Data Exchange Spans ---

    span<real>   temperature() override;
    span<real>   emissivity() override;
    span<real>   radSumTemp() override;
    span<uint32> radNumPrt() override;
    span<real>   parFluidHeatSourceConv() override;
    span<real>   parFluidHeatSourceRad() override;
    
    bool         sendFluidHeatSourcesToDEM() override;

    // --- Section 5: PFP Sub-grid Model Pipeline ---

    span<real>   parFluidKappa() override;
    span<real>   parFluidAlpha() override;
    bool         sendFluidPropertiesToDEM() override;
    
    // --- Section 6: Logic Flags ---

    /**
     * @brief Evaluates radiation module availability.
     * @return True if the module is active and enabled by user dictionary.
     */
    bool hasRadiation() const override 
    { 
        return thermalInteraction_ != nullptr && 
               thermalInteraction_->isRadiationEnabled(); 
    }
};

} // namespace pFlow

#endif // __thermalSphereDEMSystem_hpp__



