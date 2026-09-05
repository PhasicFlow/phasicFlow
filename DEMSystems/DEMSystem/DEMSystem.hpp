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

#ifndef pFlow_DEMSystem_hpp
#define pFlow_DEMSystem_hpp

#include <vector>

#include "types.hpp"
#include "span.hpp"
#include "box.hpp"
#include "virtualConstructor.hpp"
#include "uniquePtr.hpp"
#include "systemControl.hpp"
#include "readControlDict.hpp"

namespace pFlow
{

class DEMSystem
{
protected:

    //- protected members

        /// Parsed contents of the case's controlDict (start/end time,
        /// save interval, etc.), read once at construction.
        readControlDict             ControlDict_;

        /// Owns the case's systemControl repository (time, settings,
        /// case-setup path) for the lifetime of this DEM system.
        uniquePtr<systemControl>    Control_ = nullptr;

        /// Performance-timer registry for this DEM system, rooted under
        /// Control_'s own timer tree.
        uniquePtr<Timers>           timers_;

public:

    //- Type info
        TypeInfo("DEMSystem");

    //- constructors

        DEMSystem(
            word                    demSystemName,
            const std::vector<box>& domains,
            int                     argc,
            char*                   argv[]);

        virtual ~DEMSystem();

        DEMSystem(const DEMSystem&) = delete;

        DEMSystem& operator=(const DEMSystem&) = delete;

    //- public methods

        create_vCtor(
            DEMSystem,
            word,
            (
                word                    demSystemName,
                const std::vector<box>& domains,
                int                     argc,
                char*                   argv[],
                bool                    requireRVel
            ),
            (
                demSystemName,
                domains,
                argc,
                argv,
                requireRVel
            )
        );

        //--- global accessors ------------------------------------------------

        inline
        realx3 g() const
        {
            return Control_->g();
        }

        inline
        auto& Control()
        {
            return Control_();
        }

        inline
        const auto& Control() const
        {
            return Control_();
        }

        inline constexpr
        auto usingDouble() const
        {
            return pFlow::usingDouble__;
        }

        inline
        Timers& timers()
        {
            return Control_->timers();
        }

        //--- pure virtual interfaces: mechanics -------------------------------

        virtual
        bool updateParticleDistribution(
            real                    extentFraction,
            const std::vector<box>  domains) = 0;

        virtual span<const int32> parIndexInDomain(int32 domIndx) const = 0;
        virtual std::vector<int32> numParInDomains() const = 0;
        virtual int32 numParInDomain(int32 di) const = 0;
        virtual real maxBounndingSphereSize() const = 0;

        virtual std::vector<real> shapeDiameters() const = 0;
        virtual span<real> diameter() = 0;
        virtual span<uint32> particleId() = 0;
        virtual span<real> courseGrainFactor() = 0;

        virtual span<realx3> acceleration() = 0;
        virtual span<realx3> velocity() = 0;
        virtual span<realx3> position() = 0;
        virtual span<realx3> rAcceleration() = 0;
        virtual span<realx3> rVelocity() = 0;
        virtual span<realx3> rPosition() = 0;

        virtual span<realx3> parFluidForce() = 0;
        virtual span<realx3> parFluidTorque() = 0;
        virtual bool sendFluidForceToDEM() = 0;
        virtual bool sendFluidTorqueToDEM() = 0;

        virtual bool beforeIteration() = 0;

        virtual
        bool iterate(
            real                    upToTime,
            real                    timeToWrite,
            word                    timeName) = 0;

        virtual bool iterate(real upToTime) = 0;

        //--- thermal & radiation coupling interfaces --------------------------
        // Default no-op / empty-span implementations: a plain
        // (non-thermal) DEM system never overrides these, so the CFD
        // coupling layer can call them unconditionally on any
        // DEMSystem and safely get "nothing to report" back.

        virtual span<real> temperature() { return span<real>(); }
        virtual span<real> emissivity() { return span<real>(); }
        virtual span<real> radSumTemp() { return span<real>(); }
        virtual span<uint32> radNumPrt() { return span<uint32>(); }

        virtual span<real> parFluidHeatSourceConv() { return span<real>(); }
        virtual span<real> parFluidHeatSourceRad() { return span<real>(); }
        virtual bool sendFluidHeatSourcesToDEM() { return false; }
        virtual bool hasRadiation() const { return false; }

        virtual span<real> parFluidKappa() { return span<real>(); }
        virtual span<real> parFluidAlpha() { return span<real>(); }
        virtual bool sendFluidPropertiesToDEM() { return false; }

        //--- chemical reaction coupling interfaces ----------------------------
        // Same default-no-op convention as the thermal interfaces above,
        // for DEM systems that carry no reaction kinetics.

        virtual span<real> solidMassFractions() { return span<real>(); }
        virtual span<real> gasMassSource() { return span<real>(); }
        virtual span<real> gasMassSourceSp() { return span<real>(); }
        virtual span<real> gasConcentrations() { return span<real>(); }
        virtual span<real> reactionHeat() { return span<real>(); }
        virtual span<real> reactionHeatFluid() { return span<real>(); }

        virtual
        std::vector<std::string> gasSpeciesNames() const
        {
            return std::vector<std::string>();
        }

        /**
         * @brief Molar masses [kg/mol] of the gas species known to the DEM
         *        kinetics, in the same order as gasSpeciesNames().
         *
         *        DEM and CFD each keep an independent copy of gas molar
         *        masses, since the DEM kinetics model must remain usable by
         *        the standalone (fluid-mesh-free) DEM solver. This accessor
         *        lets the CFD coupling layer cross-check its own
         *        transportProperties/gasMw values against the DEM's at
         *        startup, so the two copies cannot silently drift apart.
         */
        virtual
        std::vector<real> gasMolarMasses() const
        {
            return std::vector<real>();
        }

        virtual bool sendGasConcentrationsToDEM() { return false; }

        static
        uniquePtr<DEMSystem> create(
            word                    demSystemName,
            const std::vector<box>& domains,
            int                     argc,
            char*                   argv[],
            bool                    requireRVel = false);

}; // DEMSystem

} // pFlow

#endif // pFlow_DEMSystem_hpp
