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

//----------------------------- protected methods -----------------------------

void thermalSphereParticles::checkHostMemory()
{
    if (temperature_.size() != temperatureHost_.size())
    {
        size_t oldSize = temperatureHost_.size();
        size_t newSize = temperature_.size();

        resizeNoInit(temperatureHost_,      newSize);
        resizeNoInit(heatSourceConvHost_,   newSize);
        resizeNoInit(heatSourceRadHost_,    newSize);
        resizeNoInit(heatSourceCondPPHost_, newSize);
        resizeNoInit(emissivityHost_,       newSize);
        resizeNoInit(radSumTempHost_,       newSize);
        resizeNoInit(radNumPrtHost_,        newSize);
        resizeNoInit(fluidKappaHost_,       newSize);
        resizeNoInit(fluidAlphaHost_,       newSize);

        for (size_t i = oldSize; i < newSize; ++i)
        {
            temperatureHost_[i]      = temperature_.field()[i];
            heatSourceConvHost_[i]   = heatSourceConv_.field()[i];
            heatSourceRadHost_[i]    = heatSourceRad_.field()[i];
            heatSourceCondPPHost_[i] = heatSourceCondPP_.field()[i];
            emissivityHost_[i]       = emissivity_.field()[i];
            radSumTempHost_[i]       = radSumTemp_.field()[i];
            radNumPrtHost_[i]        = radNumPrt_.field()[i];
            fluidKappaHost_[i]       = fluidKappa_.field()[i];
            fluidAlphaHost_[i]       = fluidAlpha_.field()[i];
        }
    }
}

//----------------------------- constructors ----------------------------------

thermalSphereParticles::thermalSphereParticles(
    systemControl&              control,
    const sphereShape&          shpShape,
    const thermalSphereShape&   thShpShape)
:
    sphereParticles(control, shpShape),
    thSpheres_(thShpShape),
    temperature_(
        objectFile(
            "temperature", 
            "",
            objectFile::READ_ALWAYS,
            objectFile::WRITE_ALWAYS),
        dynPointStruct(), 
        0.0),
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
    heatSourceConv_(
        objectFile(
            "heatSourceConv", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.0),
    heatSourceRad_(
        objectFile(
            "heatSourceRad", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.0),
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
    emissivity_(
        objectFile(
            "emissivity", 
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
        0u),
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
    checkHostMemory();

    temperatureHostUpdatedSync();
    radiationDataHostUpdatedSync();
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
    sphereParticles::beforeIteration();
    checkHostMemory();

    if (heatSourceConvHost_.size() == heatSourceConv_.deviceView().size())
    {
        Kokkos::deep_copy(heatSourceConv_.deviceView(), heatSourceConvHost_);
    }

    if (heatSourceRadHost_.size() == heatSourceRad_.deviceView().size())
    {
        Kokkos::deep_copy(heatSourceRad_.deviceView(), heatSourceRadHost_);
    }

    temperatureRate_.field().fill(0.0);

    temperatureHostUpdatedSync();
    radiationDataHostUpdatedSync();

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

    heatTransferTimer_.start();

    thermalSphereParticlesKernels::calcFluidParticleHeatTransfer(
        mask,
        diameter().deviceViewAll(),
        mass().deviceViewAll(),
        Cp().deviceViewAll(),
        temperature().deviceViewAll(),
        heatSourceConv_.deviceViewAll(),
        heatSourceRad_.deviceViewAll(),
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
    realVector tV       ("T");
    realVector kappaV   ("fluidKappa");
    realVector alphaV   ("fluidAlpha");
    realVector pfpV     ("heatSourcePFP");

    // Placeholder ambient temperature for newly inserted particles.
    // insertionTemperature was removed from thermalSphereShape (it mixed
    // an insertion-event concern into a per-material properties class).
    // The proper fix is a per-insertion-event temperature read from the
    // particle-insertion mechanism itself (sphereInsertion); until that
    // exists, all dynamically inserted particles start at this fixed
    // ambient value.
    constexpr real ambientInsertionTemperature = real(298);

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
            pfpV  .push_back(0.0);

            tV.push_back(ambientInsertionTemperature);
        }
    }

    nv.emplaceBack(Cp_.name()            + "Vector", std::move(cpV));
    nv.emplaceBack(conductivity_.name()  + "Vector", std::move(kV));
    nv.emplaceBack(emissivity_.name()    + "Vector", std::move(epsV));
    nv.emplaceBack(E0_.name()            + "Vector", std::move(e0V));
    nv.emplaceBack(nu_.name()            + "Vector", std::move(nuV));
    nv.emplaceBack(temperature_.name()   + "Vector", std::move(tV));
    nv.emplaceBack(fluidKappa_.name()    + "Vector", std::move(kappaV));
    nv.emplaceBack(fluidAlpha_.name()    + "Vector", std::move(alphaV));
    nv.emplaceBack(heatSourcePFP_.name() + "Vector", std::move(pfpV));

    return sphereParticles::insertParticles(pos, names, nv);
}

void thermalSphereParticles::heatSourcesHostUpdatedSync()
{
    checkHostMemory();

    bool sizeConv = 
        (heatSourceConvHost_.size() == heatSourceConv_.deviceView().size());
    bool sizeRad = 
        (heatSourceRadHost_.size() == heatSourceRad_.deviceView().size());
    bool sizeCond = 
        (heatSourceCondPPHost_.size() == heatSourceCondPP_.deviceView().size());

    if (sizeConv && sizeRad && sizeCond)
    {
        Kokkos::deep_copy(
            heatSourceConv_.deviceView(),   
            heatSourceConvHost_);
            
        Kokkos::deep_copy(
            heatSourceRad_.deviceView(),    
            heatSourceRadHost_);
            
        Kokkos::deep_copy(
            heatSourceCondPP_.deviceView(), 
            heatSourceCondPPHost_);
    }
}

void thermalSphereParticles::fluidPropertiesHostUpdatedSync()
{
    checkHostMemory();

    bool sizeKappa = 
        (fluidKappaHost_.size() == fluidKappa_.deviceView().size());
    bool sizeAlpha = 
        (fluidAlphaHost_.size() == fluidAlpha_.deviceView().size());

    if (sizeKappa && sizeAlpha)
    {
        Kokkos::deep_copy(fluidKappa_.deviceView(), fluidKappaHost_);
        Kokkos::deep_copy(fluidAlpha_.deviceView(), fluidAlphaHost_);
    }
}

void thermalSphereParticles::temperatureHostUpdatedSync()
{
    checkHostMemory();

    if (temperatureHost_.size() == temperature_.deviceView().size())
    {
        Kokkos::deep_copy(temperatureHost_, temperature_.deviceView());
    }
}

void thermalSphereParticles::radiationDataHostUpdatedSync()
{
    checkHostMemory();

    bool sizeEps = 
        (emissivityHost_.size() == emissivity_.deviceView().size());
    bool sizeSum = 
        (radSumTempHost_.size() == radSumTemp_.deviceView().size());
    bool sizeNum = 
        (radNumPrtHost_.size()  == radNumPrt_.deviceView().size());

    if (sizeEps && sizeSum && sizeNum)
    {
        Kokkos::deep_copy(emissivityHost_, emissivity_.deviceView());
        Kokkos::deep_copy(radSumTempHost_, radSumTemp_.deviceView());
        Kokkos::deep_copy(radNumPrtHost_,  radNumPrt_.deviceView());
    }
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow
