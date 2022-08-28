# phasicFlow
phasicFlow is a parallel C++ code for performing DEM simulations. It can run on shared-memory multi-core computational units such as multi-core CPUs or GPUs (for now it works on CUDA-enabled GPUs). The parallelization method mainly relies on loop-level parallelization on a shared-memory computational unit. You can build and run phasicFlow in serial mode on regualr PCs, in parallel mode for multi-core CPUs, or build it for a GPU device to off-load computations to a GPU. 



# Requirements
## Compilers
phasicFlow should work with every gnu compiler that implements C++17 standards. For now, it is tested on Ubuntu distribution of linux operating systems. If the minimum requirements are met, there should not be any problem with compiling the code. However, there are always compiler bugs from one version to another that may need you extra attempts for upgrading to newer versions or downgrading to prior versions of the compiler.
* for CPU builds: It requires gcc-6.x or higher. 
* for GPU (CUDA-enabled): NVCC-10.x or higher.

## Required packages
phasicFlow uses [Kokkos]( https://github.com/kokkos/kokkos) as the backend for parallelization. So, you need to  have the code in the local machine alongside phasicFlow. The make system is adjusted in a way so you do not need to compile Kokkos separately and the required source code files from Kokkos are compiled alongside the phasicFlow.

## CMake
You also need to have CMake installed on your computer. At the moment CMake-3.22 or higher is needed. 

# How to build? 
phasicFlow uses CMake as the build system. you need to have CMake-3.22 or higher installed on your machine. you can either use the command line for setting-up the build system, or use `cmake-gui` to setup your build system through the GUI. 

### build for serial execution
 
### build for parallel execution on CPU
 
### build for parallel execution on CUDA-enabled GPUs
 
# How to use phasicFlow?
  
# Extentions in future
### parallelization 
* Extending the code for using OpenMPTarget backend to include more GPUs for off-loading the computations. 
* Extending high-level parallelization and impelmenting spacial partitioning and load balancing for muilti-GPU computations and running phasicFlow on distributed memory super-computers 
 
### basic features 
