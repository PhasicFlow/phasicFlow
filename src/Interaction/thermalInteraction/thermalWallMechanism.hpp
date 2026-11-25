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

#ifndef pFlow_thermalWallMechanism_hpp
#define pFlow_thermalWallMechanism_hpp

#include <fstream>
#include <vector>

#include "types.hpp"
#include "dictionary.hpp"
#include "systemControl.hpp"
#include "geometry.hpp"
#include "mapperNBS.hpp"

namespace pFlow
{

/**
 * @brief Heat exchange between the particles and the wall surfaces that
 * have a fixed temperature: contact conduction (Q_pw), conduction
 * through the gas lens (Q_pfw) and radiation (Q_rw).
 *
 * @details
 * The wall temperature is the key "temperature" of a surface in
 * settings/geometryDict. A surface without it is adiabatic and takes no
 * part. Each particle exchanges heat with each surface once, through
 * the triangle of that surface nearest to its centre; so a particle on
 * the common edge or corner of two triangles is not counted twice
 * (Norouzi et al. 2016, Section 3.6.2, names this double counting for
 * the contact force).
 *
 * Paths, each switched on in thermoPhysicalInteraction:
 *   - enablePW: Q_pw = 4 r_c (T_w - T_p) / (1/k_p + 1/k_w), the
 *     particle-particle contact conduction with the wall as a particle
 *     of infinite radius (Norouzi et al., Eq. 6.159 and Section
 *     6.3.2.2), r_c corrected with Eqs. 6.166-6.167.
 *   - enablePFW: Q_pfw = 2 pi k_f G (T_w - T_p), the static gas lens of
 *     Rong and Horio (1999) as written by Liu et al. (2021, Eqs. 31-36),
 *     with k_f the gas conductivity of the particle's CFD cell.
 *   - enableWallRadiation: Q_rw = eps_p A_p sigma D_iw (T_w^4 - T_ref^4)
 *     with D_iw of Johnson et al. (2021). T_ref is T_p when nothing else
 *     carries the particle's radiation. In a CFD-coupled run with
 *     enableRadiation, the CFD side already exchanges the particle's
 *     whole emission with its local environment temperature (Eq. 6.179).
 *     Johnson et al. (Section 3.1) note that the distribution factors of
 *     a particle add up to one, so a share D_iw of that exchange
 *     belongs to the wall instead; the wall term then becomes
 *     eps_p A_p sigma D_iw (T_w^4 - T_env^4), with T_env the mean
 *     temperature of the neighbours (radSumTemp/radNumPrt). The gas
 *     part of the CFD environment temperature is left out of T_env
 *     here; the gas and the particles of a cell are close in
 *     temperature in a fluidized bed.
 *
 * Optional keys (defaults and sources):
 *   - wallLensRatio 1.4: r_lens/R_p (Zhang and Lu 2021; Liu et al. 2021
 *     find the result converged at 1.4).
 *   - wallMinGap 2.75e-8 m: minimum conduction distance s (Zhang and Lu
 *     2021; Liu et al. 2021 find 0.01-1 micrometre makes little
 *     difference).
 *   - wallRadiationRange 8: largest centre-to-wall distance of Q_rw in
 *     particle radii (at most 10.2, the last row kept from the tables).
 *     Integrating the tables, 8 radii hold 91% of the wall exchange at
 *     solid fraction 0.25 and 98-100% at 0.35-0.64.
 *   - wallHeatReportInterval 0.01 s: interval of wallHeatFlow.dat.
 *
 * The heat into the particles from each surface, averaged over the
 * steps since the last line, is written to wallHeatFlow.dat in the case
 * folder.
 */
class thermalWallMechanism
{
private:

    //- private members

        const geometry&             walls_;

        bool                        enablePW_  = false;

        bool                        enablePFW_ = false;

        bool                        enableRW_  = false;

        /// The CFD side exchanges radiation with the environment
        bool                        envRadiation_ = false;

        real                        simYoungsModulus_ = 1e7;

        real                        lensRatio_ = 1.4;

        real                        minGap_ = 2.75e-8;

        real                        radRange_ = 8.0;

        real                        reportInterval_ = 0.01;

        uint32                      numSurfaces_ = 0;

        uint32                      numActive_ = 0;

        std::vector<word>           surfaceNames_;

        /// Subsurface of each triangle
        deviceViewType1D<uint32>    triSurface_;

        /// Wall temperature of each subsurface; negative: adiabatic
        deviceViewType1D<real>      surfT_;

        deviceViewType1D<real>      surfK_;

        deviceViewType1D<real>      surfEps_;

        deviceViewType1D<real>      surfE0_;

        deviceViewType1D<real>      surfNu_;

        /// Grids and values of johnsonWallRDF
        deviceViewType1D<real>      rdfTable_;

        /// Nearest distance of each particle to each subsurface
        deviceViewType1D<real>      minDist_;

        /// Heat [W] into the particles from each subsurface, per path
        deviceViewType1D<real>      surfHeat_;

        /// Sums of surfHeat_ since the last report line
        std::vector<real>           heatSum_;

        uint32                      stepsInSum_ = 0;

        real                        lastReportTime_ = -1;

        /// Time of the last call of iterate
        real                        lastTime_ = 0;

        std::ofstream               report_;

    //- private methods

        /// Wall data of each subsurface from the case dictionaries
        void readSurfaces(
            const dictionary&       thermoDict,
            const systemControl&    control);

        /// Nearest distance of each particle to each active subsurface
        void findNearestWalls(
            const pFlagTypeDevice&          flags,
            const deviceViewType1D<realx3>& pos,
            const deviceViewType1D<real>&   diameter,
            const mapperNBS&                mapper,
            real                            maxRangeRatio);

        void writeReport(real time);

public:

    //- constructors

        /**
         * @param envRadiation True when the CFD side exchanges each
         * particle's radiation with its environment temperature
         * (CFD-coupled run with enableRadiation).
         */
        thermalWallMechanism(
            const dictionary&       thermoDict,
            const systemControl&    control,
            const geometry&         walls,
            bool                    envRadiation);

        thermalWallMechanism(const thermalWallMechanism&) = delete;

        thermalWallMechanism& operator=(
            const thermalWallMechanism&) = delete;

        /// Writes the last, partial report window
        ~thermalWallMechanism();

    //- public methods

        /// True when a path is on and a surface has a temperature
        inline
        bool active() const
        {
            return numActive_ > 0 && (enablePW_ || enablePFW_ || enableRW_);
        }

        /// Adds Q_pw, Q_pfw and Q_rw [W] of this step to the particles
        void iterate(
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
            deviceViewType1D<real>          Q_rw);

}; // thermalWallMechanism

} // pFlow

#endif // pFlow_thermalWallMechanism_hpp
