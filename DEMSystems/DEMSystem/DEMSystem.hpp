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

#ifndef __DEMSystem_hpp__
#define __DEMSystem_hpp__

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

/**
 * @class DEMSystem
 * @brief Base class defining the interface for all discrete element systems.
 * * @details
 * Provides the overarching API for mechanical, thermal, and chemical 
 * interactions between particles and coupling layers. Derived classes 
 * override these virtual methods to expose their specific physics arrays.
 */
class DEMSystem
{
protected:

    // --- Section 1: System Control ---

    readControlDict          ControlDict_;
    uniquePtr<systemControl> Control_ = nullptr;
    uniquePtr<Timers>        timers_;

public:

    TypeInfo("DEMSystem");

    // --- Section 2: Constructor / Destructor ---

    DEMSystem
    (
        word                    demSystemName,
        const std::vector<box>& domains,
        int                     argc,
        char* argv[]
    );

    virtual ~DEMSystem();

    DEMSystem(const DEMSystem&)            = delete;
    DEMSystem& operator=(const DEMSystem&) = delete;

    create_vCtor
    (
        DEMSystem,
        word,
        (
            word                    demSystemName,
            const std::vector<box>& domains,
            int                     argc,
            char* argv[],
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

    // --- Section 3: Global Accessors ---

    inline realx3        g()            const { return Control_->g();        }
    inline auto&         Control()            { return Control_();           }
    inline const auto&   Control()      const { return Control_();           }
    inline constexpr auto usingDouble() const { return pFlow::usingDouble__; }
    inline Timers&       timers()             { return Control_->timers();   }

    // --- Section 4: Pure Virtual Interfaces (Mechanics) ---

    virtual bool updateParticleDistribution
    (
        real extentFraction,
        const std::vector<box> domains
    ) = 0;
    
    virtual int32 numParInDomain(int32 di) const = 0;
    virtual std::vector<int32> numParInDomains() const = 0;
    virtual span<const int32>  parIndexInDomain(int32 domIndx) const = 0;
    virtual real maxBounndingSphereSize() const = 0;

    virtual std::vector<real> shapeDiameters() const = 0;
    virtual span<real>   diameter()            = 0;
    virtual span<uint32> particleId()          = 0;
    virtual span<real>   courseGrainFactor()   = 0;

    virtual span<realx3> acceleration()        = 0;
    virtual span<realx3> velocity()            = 0;
    virtual span<realx3> position()            = 0;
    virtual span<realx3> rAcceleration()       = 0;
    virtual span<realx3> rVelocity()           = 0;
    virtual span<realx3> rPosition()           = 0;

    virtual span<realx3> parFluidForce()       = 0;
    virtual span<realx3> parFluidTorque()      = 0;
    
    virtual bool sendFluidForceToDEM()         = 0;
    virtual bool sendFluidTorqueToDEM()        = 0;

    virtual bool beforeIteration() = 0;
    virtual bool iterate(real upToTime, real timeToWrite, word timeName) = 0;
    virtual bool iterate(real upToTime) = 0;

    // --- Section 5: Virtual Interfaces (Thermal & Radiation) ---
    // Default implementation returns empty spans or false for non-thermal systems.

    virtual span<real>   temperature()              { return span<real>();   }
    virtual span<real>   emissivity()               { return span<real>();   }
    virtual span<real>   radSumTemp()               { return span<real>();   }
    virtual span<uint32> radNumPrt()                { return span<uint32>(); }

    virtual span<real>   parFluidHeatSourceConv()   { return span<real>();   }
    virtual span<real>   parFluidHeatSourceRad()    { return span<real>();   }
    virtual bool         sendFluidHeatSourcesToDEM(){ return false;          }
    virtual bool         hasRadiation() const       { return false;          }

    virtual span<real>   parFluidKappa()            { return span<real>();   }
    virtual span<real>   parFluidAlpha()            { return span<real>();   }
    virtual bool         sendFluidPropertiesToDEM() { return false;          }

    // --- Section 6: Virtual Interfaces (Chemical Reactions) ---
    // Default implementation returns empty spans or false for non-reactive systems.

    virtual span<real> solidMassFractions()         { return span<real>(); }
    virtual span<real> gasMassSource()              { return span<real>(); }
    virtual span<real> gasMassSourceSp()            { return span<real>(); }
    virtual span<real> gasConcentrations()          { return span<real>(); }

    /**
     * @brief Per-particle heat deposited in the solid: (1-η) * Q_rxn [W].
     * Drives DEM temperature integration and is available for post-processing.
     */
    virtual span<real> reactionHeat()               { return span<real>(); }

    /**
     * @brief Per-particle heat routed to the CFD energy equation: η * Q_rxn [W].
     * Zero-filled when η = 0 (default behavior for surface reactions).
     */
    virtual span<real> reactionHeatFluid()          { return span<real>(); }

    /**
     * @brief Names of the gas species in the DEM kinetics order.
     */
    virtual std::vector<std::string> gasSpeciesNames() const
    {
        return std::vector<std::string>();
    }

    virtual bool sendGasConcentrationsToDEM()       { return false; }

    // --- Section 7: Static Factory ---

    static uniquePtr<DEMSystem> create
    (
        word                    demSystemName,
        const std::vector<box>& domains,
        int                     argc,
        char* argv[],
        bool                    requireRVel = false
    );
};

} // namespace pFlow

#endif // __DEMSystem_hpp__



