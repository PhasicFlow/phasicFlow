<div align ="center">
<img src="doc/phasicFlow_logo_github.png" style="width: 400px;">
</div>


**PhasicFlow** is a parallel C++ code for performing DEM simulations. It can run on shared-memory multi-core computational units such as multi-core CPUs or GPUs (for now it works on CUDA-enabled GPUs). The parallelization method mainly relies on loop-level parallelization on a shared-memory computational unit. You can build and run PhasicFlow in serial mode on regular PCs, in parallel mode for multi-core CPUs, or build it for a GPU device to off-load computations to a GPU. In its current statues you can simulate millions of particles (up to 32M particles tested) on a single desktop computer. You can see the [performance tests of PhasicFlow](https://github.com/PhasicFlow/phasicFlow/wiki/Performance-of-phasicFlow) in the wiki page. 

## How to build?
You can build PhasicFlow for CPU and GPU executions. [Here is a complete step-by-step procedure](https://github.com/PhasicFlow/phasicFlow/wiki/How-to-Build-PhasicFlow).

## Online code documentation
You can find a full documentation of the code, its features, and other related materials on [online documentation of the code](https://phasicflow.github.io/phasicFlow/)

## How to use PhasicFlow?
You can navigate into [tutorials folder](./tutorials) in the phasicFlow folder to see some simulation case setups. If you need more detailed discription, visit our [wiki page tutorials](https://github.com/PhasicFlow/phasicFlow/wiki/Tutorials).  

## [PhasicFlowPlus](https://github.com/PhasicFlow/PhasicFlowPlus)
PhasicFlowPlus is and extension to PhasicFlow for simulating particle-fluid systems using resolved and unresolved CFD-DEM. [See the repository of this package.](https://github.com/PhasicFlow/PhasicFlowPlus)


## Supporting packages
* [Kokkos](https://github.com/kokkos/kokkos) from National Technology & Engineering Solutions of Sandia, LLC (NTESS)
* [CLI11 1.8](https://github.com/CLIUtils/CLI11) from University of Cincinnati.

