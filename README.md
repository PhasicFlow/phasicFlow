# PhasicFlow
PhasicFlow is a parallel C++ code for performing DEM simulations. It can run on shared-memory multi-core computational units such as multi-core CPUs or GPUs (for now it works on CUDA-enabled GPUs). The parallelization method mainly relies on loop-level parallelization on a shared-memory computational unit. You can build and run PhasicFlow in serial mode on regular PCs, in parallel mode for multi-core CPUs, or build it for a GPU device to off-load computations to a GPU. 



# Requirements
### Compilers
PhasicFlow should work with every gnu compiler that implements C++17 standards. For now, it is tested on Ubuntu distribution of Linux operating systems. If the minimum requirements are met, there should not be any problem with compiling the code. However, there are always compiler bugs from one version to another that may need you extra attempts for upgrading to newer versions or downgrading to prior versions of the compiler.
* For CPU builds: It requires gcc-6.x or higher. 
* For GPU (CUDA-enabled): NVCC-10.x or higher.

### Required packages
PhasicFlow uses [Kokkos]( https://github.com/kokkos/kokkos) as the backend for parallelization. So, you need to  have the code in the local machine alongside PhasicFlow. The make system is adjusted in a way so you do not need to compile Kokkos separately and the required source code files from Kokkos are compiled alongside the PhasicFlow.

### CMake
You also need to have CMake installed on your computer. At the moment CMake-3.22 or higher is needed.

### tbb
For now, some parallel algorithms rely on tbb parallel library (C++ parallel backend). This dependency will be removed soon. Use the following commands to install it:

`sudo apt update`

`sudo apt install libtbb-dev`

# How to build? 
PhasicFlow uses CMake as the build system. you need to have CMake-3.22 or higher installed on your machine. you can either use the command line for setting-up the build system, or use `cmake-gui` to setup your build system through the GUI. 
### Step 1:
Make sure you have installed all the required packages on your computer (tbb and CMake and compilers). 
### Step 2: Cloning Kokkos
It is assumed that Kokkos source is located in the home folder of your OS. Clone the current version of Kokkos into your home folder:

```
cd ~
mkdir Kokkos
cd Kokkos
git clone https://github.com/kokkos/kokkos.git
```

or simply download and extract the source code of Kokkos in `~/Kokkos` folder. In the end, the top level CMakeLists.txt file should be located in `~/Kokkos/kokkos` folder. 

### Step 3: Cloning PhasicFlow
Create the PhasicFlow folder in your home folder and then clone the source code into that folder:

```
cd ~
mkdir PhasicFlow
cd PhasicFlow
git clone https://github.com/hamidrezanorouzi/phasicFlow.git
```
### Step 4: Environmental variables
In the terminal enter the following command:

`gedit ~/.bashrc`

and add the following line to the end of file, save and close it.

source $HOME/PhasicFlow/phasicFlow/cmake/bashrc

close the terminal. 

### Step 5: Building PhasicFlow
Follow one of the followings to build PhasicFlow for one mode of execution.
#### Serial build for CPU
Open a new terminal and enter the following commands:
```
cd ~/PhasicFlow/phasicFlow/build
cmake ../ -DpFlow_Build_Serial=On
make install
```
For faster builds, use `make install -j`. This will use all the CPU cores on your computer for building. 
#### OpenMP build for CPU
 
#### GPU build for parallel execution on CUDA-enabled GPUs



After building, `bin`, `include`, and `lib` folders will be created in `~/PhasicFlow/phasicFlow/` folder. Now you are ready to use PhasicFlow.
 
# How to use PhasicFlow?
You can navigate into tutorials folder in the phasicFlow folder to see some simulation case setup.

# Extensions in future
### parallelization 
* Extending the code for using OpenMPTarget backend to include more GPUs for off-loading the computations. 
* Extending high-level parallelization and implementing space partitioning and load balancing for muilti-GPU computations and running PhasicFlow on distributed memory super-computers 
 
