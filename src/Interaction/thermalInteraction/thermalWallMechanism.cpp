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

#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <string>

#include "thermalWallMechanism.hpp"
#include "thermalWallKernels.hpp"
#include "johnsonWallRDF.hpp"

namespace pFlow
{

namespace
{

using wallPolicy = Kokkos::RangePolicy<
    DefaultExecutionSpace,
    Kokkos::Schedule<Kokkos::Dynamic>,
    Kokkos::IndexType<uint32>>;

// One team per triangle; its threads share the cells of the search box
// (the pattern of cellsWallLevel0)
using wallTeamPolicy = Kokkos::TeamPolicy<
    DefaultExecutionSpace,
    Kokkos::Schedule<Kokkos::Dynamic>,
    Kokkos::IndexType<uint32>>;

/// Distance of a particle with no wall triangle in range
constexpr real farFromWall = 1.0e30;

/// Stefan-Boltzmann constant [W/(m2 K4)]
constexpr real stefanBoltzmann = 5.670374419e-8;

template<typename T>
deviceViewType1D<T> toDevice(const std::vector<T>& h, const word& name)
{
    deviceViewType1D<T> d(name, h.size());

    auto m = Kokkos::create_mirror_view(d);

    for (size_t i = 0; i < h.size(); ++i) m(i) = h[i];

    Kokkos::deep_copy(d, m);

    return d;
}

} // anonymous

//----------------------------- private methods -------------------------------

void thermalWallMechanism::readSurfaces(
    const dictionary&       thermoDict,
    const systemControl&    control)
{
    // Material lists, in the order of caseSetup/interaction
    dictionary interactionDict(
        "interaction",
        control.caseSetup().path() + "interaction");

    const auto materials = interactionDict.getVal<wordVector>("materials");
    const auto E0s = interactionDict.getVal<realVector>("realYoungsModuli");
    const auto nus = interactionDict.getVal<realVector>("poissonRatios");
    const auto Ks  = thermoDict.getVal<realVector>("heatConductivities");
    const auto eps = thermoDict.getVal<realVector>("emissivities");

    dictionary geomDict(
        "geometryDict",
        control.settings().path() + "geometryDict");

    const dictionary& surfDicts = geomDict.subDict("surfaces");

    numSurfaces_ = walls_.numSurfaces();

    const auto pidHost = walls_.propertyId().hostView();
    const auto& wallMaterials = walls_.wallProperty().materials();

    std::vector<uint32> triSurf(walls_.size(), 0);
    std::vector<real>   T(numSurfaces_, real(-1));
    std::vector<real>   K(numSurfaces_), Eps(numSurfaces_);
    std::vector<real>   E0(numSurfaces_), Nu(numSurfaces_);

    for (uint32 s = 0; s < numSurfaces_; ++s)
    {
        const auto& sub  = walls_.subSurfaces()[s];
        const word  name = sub.name();

        surfaceNames_.push_back(name);

        for (uint32 t = sub.start(); t < sub.end(); ++t) triSurf[t] = s;

        if (sub.end() <= sub.start()) continue;

        if (surfDicts.containsDictionay(name))
        {
            const dictionary& sd = surfDicts.subDict(name);

            if (sd.containsDataEntry("temperature"))
            {
                T[s] = sd.getVal<real>("temperature");
            }
        }

        // The material of a surface is that of its first triangle
        const word matName = wallMaterials[pidHost[sub.start()]];

        const auto it =
            std::find(materials.begin(), materials.end(), matName);

        if (it == materials.end())
        {
            fatalErrorInFunction
                << "Material " << matName << " of wall surface " << name
                << " is not in caseSetup/interaction." << endl;
            fatalExit;
        }

        const size_t m = static_cast<size_t>(it - materials.begin());

        K[s]   = Ks[m];
        Eps[s] = eps[m];
        E0[s]  = E0s[m];
        Nu[s]  = nus[m];

        if (T[s] > real(0))
        {
            ++numActive_;

            REPORT(1) << "Wall surface " << Green_Text(name)
                << ": T_w = " << T[s] << " K, material " << matName
                << END_REPORT;
        }
        else
        {
            REPORT(1) << "Wall surface " << Yellow_Text(name)
                << ": no temperature, adiabatic" << END_REPORT;
        }
    }

    triSurface_ = toDevice(triSurf, "triSurface");
    surfT_      = toDevice(T, "surfT");
    surfK_      = toDevice(K, "surfK");
    surfEps_    = toDevice(Eps, "surfEps");
    surfE0_     = toDevice(E0, "surfE0");
    surfNu_     = toDevice(Nu, "surfNu");
}

void thermalWallMechanism::findNearestWalls(
    const pFlagTypeDevice&          flags,
    const deviceViewType1D<realx3>& pos,
    const deviceViewType1D<real>&   diameter,
    const mapperNBS&                mapper,
    real                            maxRangeRatio)
{
    const uint32 ns   = numSurfaces_;
    const uint32 nCap = static_cast<uint32>(pos.extent(0));

    if (minDist_.extent(0) != static_cast<size_t>(nCap)*ns)
    {
        minDist_ = deviceViewType1D<real>("minDist", nCap*ns);
    }

    Kokkos::deep_copy(minDist_, farFromWall);

    auto r = flags.activeRange();

    // The search box of a triangle reaches the largest particle range
    real maxD = 0;

    Kokkos::parallel_reduce(
        "thermalWallMechanism::maxDiameter",
        wallPolicy(r.start(), r.end()),
        LAMBDA_HD(uint32 i, real& m)
        {
            if (flags(i)) m = Kokkos::max(m, diameter[i]);
        },
        Kokkos::Max<real>(maxD));

    const real range = real(0.5)*maxRangeRatio*maxD;

    auto cellIter    = mapper.getCellIterator();
    auto searchCells = mapper.getSearchCells();
    auto triangles   = walls_.getTriangleAccessor();
    auto triSurf     = triSurface_;
    auto surfT       = surfT_;
    auto minDist     = minDist_;

    Kokkos::parallel_for(
        "thermalWallMechanism::findNearestWalls",
        wallTeamPolicy(walls_.size(), Kokkos::AUTO),
        LAMBDA_HD(const typename wallTeamPolicy::member_type& team)
        {
            const uint32 t = team.league_rank();
            const uint32 s = triSurf[t];

            if (surfT[s] <= real(0)) return;

            const realx3x3 tri = triangles(t);

            const realx3 a = tri.x();
            const realx3 b = tri.y();
            const realx3 c = tri.z();

            const int32x3 lo = searchCells.bound(
                searchCells.pointIndex(min(min(a, b), c) - range));

            const int32x3 hi = searchCells.bound(
                searchCells.pointIndex(max(max(a, b), c) + range));

            const int32  ny = hi.y() - lo.y() + 1;
            const int32  nz = hi.z() - lo.z() + 1;
            const uint32 nCells =
                static_cast<uint32>((hi.x() - lo.x() + 1)*ny*nz);

            Kokkos::parallel_for(
                Kokkos::TeamThreadRange(team, nCells),
                [&](const uint32 k)
                {
                    const int32 ik = static_cast<int32>(k);
                    const int32 ix = lo.x() + ik/(ny*nz);
                    const int32 iy = lo.y() + (ik/nz)%ny;
                    const int32 iz = lo.z() + ik%nz;

                    uint32 n = cellIter.start(ix, iy, iz);

                    while (n != mapperNBS::CellIterator::NoPos)
                    {
                        const real d = thermalWallKernels::
                            distanceToTriangle(pos[n], a, b, c);

                        if (d <= real(0.5)*maxRangeRatio*diameter[n])
                        {
                            Kokkos::atomic_min(&minDist[n*ns + s], d);
                        }

                        n = cellIter.next(n);
                    }
                });
        });

    Kokkos::fence();
}

void thermalWallMechanism::writeReport(real time)
{
    if (stepsInSum_ > 0 && report_.is_open())
    {
        report_ << std::setprecision(8) << time;

        for (uint32 s = 0; s < numSurfaces_; ++s)
        {
            for (uint32 k = 0; k < 3; ++k)
            {
                report_ << ' ' << heatSum_[3*s + k] / stepsInSum_;
            }
        }

        report_ << '\n';
        report_.flush();
    }

    std::fill(heatSum_.begin(), heatSum_.end(), real(0));
    stepsInSum_     = 0;
    lastReportTime_ = time;
}

//----------------------------- constructors ----------------------------------

thermalWallMechanism::thermalWallMechanism(
    const dictionary&       thermoDict,
    const systemControl&    control,
    const geometry&         walls,
    bool                    envRadiation)
:
    walls_(walls),
    envRadiation_(envRadiation)
{
    enablePW_ = static_cast<bool>(
        thermoDict.getValOrSet<Logical>("enablePW", Logical("No")));

    enablePFW_ = static_cast<bool>(
        thermoDict.getValOrSet<Logical>("enablePFW", Logical("No")));

    enableRW_ = static_cast<bool>(
        thermoDict.getValOrSet<Logical>(
            "enableWallRadiation",
            Logical("No")));

    if (!(enablePW_ || enablePFW_ || enableRW_)) return;

    simYoungsModulus_ = thermoDict.getVal<real>("simYoungsModulus");

    lensRatio_ = thermoDict.getValOrSet<real>("wallLensRatio", real(1.4));

    minGap_ = thermoDict.getValOrSet<real>("wallMinGap", real(2.75e-8));

    radRange_ = thermoDict.getValOrSet<real>(
        "wallRadiationRange",
        real(8.0));

    reportInterval_ = thermoDict.getValOrSet<real>(
        "wallHeatReportInterval",
        real(0.01));

    const real lastRow = johnsonWallRDF::dist[johnsonWallRDF::nDist - 1];

    if (lensRatio_ < real(1) || minGap_ <= real(0) ||
        radRange_ < real(1) || radRange_ > lastRow)
    {
        fatalErrorInFunction
            << "Need wallLensRatio >= 1, wallMinGap > 0 and "
            << "1 <= wallRadiationRange <= " << lastRow << "." << endl;
        fatalExit;
    }

    readSurfaces(thermoDict, control);

    // Grids then values, as thermalWallKernels::wallRDF reads them
    std::vector<real> tab;

    using namespace johnsonWallRDF;

    tab.insert(tab.end(), epsP, epsP + nEp);
    tab.insert(tab.end(), epsW, epsW + nEw);
    tab.insert(tab.end(), solidFrac, solidFrac + nSf);
    tab.insert(tab.end(), dist, dist + nDist);
    tab.insert(tab.end(), rdf, rdf + nEp*nEw*nDist*nSf);

    rdfTable_ = toDevice(tab, "johnsonWallRDF");

    surfHeat_ = deviceViewType1D<real>("surfHeat", 3*numSurfaces_);
    heatSum_.assign(3*numSurfaces_, real(0));

    if (!active()) return;

    // caseSetup may come with a trailing separator
    std::filesystem::path setupDir(control.caseSetup().path().wordPath());

    if (!setupDir.has_filename()) setupDir = setupDir.parent_path();

    const std::filesystem::path caseDir = setupDir.parent_path();

    const std::filesystem::path reportFile = caseDir / "wallHeatFlow.dat";

    const real startTime = control.time().currentTime();

    if (startTime > 0 && std::filesystem::exists(reportFile))
    {
        // Restart: keep the header and the lines up to the start time
        std::vector<std::string> kept;
        std::ifstream            old(reportFile);
        std::string              line;

        while (std::getline(old, line))
        {
            if (line.empty() || line[0] == '#'
                || std::stod(line) <= startTime*(1 + 1e-9))
            {
                kept.push_back(line);
            }
        }

        old.close();
        report_.open(reportFile);

        for (const auto& l : kept) report_ << l << '\n';
    }
    else
    {
        report_.open(reportFile);

        report_ << "# heat into the particles from each wall surface [W],"
            << " averaged over " << reportInterval_ << " s\n"
            << "# _rw: radiation that crosses the wall, D (T_w^4 - T_p^4)"
            << "\n# time";

        for (uint32 s = 0; s < numSurfaces_; ++s)
        {
            report_ << ' ' << surfaceNames_[s] << "_pw "
                << surfaceNames_[s] << "_pfw " << surfaceNames_[s] << "_rw";
        }

        report_ << '\n';
    }

    REPORT(1) << "Particle-wall heat transfer: Q_pw "
        << (enablePW_ ? "on" : "off") << ", Q_pfw "
        << (enablePFW_ ? "on" : "off") << ", Q_rw "
        << (enableRW_ ? "on" : "off") << END_REPORT;
}

thermalWallMechanism::~thermalWallMechanism()
{
    // The last window, shorter than reportInterval_
    if (stepsInSum_ > 0) writeReport(lastTime_);
}

//---------------------------- public methods ---------------------------------

void thermalWallMechanism::iterate(
    real                            time,
    const pFlagTypeDevice&          flags,
    const deviceViewType1D<realx3>& pos,
    const deviceViewType1D<real>&   diameter,
    const deviceViewType1D<real>&   temperature,
    const deviceViewType1D<real>&   K,
    const deviceViewType1D<real>&   E0,
    const deviceViewType1D<real>&   nu,
    const deviceViewType1D<real>&   emissivity,
    const deviceViewType1D<real>&   fluidKappa,
    const deviceViewType1D<real>&   fluidAlpha,
    const deviceViewType1D<real>&   radSumTemp,
    const deviceViewType1D<uint32>& radNumPrt,
    const mapperNBS&                mapper,
    deviceViewType1D<real>          Q_pw,
    deviceViewType1D<real>          Q_pfw,
    deviceViewType1D<real>          Q_rw)
{
    if (!active()) return;

    real maxRatio = real(1);

    if (enablePFW_) maxRatio = Kokkos::max(maxRatio, lensRatio_);
    if (enableRW_)  maxRatio = Kokkos::max(maxRatio, radRange_);

    findNearestWalls(flags, pos, diameter, mapper, maxRatio);

    Kokkos::deep_copy(surfHeat_, real(0));

    const bool calcPW   = enablePW_;
    const bool calcPFW  = enablePFW_;
    const bool calcRW   = enableRW_;
    const bool envRad   = envRadiation_;
    const real simE     = simYoungsModulus_;
    const real lens     = lensRatio_;
    const real sMin     = minGap_;
    const real radRange = radRange_;
    const uint32 ns     = numSurfaces_;

    auto minDist  = minDist_;
    auto surfT    = surfT_;
    auto surfK    = surfK_;
    auto surfEps  = surfEps_;
    auto surfE0   = surfE0_;
    auto surfNu   = surfNu_;
    auto surfHeat = surfHeat_;
    auto rdfTab   = rdfTable_;

    // Constants as locals, so that the device lambda captures them
    const real pi      = Pi;
    const real sigmaSB = stefanBoltzmann;
    const real far     = farFromWall;

    auto r = flags.activeRange();

    Kokkos::parallel_for(
        "thermalWallMechanism::exchange",
        wallPolicy(r.start(), r.end()),
        LAMBDA_HD(uint32 i)
        {
            if (!flags(i)) return;

            const real R  = real(0.5)*diameter[i];
            const real Ti = temperature[i];

            real qPW = 0, qPFW = 0, qRW = 0;

            for (uint32 s = 0; s < ns; ++s)
            {
                const real d  = minDist[i*ns + s];
                const real Tw = surfT[s];

                if (Tw <= real(0) || d >= far) continue;

                const real gap = d - R;

                // Corrected contact radius, when the particle overlaps
                const real rc = gap < 0
                    ? thermalWallKernels::wallContactRadius(
                        R, -gap, nu[i], surfNu[s], E0[i], surfE0[s], simE)
                    : real(0);

                real sPW = 0, sPFW = 0, sRW = 0, sRWwall = 0;

                if (calcPW && gap < 0)
                {
                    sPW = real(4)*rc*(Tw - Ti)
                        / (real(1)/K[i] + real(1)/surfK[s]);
                }

                if (calcPFW && gap <= (lens - real(1))*R)
                {
                    // Thermal overlap of the corrected contact radius
                    const real delta = gap < 0 ? -rc*rc/R : gap;

                    const real G = thermalWallKernels::lensConductance(
                        R, delta, lens, sMin);

                    sPFW = real(2)*pi*fluidKappa[i]*G*(Tw - Ti);
                }

                if (calcRW && d <= radRange*R)
                {
                    const real D = thermalWallKernels::wallRDF(
                        rdfTab,
                        johnsonWallRDF::nEp,
                        johnsonWallRDF::nEw,
                        johnsonWallRDF::nSf,
                        johnsonWallRDF::nDist,
                        emissivity[i],
                        surfEps[s],
                        real(1) - fluidAlpha[i],
                        d / R);

                    real Tref = Ti;

                    if (envRad && radNumPrt[i] > 0)
                    {
                        Tref = radSumTemp[i] / real(radNumPrt[i]);
                    }

                    const real g   = emissivity[i]*pi*diameter[i]*diameter[i]
                        * sigmaSB*D;
                    const real Tw2 = Tw*Tw, Tr2 = Tref*Tref, Ti2 = Ti*Ti;

                    // The particle: the share D of its view goes from the
                    // environment T_ref to the wall. The wall: what crosses
                    // its surface, D (T_w^4 - T_i^4), for the report.
                    sRW     = g*(Tw2*Tw2 - Tr2*Tr2);
                    sRWwall = g*(Tw2*Tw2 - Ti2*Ti2);
                }

                qPW  += sPW;
                qPFW += sPFW;
                qRW  += sRW;

                Kokkos::atomic_add(&surfHeat[3*s],     sPW);
                Kokkos::atomic_add(&surfHeat[3*s + 1], sPFW);
                Kokkos::atomic_add(&surfHeat[3*s + 2], sRWwall);
            }

            Q_pw[i]  += qPW;
            Q_pfw[i] += qPFW;
            Q_rw[i]  += qRW;
        });

    Kokkos::fence();

    auto h = Kokkos::create_mirror_view_and_copy(
        Kokkos::HostSpace(),
        surfHeat_);

    for (uint32 k = 0; k < 3*ns; ++k) heatSum_[k] += h(k);

    ++stepsInSum_;
    lastTime_ = time;

    if (lastReportTime_ < real(0))
    {
        lastReportTime_ = time;
    }
    else if (time - lastReportTime_ >= reportInterval_*(1 - 1e-6))
    {
        writeReport(time);
    }
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow
