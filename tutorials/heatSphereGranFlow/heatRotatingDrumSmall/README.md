# Simulating Heat Transfer in a Rotating Drum (v-1.0)

## Problem Definition

This tutorial extends the [rotating drum tutorial](../../sphereGranFlow/rotatingDrumSmall) with
particle-scale heat transfer. The geometry, particle count, and mechanical
contact setup are identical to that tutorial (a drum of diameter 0.24 m and
length 0.1 m, rotating at 11.6 rpm, filled with 30,000 spherical particles
of 4 mm diameter). What's new here: half of the particles (by axial position)
start at 300 K, the other half at 400 K, and heat flows between them through
direct contact (`Q_pp`) and through the thin gas layer between near-touching
particles (`Q_pfp`) as the drum rotates and mixes them.

***

## What's Different From the Non-Thermal Tutorial

If you haven't gone through the [non-thermal rotating drum tutorial](../../sphereGranFlow/rotatingDrumSmall)
yet, do that first -- this tutorial only covers what's added on top of it.

### Material Properties for Heat Transfer

`caseSetup/interaction` needs two properties beyond the mechanical ones,
read directly by the thermal solver for the Hertzian contact-radius
correction used in the conduction model:

<div align="center"> 
in <b>caseSetup/interaction</b> file
</div>

```C++
realYoungsModuli   (2.0e9);   // real (unsoftened) Young's modulus [Pa]

poissonRatios      (0.25);    // [-]
```

`Yeff` in the same file (`1.0e6`) is the *softened* Young's modulus used
for the mechanical contact force model -- softened so the simulation can
use a larger timestep. `realYoungsModuli` is the *actual* material value,
used only to correct the contact radius used in the heat conduction
calculation.

### The New `thermoPhysicalInteraction` File

This is a new dictionary (there was no thermal dictionary in the
non-thermal tutorial) that turns on the heat transfer mechanisms and
gives the thermal properties of the material:

<div align="center"> 
in <b>caseSetup/thermoPhysicalInteraction</b> file
</div>

```C++
heatCapacities      (1800.0);   // [J/(kg.K)]
heatConductivities  (0.3);      // [W/(m.K)]
emissivities        (0.9);

enableRadiation      No;
enablePP             Yes;    // collisional (particle-particle) conduction
enablePFP            Yes;    // particle-fluid-particle sub-grid conduction

simYoungsModulus     1.0e6;  // must equal Yeff in caseSetup/interaction

fluidProperties
{
    kappa   0.030;  // conductivity of the gas [W/(m.K)]
    alpha   0.4;    // gas volume fraction around the particles [-]
}
```

Radiation is off in this tutorial since there's no surrounding fluid
temperature field to exchange with in a standalone (non-CFD-coupled) run --
see the model documentation in `thermalRadiationMechanism.hpp` for why.
`Q_pp` (direct-contact conduction) and `Q_pfp` (through-the-air-gap
conduction) are both active. See `thermalConductionPFPKernels.hpp` for the
full model description and references.

`fluidProperties` describes the gas between the particles. In a CFD-DEM run
`Q_pfp` takes the gas conductivity and volume fraction from the fluid cells;
a standalone run has no fluid mesh, so it takes them from this block. The
values are air at 350 K (between the two starting temperatures) and the gas
fraction of a poured bed of equal spheres. The sources are given next to the
values in the file.

### Two Starting Temperatures

`settings/particlesDict` now sets a `temperature` field, and uses two
`selectors` to split the drum's particles into a cold half and a hot half
by their axial (z) position:

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
defaultValue
{
    ...
    temperature     real    300.0;   // fallback; fully overridden below
}

selectors
{
    coldHalf
    {
        selector    box;

        boxInfo
        {
            min (-0.08 -0.08 0.015);
            max ( 0.08  0.08 0.05 );   // drum's z-midpoint
        }

        fieldValue
        {
            temperature   real   300.0;
        }
    }

    hotHalf
    {
        selector    box;

        boxInfo
        {
            min (-0.08 -0.08 0.05 );   // drum's z-midpoint
            max ( 0.08  0.08 0.098);
        }

        fieldValue
        {
            temperature   real   400.0;
        }
    }
}
```

Each selector is applied, in order, after `defaultValue` -- so every
particle ends up at exactly 300 K or 400 K depending on which half of the
drum it starts in.

### Heating Through the Drum Wall (Optional)

The drum wall is adiabatic in this tutorial. Three particle-wall heat paths
can be switched on in `caseSetup/thermoPhysicalInteraction`:

```C++
enablePW              No;   // contact conduction (Q_pw)
enablePFW             No;   // conduction through the gas lens (Q_pfw)
enableWallRadiation   No;   // radiation (Q_rw)
```

The paths need a wall temperature, given per surface in
`settings/geometryDict`:

```C++
cylinder
{
    ...
    temperature  400;   // [K]
}
```

A surface without `temperature` stays adiabatic. Each particle exchanges
heat with each surface through its nearest triangle. A wall takes the
thermal properties of its material, which is `prop1` (the particle
material) in this tutorial. With any path on, the solver writes the heat
that each surface gives to the particles, per path, to `wallHeatFlow.dat`
in the case folder (one line every `wallHeatReportInterval`, 0.01 s by
default). The models and their references are described in
`thermalWallMechanism.hpp` and `thermalWallKernels.hpp`.

## Running the Case

Run the script `./runThisCase`, or the same three steps as the non-thermal
tutorial, except the last one:

```
particlesPhasicFlow
geometryPhasicFlow
heatSphereGranFlow
```

(`heatSphereGranFlow` is the standalone thermal DEM solver -- the
mechanical-only tutorial uses `sphereGranFlow` instead.) `./cleanThisCase`
removes the time folders and the VTK files.

## Post Processing

```
pFlowToVTK --binary --fields temperature velocity id
```

Open the result in ParaView and colour by `temperature` to watch the hot
and cold halves mix as the drum rotates.

## References

The heat transfer models used here (`Q_pp`, `Q_pfp`, radiation) are
described and referenced in detail in the Doxygen documentation of
`thermalConductionPFPKernels.hpp` and `thermalRadiationMechanism.hpp`.
The primary reference for all three models is:

- H.R. Norouzi, R. Zarghami, R. Sotudeh-Gharebagh, N. Mostoufi,
  *Coupled CFD-DEM Modeling: Formulation, Implementation and Application
  to Multiphase Flows*, Wiley, 2016, Section 6.3.2.

The particle-wall paths take the wall as a particle of infinite radius
(Section 6.3.2.2 of the same book), the static gas lens of Rong and Horio
(1999) in the form of Liu, Deen and Tang (2021, Chem. Eng. Sci. 236,
116492), and the particle-wall radiation distribution factors of Johnson,
Tari and Baker (2021, Powder Technol. 380, 164-182).
