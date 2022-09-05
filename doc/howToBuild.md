# How to build PhasicFlow? 
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
