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

#include "thermalSphereParticles.hpp"
#include "thermalSphereParticlesKernels.hpp"
#include <iostream>

namespace pFlow
{

//----------------------------- constructors ----------------------------------

thermalSphereParticles::thermalSphereParticles(
    systemControl&              control,
    const thermalSphereShape&   thShpShape)
:
    sphereParticles(control, thShpShape),
    thSpheres_(thShpShape),
    temperature_(
        objectFile(
            "temperature", 
            "",
            objectFile::READ_ALWAYS,
            objectFile::WRITE_ALWAYS),
        dynPointStruct(), 
        300.0),
    Cp_(
        objectFile(
            "Cp", 
            "", 
            objectFile::READ_NEVER, 
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        1.0),
    conductivity_(
        objectFile(
            "conductivity", 
            "", 
            objectFile::READ_NEVER, 
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        1.0),
    heatSourceCondPP_(
        objectFile(
            "heatSourceCondPP", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.0),
    heatSourcePFP_(
        objectFile(
            "heatSourcePFP", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.0),
    radSumTemp_(
        objectFile(
            "radSumTemp", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.0),
    radNumPrt_(
        objectFile(
            "radNumPrt", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        static_cast<uint32>(0)),
    emissivity_(
        objectFile(
            "emissivity", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.85),
    E0_(
        objectFile(
            "E0", 
            "", 
            objectFile::READ_NEVER, 
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        1e9),
    nu_(
        objectFile(
            "nu", 
            "", 
            objectFile::READ_NEVER, 
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.3),
    temperatureRate_(
        objectFile(
            "temperatureRate", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.0),
    fluidKappa_(
        objectFile(
            "fluidKappa", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.0),
    fluidAlpha_(
        objectFile(
            "fluidAlpha", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.0),
    heatTransferTimer_("heatTransfer", &this->timers()),
    temperatureIntegrationTimer_("tempInt", &this->timers())
{
    initializeThermalParticles();
}

//---------------------------- public methods ---------------------------------

bool thermalSphereParticles::initializeThermalParticles()
{
    auto activeMask = this->dynPointStruct().activePointsMaskDevice();

    realVector h_Cp  = thSpheres_.heatCapacities();
    realVector h_K   = thSpheres_.heatConductivities();
    realVector h_Eps = thSpheres_.emissivities();
    realVector h_E0  = thSpheres_.realYoungsModuli();
    realVector h_Nu  = thSpheres_.poissonRatios();

    deviceViewType1D<real> d_Cp ("dCp",  h_Cp.size());
    deviceViewType1D<real> d_K  ("dK",   h_K.size());
    deviceViewType1D<real> d_Eps("dEps", h_Eps.size());
    deviceViewType1D<real> d_E0 ("dE0",  h_E0.size());
    deviceViewType1D<real> d_Nu ("dNu",  h_Nu.size());

    auto m_Cp  = Kokkos::create_mirror_view(d_Cp);
    auto m_K   = Kokkos::create_mirror_view(d_K);
    auto m_Eps = Kokkos::create_mirror_view(d_Eps);
    auto m_E0  = Kokkos::create_mirror_view(d_E0);
    auto m_Nu  = Kokkos::create_mirror_view(d_Nu);

    for (size_t i = 0; i < h_Cp.size(); ++i)
    {
        m_Cp (i) = h_Cp [i];
        m_K  (i) = h_K  [i];
        m_Eps(i) = h_Eps[i];
        m_E0 (i) = h_E0 [i];
        m_Nu (i) = h_Nu [i];
    }

    Kokkos::deep_copy(d_Cp,  m_Cp);
    Kokkos::deep_copy(d_K,   m_K);
    Kokkos::deep_copy(d_Eps, m_Eps);
    Kokkos::deep_copy(d_E0,  m_E0);
    Kokkos::deep_copy(d_Nu,  m_Nu);

    thermalSphereParticlesKernels::initThermalProperties(
        activeMask,
        shapeIndex().deviceViewAll(),
        d_Cp,
        d_K,
        d_Eps,
        d_E0,
        d_Nu,
        Cp_.deviceViewAll(),
        conductivity_.deviceViewAll(),
        emissivity_.deviceViewAll(),
        E0_.deviceViewAll(),
        nu_.deviceViewAll());

    // Ambient fluid properties for the PFP model when no CFD mesh is
    // present to sample fluidKappa_/fluidAlpha_ from (standalone DEM
    // solvers). Harmless when CFD coupling is active: the coupled
    // solver overwrites both fields from the real mesh on its first
    // data exchange, before any thermal kernel uses them.
    fluidKappa_.fill(thSpheres_.ambientFluidKappa());
    fluidAlpha_.fill(thSpheres_.ambientFluidAlpha());

    return true;
}

bool thermalSphereParticles::beforeIteration()
{
    if (!sphereParticles::beforeIteration())
    {
        return false;
    }

    zeroHeatSourceCondPP();
    zeroHeatSourcePFP();
    zeroRadSumTemp();
    zeroRadNumPrt();

    return true;
}

bool thermalSphereParticles::iterate()
{
    if (!sphereParticles::iterate())
    {
        return false;
    }

    iterateThermal();

    return true;
}

void thermalSphereParticles::iterateThermal()
{
    auto mask = dynPointStruct().activePointsMaskDevice();

    // Reset the rate scratch buffer before recomputation. Folded in
    // here rather than beforeIteration() -- unlike heatSourceCondPP_/
    // heatSourcePFP_, temperatureRate_ is never written by another
    // class, so there is no ordering requirement forcing it earlier.
    temperatureRate_.field().fill(0.0);

    heatTransferTimer_.start();

    thermalSphereParticlesKernels::calcFluidParticleHeatTransfer(
        mask,
        diameter().deviceViewAll(),
        mass().deviceViewAll(),
        Cp().deviceViewAll(),
        temperature().deviceViewAll(),
        heatSourceCondPP_.deviceViewAll(),
        heatSourcePFP_.deviceViewAll(),
        temperatureRate_.deviceViewAll());

    heatTransferTimer_.end();

    temperatureIntegrationTimer_.start();

    thermalSphereParticlesKernels::integrateTemperature(
        mask,
        control().time().dt(),
        temperature().deviceViewAll(),
        temperatureRate_.deviceViewAll());

    temperatureIntegrationTimer_.end();
}

bool thermalSphereParticles::insertParticles(
    const realx3Vector&         pos,
    const wordVector&           names,
    const anyList&              vars)
{
    anyList nv(vars);

    realVector cpV      ("Cp");
    realVector kV       ("k");
    realVector epsV     ("emissivity");
    realVector e0V      ("E0");
    realVector nuV      ("nu");
    realVector kappaV   ("fluidKappa");
    realVector alphaV   ("fluidAlpha");

    // temperature, heatSourcePFP, radSumTemp, radNumPrt are not
    // seeded here: all default correctly for newly inserted particles
    // via their own field default value.

    for (const auto& name : names)
    {
        uint32 i;
        if (thSpheres_.shapeNameToIndex(name, i))
        {
            cpV .push_back(thSpheres_.heatCapacity(i));
            kV  .push_back(thSpheres_.heatConductivity(i));
            epsV.push_back(thSpheres_.emissivity(i));
            e0V .push_back(thSpheres_.realYoungsModulus(i));
            nuV .push_back(thSpheres_.poissonRatio(i));

            // Same ambient PFP fallback as initializeThermalParticles();
            // overwritten immediately by the coupled solver if one exists.
            kappaV.push_back(thSpheres_.ambientFluidKappa());
            alphaV.push_back(thSpheres_.ambientFluidAlpha());
        }
    }

    nv.emplaceBack(Cp_.name()            + "Vector", std::move(cpV));
    nv.emplaceBack(conductivity_.name()  + "Vector", std::move(kV));
    nv.emplaceBack(emissivity_.name()    + "Vector", std::move(epsV));
    nv.emplaceBack(E0_.name()            + "Vector", std::move(e0V));
    nv.emplaceBack(nu_.name()            + "Vector", std::move(nuV));
    nv.emplaceBack(fluidKappa_.name()    + "Vector", std::move(kappaV));
    nv.emplaceBack(fluidAlpha_.name()    + "Vector", std::move(alphaV));

    return sphereParticles::insertParticles(pos, names, nv);
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow
