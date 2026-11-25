# phasicFlow Tutorials

This directory contains various tutorial examples to help you learn how to use phasicFlow for different particle simulation scenarios. Each tutorial provides a complete working example with configuration files and instructions.


## 1. Sphere DEM Simulations (`sphereGranFlow`)

The most comprehensive collection of tutorials using spherical particles for DEM simulations:

- [Rotating Drum (Small)](./sphereGranFlow/rotatingDrumSmall/) - Simple rotating drum simulation with 30,000 spherical particles (4 mm diameter)
- [Rotating Drum (Medium)](./sphereGranFlow/rotatingDrumMedium/) - Medium-sized rotating drum simulation
- [Rotating Drum with Baffles](./sphereGranFlow/RotatingDrumWithBaffles/) - Rotating drum with internal baffles and binary particle mixture
- [Rotating Drum with Periodic Boundaries](./sphereGranFlow/drum-PeriodicBoundary/) - Simulates an "infinite length" drum using periodic boundaries
- [V-blender](./sphereGranFlow/V-blender/) - V-shaped blender simulation for particle mixing
- [Tote Blender](./sphereGranFlow/toteBlender/) - Double pedestal tote blender simulation with 24,000 particles
- [Binary System of Particles](./sphereGranFlow/binarySystemOfParticles/) - Demonstrates particle segregation in a rotating drum with two particle sizes
- [Screw Conveyor](./sphereGranFlow/screwConveyor/) - Simulates particle transport using a screw conveyor
- [Conveyor Belt](./sphereGranFlow/conveyorBelt/) - Simulates particles on a moving conveyor belt
- [Rotary Air Lock Valve](./sphereGranFlow/RotaryAirLockValve/) - Simulates a rotary air lock valve with particle insertion
- [Layered Silo Filling](./sphereGranFlow/layeredSiloFilling/) - Demonstrates filling a silo with layers of particles
- [Homogenization Silo (Periodic Boundaries)](./sphereGranFlow/homogenizationSilo-PeriodicBoundary/) - Simulates homogenization in a silo with periodic boundaries

## 2. Course-grained (Grain) DEM Simulations (`grainGranFlow`)

Tutorials for simulations with non-spherical (grain-shaped) particles:

- [Rotating Drum](./grainGranFlow/rotatingDrum/) - Demonstrates non-spherical grain particles in a rotating drum

## 3. Geometry Iteration (`iterateGeometry`)

Tutorials focused on geometry handling and visualization:

- [Inclined Screw Conveyor](./iterateGeometry/inclinedScrewConveyor/) - Visualizes the geometry of an inclined screw conveyor without particles

## 4. Post-processing (`postprocessPhasicFlow`)

Tutorials focused on post-processing and analysis:

- [Segregation Analysis](./postprocessPhasicFlow/segregation/) - Demonstrates how to analyze particle segregation in simulation results
- [Lacey Mixing Index Analysis](./postprocessPhasicFlow/LaceyMixingIndex/) - Setup a case and calculates the Lacey Mixing Index (LMI) in a rotating drum filled with two types of particles during the simulaiton.


## 5. Heat Transfer (`heatSphereGranFlow`)

Tutorials with particle temperature and heat transfer:

- [Heat Transfer in a Rotating Drum (Small)](./heatSphereGranFlow/heatRotatingDrumSmall/) - The hot and cold halves of a 30,000-particle drum mix and exchange heat by contact conduction and conduction through the gas between particles

## 6. Reacting Particles (`multiSpeciesGranFlow`)

Tutorials with gas-solid reactions in porous particles:

- [Oxidation of a Packed Bed](./multiSpeciesGranFlow/packedBedOxidation/) - 1,000 oxygen-carrier particles are oxidized in air, with the grain model of the porous particle and heat exchange with a furnace tube
