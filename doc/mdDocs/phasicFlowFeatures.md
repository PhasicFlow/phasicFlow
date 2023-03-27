# PhasicFlow Features {#phasicFlowFeatures}

## Building options
You can build PhasicFlow to be executed on multi-core CPUs or GPUs. It is also possible to select the type of floating point variables in PhasicFlow: double or float. float type requires less memory and mostly consumes less time of a processor to complete a mathematical operation. So, there is a benefit for using floats in DEM simulation specially when GPU is targeted for computations. 
Build options for PhasicFlow:
* **serial (double or float type)**: execution on one cpu core
* **OpenMp (double or float type)**: execution on multiple cores of a CPU
* **cuda (double or float type)**: execution on cuda-enabled GPUs



## Preprocessing tools
Preprocessing tools are used to facilitate the process of case setup. They include tools for defining initial state of particles and geometry conversion. 
* **particlesPhasicFlow** tool can be used to define the initial position of particles (for example at t = 0 s) and to set the initial field values for particles (like velocity, orientation, acceleration and etc).
* **geometryPhasicFlow** converts user inputs for walls into a data structures that is used by PhasicFlow.



## Models and features for simulations


### General representation of walls

Walls can be defined in three ways in PhasicFlow:
* **Builtin walls** in PhasicFlow that include plane wall, cylinder/cone wall, cuboid, circle.
* **stl wall**  that reads the data of the wall from an ASCII stl file.
* **foamPatch wall** that reads the OpenFOAM mesh and converts the boundary patches into PhasicFlow walls (this feature is only available when performing CFD-DEM simulation using OpenFOAM).

Walls can be fixed or in motion during simulations. Various motion models are implemented to cover most of the wall motions in phasicFlow ([see the source code] (./../../../src/MotionModel/)):
* **fixedWall**  model, in which all walls are fixed. This model is mostly useful for granular flow under gravity or gas-solid flows (CFD-DEM). 
* **rotatingAxisMotion**  model, in which walls are rotating around an axis of rotation with specified rotation speed. This model covers a wide range of granular flows in which the whole or a part of geometry is rotating, like mixers. 
* **multiRotatingAxisMotion** model, in which a combination of rotations can be specified. One axis of rotation can itself have another axis of rotation, and so on. This creates the possibility of defining very complex motion pattern for walls, like what we see in Nauta blenders.
* **vibratingMotion** model, in which walls vibrates based on a sinusoidal model with specified frequency and amplitude.
In addition to these models, the user can add other motion models to the code based on their need. 


### High precision integeration methods
The precision of integration in a DEM simulation is very important. Since sudden changes in the interaction forces occur during simulations (when objects contact or when they rebound). High precision integration methods makes it possible to accurately track position and velocity of objects (specially when they are in contact). When using these methods, it is possible to choose larger time steps for integration without loosing accuracy and causing instability in the simulation. Although  a high-precision integration requires more computations, but the benefits of choosing larger time steps in simulation can totally compensate it. 
Various integration methods are implemented in PhasicFlow:

| Integration Method | Order | Type|
| :--- | :---: | :---: |
| AdamsBashforth2 | 2 | one-step |
| AdamsBashforth3 | 3 | one-step |
| AdamsBashforth4 | 4 | one-step |
| AdamsBashforth5 | 5 | one-step |
| AdamsMoulton3 | 3 | predictor-corrector |
| AdamsMoulton4 | 4 | predictor-corrector |
| AdamsMoulton5 | 5 | predictor-corrector |


### Contact force models
Linear and non-linear visco-elastic contact force models are considered in the simulation. In addition to these, limited and non-limited Coulomb's friction model can be used to account for the friction between objects. For spherical objects, rolling friction can also be specified between bodies in contact. 

### Particle insertion
Particles can be inserted during simulation from specified region at specified rate and time interval. Any number of insertion regions can be defined in a simulation. Various region types are considered here: box, cylinder and sphere. Particles are inserted into the simulation through the specified region.

### restarting/resuming a simulation
It is possible to resume a simulation fron any time-folder that is avaiable in the simulation case setup directory. PhasicFlow restart the simulation from that time folder.

## Postprocessing tools

* **pFlowToVTK** is used to convert simulation results into vtk file format. vtk file format can be read by Paraview for visualizing the results.
* **postprocessPhasicFlow** is a tool for performing various cell-based averaging on the fields. 
