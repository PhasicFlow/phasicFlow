# Oxidation of a Packed Bed of Oxygen Carrier (v-1.0)

## Problem Definition

A packed bed of 1,000 porous oxygen-carrier particles sits in a tube of
20 mm inner diameter, in a furnace at 1173.15 K (900 °C), surrounded by
air. The particles are 2 mm spheres of the carrier Fe45Al-FG of Abad et al.
(2007): Fe2O3 on Al2O3, here fully reduced, so the active part is Fe3O4.
The particles fall into the tube, settle within half a second into a bed
about 2.4 cm high, and react with the oxygen of the air:

4 Fe3O4 + O2 → 6 Fe2O3, with −475 kJ per mol of O2 at 1173 K

This is the oxidation step (air reactor) of chemical looping combustion.
The reaction heats the particles. They pass the heat to each other and to
the tube wall and floor, which stay at 1173.15 K, so the bed centre gets
hotter than the particles at the wall. The run lasts 5 s.

The solver is `multiSpeciesGranFlow`, the standalone reacting DEM solver.
It solves no gas phase: every particle sees the gas composition
`defaultGasConcentrations` for the whole run, as in a bed swept by an
excess of air, and the gas carries no heat away. For a bed with a gas
phase, see the `reactingSpherePFPlus` tutorial of PhasicFlowPlus.

***

## What's New Compared With the Thermal Tutorial

If you haven't gone through the [thermal rotating drum tutorial](../../heatSphereGranFlow/heatRotatingDrumSmall)
yet, do that first. It explains `thermoPhysicalInteraction`, the real
Young's modulus and `fluidProperties`. This tutorial covers what's added on
top of it.

### Species of the Particles

Each particle carries the mass fractions of its solid species, one field
`solidFrac_<species>` per species. They are set like the temperature, and
they must add up to 1:

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
defaultValue
{
    ...
    temperature     real    1173.15;
    solidFrac_Fe2O3 real    0.0;
    solidFrac_Fe3O4 real    0.4416;
    solidFrac_inert real    0.5584;
}
```

The particle density of `caseSetup/interaction` (3208 kg/m³) is the
apparent density of the porous particle at this composition. With
`enableShrinking No` the diameter stays fixed and the particle mass follows
the reaction.

### Reactions

The reactions are read from `multiReactiveProperties` in
`caseSetup/thermoPhysicalInteraction`:

<div align="center"> 
in <b>caseSetup/thermoPhysicalInteraction</b> file
</div>

```C++
enableMultiReaction   Yes;

multiReactiveProperties
{
    solidSpecies        (Fe2O3   Fe3O4   inert);
    solidMolarMasses    (159.69  231.53  101.96);  // [kg/kmol]
    solidDensities      (5240    5170    4234);    // [kg/m3]

    productGas          (O2      N2);
    gasMolarMasses      (31.998  28.013);          // [kg/kmol]

    defaultGasConcentrations (0.002176  0.008212); // [kmol/m3], air at 1173.15 K

    enableShrinking     No;

    reactions
    {
        AR_O2       // 4 Fe3O4 + O2 -> 6 Fe2O3
        {
            rateBasis             grain;
            preExpFactor          3.1e-4;    // [m/s]
            activationTemperature 1684;      // [K]
            ...
            stoicSolid            (6  -4  0);
            stoicGas              (-1  0);

            particleModel         grain;
            ...
            correctionTable       Yes;
        }
    }
}
```

- `solidSpecies` and `productGas` name the species. `stoicSolid` and
  `stoicGas` give the moles of each species per mole of the gas reactant
  (`gasSpeciesIdx`), negative for a reactant.
- `defaultGasConcentrations` is the gas that the particles see. In a
  CFD-DEM run it is replaced by the gas of the fluid cells at the first
  exchange; here it holds for the whole run.
- `preExpFactor` and `activationTemperature` are the Arrhenius kinetics of
  Abad et al. (2007, Table 3). With `rateBasis grain` they are per unit
  surface of the grains, as measured in the thermogravimetric analyzer; the
  code converts them to the outer surface of the particle.
- `particleModel` sets the resistance of the porous particle: `none` (rate
  from the kinetics only), `USCM` (unreacted shrinking core model) or
  `grain` (grain model). The grain model uses the grain radius, the
  effective diffusivity of the gas in the pores (`D_e`, `D_eKnudsen`) and
  the gas-film coefficient `k_c`.
- `correctionTable Yes` makes the grain model read a correction table,
  which brings its closed-form rate to the exact grain model. The table is
  built by `correctionTablePhasicFlow` (step 3 of `runThisCase`) and
  written to `caseSetup/correctionTables`. Build it again after changing
  the reaction or particle data.

The sources of all values are given next to them in the file.

### Heat Exchange With the Tube

The tube and its floor are at 1173.15 K. Their temperature is given per
surface in `settings/geometryDict`, and all three particle-wall paths are
on:

```C++
enablePW              Yes;   // contact conduction (Q_pw)
enablePFW             Yes;   // conduction through the gas lens (Q_pfw)
enableWallRadiation   Yes;   // radiation (Q_rw)
```

The wall material is Kanthal APMT, the FeCrAl alloy of a lab CLC reactor,
with its own thermal properties in `thermoPhysicalInteraction`. In this
standalone run, the conduction paths take the gas conductivity and volume
fraction from `fluidProperties`: air at 1173 K and the gas fraction of a
poured bed.

## Running the Case

Run the script `./runThisCase`, which does four steps:

```
particlesPhasicFlow
geometryPhasicFlow
correctionTablePhasicFlow
multiSpeciesGranFlow
```

`./cleanThisCase` removes the time folders and the files that the run
writes, the correction table included. The run takes about half an hour
on one core of a Serial build. Raise `endTime` in `settings/settingsDict`
for a longer run: the bed reaches a conversion of about 0.66 at 10 s.

## Results

During the run, `settings/postprocessDataDict` writes every 0.1 s to
`postprocessData/`:

- `bed/temperature`: mean temperature of the particles.
- `bed/Fe2O3`: mass-weighted mean of `solidFrac_Fe2O3`, y. The conversion
  of the bed follows from it as X = y / (0.4569 − 0.0153 y), from the
  stoichiometry and the start composition.
- `radialProfile/temperature`: mean temperature in four spheres of 1.5 mm
  radius near mid-height of the bed (z = 11 mm), at r = 0, 2.8, 5.7 and
  8.5 mm from the axis. A sphere with no particle gives 0.

The solver also writes `wallHeatFlow.dat` in the case folder: the heat that
each surface (`tube`, `floor`) gives to the particles through each path, in
W, averaged over `wallHeatReportInterval`. Here it is negative: the bed heats
the wall.

At 5 s the conversion of the bed is about 0.41 and its mean temperature
about 70 K above the wall. Near mid-height, the particles on the axis are
about 90 K above the wall and those next to the tube wall about 37 K.
The tube and the floor then take about 75 W from the bed, three quarters
of it through the gas lens. The particles start from random positions, so
these numbers change by a few percent from run to run.

For ParaView:

```
pFlowToVTK --binary --fields temperature solidFrac_Fe2O3 id
```

## References

- A. Abad, J. Adánez, F. García-Labiano, L.F. de Diego, P. Gayán, J. Celaya,
  Mapping of the range of operational conditions for Cu-, Fe-, and Ni-based
  oxygen carriers in chemical-looping combustion, *Chem. Eng. Sci.* 62
  (2007) 533-549: carrier, Tables 2 and 3.
- F. García-Labiano, L.F. de Diego, J. Adánez, A. Abad, P. Gayán, Temperature
  variations in the oxygen carrier particles during their reduction and
  oxidation in a chemical-looping combustion system, *Chem. Eng. Sci.* 60
  (2005) 851-862: effective diffusivity, particle conductivity and
  emissivity.
- D.J. Gunn, Transfer of heat or mass to particles in fixed and fluidised
  beds, *Int. J. Heat Mass Transfer* 21 (1978) 467-476: film coefficient.
- H.R. Norouzi, R. Zarghami, R. Sotudeh-Gharebagh, N. Mostoufi, *Coupled
  CFD-DEM Modeling: Formulation, Implementation and Application to
  Multiphase Flows*, Wiley, 2016, Section 6.3.2: heat transfer models.

The grain model and its correction table are described in
`particleModelFactor.hpp` and `grainCorrectionTable.hpp`, and the
particle-wall paths in `thermalWallMechanism.hpp`, each with its
references.
