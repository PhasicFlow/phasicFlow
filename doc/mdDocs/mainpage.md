PhasicFlow is an open-source, parallel, multi-artitecture code for discrete element simulation (DEM) simulation of granular and multi-phase flows. It is developed in C++ and can be built and executed on **multi-core CPUs** or **GPUs**. It is fast and scalable code for large simulation. For now, we have performed simulations upto 32,000,000 particles on mid-range GPUs. The calculations can be done based on single precision (float type in C++) or double precision (double type in C++). This option can be selected when the code is first built.  
It is solves the Newton's second law of motion and Euler's second law of motion for the translational and rotational motion of spherical particles. The code is not restricted to a specifict geometry or wall motion. It is designed in a way that additional features can be added to the code very easily.

## Explore more
* [Features of PhasicFlow](./mdDocs/phasicFlowFeatures.md)
* [How to build PhasicFlow](https://github.com/PhasicFlow/phasicFlow/wiki/How-to-Build-PhasicFlow)
* How to setup and run granular flow simulations
* [Benchmarks and performance tests](https://github.com/PhasicFlow/phasicFlow/wiki/Performance-of-phasicFlow)



