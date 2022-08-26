# phasicFlow
phasicFlow is a parallel C++ code for performing DEM simulations. It can run on shared-memory multi-core computational units such as multi-core CPUs or GPUs (for now it works on CUDA-enabled GPUs). The parallelization method now mainly relies on loop-level parallelization on a shared-memory computational unit. You can build and run phasic flow in serial mode on regualr PCs, build it for multi-core CPUs to be executed in parallel, or build it for a GPU device to off-load computations to GPU. 

# Required packages
phasicFlow uses [Kokkos]( https://github.com/kokkos/kokkos) as the backend for parallelization. So, you need to also have the code in the local machine alongside phasicFlow. The make system is adjusted in a way so you do not need to compile Kokkos separately and the required code is compiled alongside the phasicFlow.


# Requirements
phasicFlow should work with every gnu compiler that implements C++17 standards. For now, it is tested on Ubuntu distribution of linux operating systems. If the minimum requirements are met, there should not be any problem with compiling the code. However, there are always compiler bugs from one version to another that may need extra attempts for upgrading to newer versions or downgrading to prior versions of the compiler.
* CPU builds: It requires gcc-6.x or higher. 
* GPU (CUDA): NVCC-10.x or higher.


