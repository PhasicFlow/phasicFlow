# PhasicFlow Features (v-1.0)

The features of PhasicFlow described here are the main features that are implemented in the code for version 1.0. This document is not a complete list of all the features of PhasicFlow. The features are being added to the code continuously and this document may be behind the latest updates. Of course, the code review will give you the complete list.

## Table of Contents

- [1. Building options](#1-building-options)
- [2. Preprocessing tools](#2-preprocessing-tools)
- [3. Solvers for simulations](#3-solvers-for-simulations)
- [4. Postprocessing tools](#4-postprocessing-tools)
- [5. Models and features for simulations](#5-models-and-features-for-simulations)
  - [5.1. General representation of walls](#51-general-representation-of-walls)
  - [5.2. High precision integeration methods](#52-high-precision-integeration-methods)
  - [5.3. Contact force models](#53-contact-force-models-needs-improvement)
  - [5.4. Particle insertion](#54-particle-insertion)
  - [5.5. Restarting/resuming a simulation](#55-restartingresuming-a-simulation)
  - [5.6. Postprocessing data during simulation](#56-postprocessing-data-during-simulation)

## 1. Building options

You can build PhasicFlow to be executed on multi-core CPUs or GPUs. It is also possible to select the type of floating point variables in PhasicFlow: double or float. float type requires less memory and mostly consumes less time of a processor to complete a mathematical operation. So, there is a benefit for using floats in DEM simulation specially when GPU is targeted for computations.

Build options for PhasicFlow:

- **serial (double or float type)**: execution on one cpu core
- **OpenMp (double or float type)**: execution on multiple cores of a CPU
- **cuda (double or float type)**: execution on cuda-enabled GPUs

for more information on building PhasicFlow, please refer to the [installation guide](./howToBuild-V1.0.md).

## 2. Preprocessing tools

PhasicFlow provides a set of tools for preprocessing the simulation case. These tools are used to define the initial state of particles, walls and other parameters that are required for running a simulation.

- [**particlesPhasicFlow**](./../../utilities/particlesPhasicFlow/) tool can be used to define the initial position of particles (for example at t = 0 s) and to set the initial field values for particles (like velocity, orientation, acceleration, etc.).

- [**geometryPhasicFlow**](./../../utilities/geometryPhasicFlow/) converts user inputs for walls into a data structure that is used by PhasicFlow.

## 3. Solvers for simulations

- [**sphereGranFlow**](./../../solvers/sphereGranFlow/) is a solver for simulating the flow of spherical particles with particle insertion mechanism. A full set of tutorial on various possible simulations can be found here: [sphereGranFlow tutorial](./../../tutorials/sphereGranFlow/).
- [**grainGranFlow**](./../../solvers/grainGranFlow/) is a solver for simulating the flow of course-grained particles with particle insertion mechanism. A full set of tutorial on various possible simulations can be found here: [grainGranFlow tutorial](./../../tutorials/grainGranFlow/).
- [**iterateGeometry**](./../../solvers/iterateGeometry/) is a  solver testing motion of walls without simulating particles. Since simulating with particles may take a long time and we may want to check the motion of geometry to be correct before actual simulation, we created this utility to test the motion of walls. A set of tutorial on various possible simulations can be found here: [iterateGeometry tutorial](./../../tutorials/iterateGeometry/).

## 4. Postprocessing tools

- [**pFlowToVTK**](./../../utilities/pFlowToVTK) is used to convert simulation results into vtk file format. vtk file format can be read by Paraview for visualizing the results.
- [**postprocessPhasicFlow**](./../../utilities/postprocessPhasicFlow/) is a tool for performing various averaging and summation on the fields. Particle probing is also possible.

## 5. Models and features for simulations

### 5.1. General representation of walls

Walls can be defined in three ways in PhasicFlow:

- **Builtin walls** in PhasicFlow that include plane wall, cylinder/cone wall, cuboid, circle.
- **stl wall**  that reads the data of the wall from an ASCII stl file.
- **foamPatch wall** that reads the OpenFOAM mesh and converts the boundary patches into PhasicFlow walls (this feature is only available when performing CFD-DEM simulation using OpenFOAM).

Walls can be fixed or in motion during simulations. Various motion models are implemented to cover most of the wall motions in phasicFlow ([see the source code](./../../src/MotionModel/)):

- **stationay**  model, in which all walls are fixed. This model is mostly useful for granular flow under gravity or gas-solid flows (CFD-DEM).
- **rotatingAxis**  model, in which walls are rotating around an axis of rotation with specified rotation speed. This model covers a wide range of granular flows in which the whole or a part of geometry is rotating, like mixers.
- **multiRotatingAxis** model, in which a combination of rotations can be specified. One axis of rotation can itself have another axis of rotation, and so on. This creates the possibility of defining very complex motion pattern for walls, like what we see in Nauta blenders.
- **vibrating** model, in which walls vibrates based on a sinusoidal model with specified frequency and amplitude.
In addition to these models, the user can add other motion models to the code based on their need.

### 5.2. High precision integeration methods

The precision of integration in a DEM simulation is very important. Since sudden changes in the interaction forces occur during simulations (when objects contact or when they rebound). High precision integration methods makes it possible to accurately track position and velocity of objects (specially when they are in contact). When using these methods, it is possible to choose larger time steps for integration without loosing accuracy and causing instability in the simulation. Although  a high-precision integration requires more computations, but the benefits of choosing larger time steps in simulation can totally compensate it.

Various integration methods are implemented in PhasicFlow:

|Integration Method | Order | Type|
| :--- | :---: | :---: |
| AdamsBashforth2 | 2 | one-step |
| AdamsBashforth3 | 3 | one-step |
| AdamsBashforth4 | 4 | one-step |
| AdamsBashforth5 | 5 | one-step |
| AdamsMoulton3 | 3 | predictor-corrector (not active)|
| AdamsMoulton4 | 4 | predictor-corrector (not active)|
| AdamsMoulton5 | 5 | predictor-corrector (not active)|

### 5.3. Contact force models (needs improvement)

Linear and non-linear visco-elastic contact force models are considered in the simulation. In addition to these, limited and non-limited Coulomb's friction model can be used to account for the friction between objects. For spherical objects, rolling friction can also be specified between bodies in contact.
In addition, for course-grained particles simulation, we developed a speciall set of***

### 5.4. Particle insertion

Particles can be inserted during simulation from specified region at specified rate and time interval. Any number of insertion regions can be defined in a simulation. Various region types are considered here: `box`, `cylinder` and `sphere`. Particles are inserted into the simulation through the specified region.

### 5.5. restarting/resuming a simulation

It is possible to resume a simulation from any time-folder that is available in the simulation case setup directory. PhasicFlow restarts the simulation from that time folder.

### 5.6. Postprocessing data during simulation

PhasicFlow provides a powerful in-simulation postprocessing module that allows users to analyze particle data in real-time while the simulation is running. This feature enables:

- **Real-time data analysis** without waiting for simulation completion
- **Region-based processing** in spheres, along lines, or at specific points
- **Various statistical operations** including weighted averages and sums of particle properties
- **Individual particle tracking** to monitor specific particles throughout simulation
- **Multiple processing methods** including arithmetic mean, uniform distribution, and Gaussian distribution
- **Particle filtering** based on properties like diameter, mass, etc.
- **Flexible time control** options for when postprocessing should be executed

To activate in-simulation postprocessing, users need to:

1. Create a `postprocessDataDict` file in the `settings` directory with appropriate configurations
2. Add `libs ("libPostprocessData.so")` and `auxFunctions postprocessData` to the `settings/settingsDict` file

Results are written to output files in the case directory with timestamps, allowing users to monitor simulation behavior as it progresses without interrupting the simulation. for more information on how to use this feature, please refer to the [PostprocessData](./../../src/PostprocessData/) module.

The same postprocessing module can also be used after simulation completion through the [`postprocessPhasicFlow`](./../../utilities/postprocessPhasicFlow/) utility.
