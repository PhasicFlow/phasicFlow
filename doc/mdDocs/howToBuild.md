# How to build PhasicFlow {#howToBuildPhasicFlow}

You can build PhasicFlow for CPU or GPU. You can have a single build or oven multiple builds on a machine. Here you learn how to have a single build of PhasicFlow, in various modes of execution. 

# Required packages
You need a list of packaged installed on your computer before building PhasicFlow:
* git, for cloning the code and package management
* g++, for compiling the code
* cmake, for generating build system
* tbb, a parallel library for STL algorithms
* Cuda (if GPU is targeted), for compiling the code for CUDA execution.
* Kokkos, the parallelization backend of PhasicFlow 

### git 
if git is not installed on your computer, enter the following commands 
```
$ sudo apt update
$ sudo apt install git
```
 

### g++ (C++ compiler)
The code is tested with g++ (gnu C++ compiler). The default version of g++ on Ubuntu 18.04 LTS or upper is sufficient for compiling. If it is not installed on your operating system, enter the following command:

``` 
$ sudo apt update
$ sudo apt install g++
```


### CMake
You also need to have CMake-3.22 or higher installed on your computer.
``` 
$ sudo apt update
$ sudo apt install cmake
```

### tbb (2020.1-2 or higher)
For **Ubuntu 20.04 LTS or higher versions**, you can install tbb using apt. For now, some parallel algorithms on host side rely on tbb parallel library (C++ parallel backend). Use e following commands to install it:
```
$ sudo apt update
$ sudo apt install libtbb-dev
```
If you are compiling on **Ubuntu-18.04 LTS**, you need to enter the following commands to get the right version (2020.1-2 or higher) of tbb:
```
$ wget "http://archive.ubuntu.com/ubuntu/pool/universe/t/tbb/libtbb2_2020.1-2_amd64.deb"
$ sudo dpkg --install libtbb2_2020.1-2_amd64.deb
$ wget "http://archive.ubuntu.com/ubuntu/pool/universe/t/tbb/libtbb-dev_2020.1-2_amd64.deb"
$ sudo dpkg --install libtbb-dev_2020.1-2_amd64.deb
```
### Cuda
If you want to build PhasicFlow to be executed on an nvidia-GPU, you need to install the latest version of Cuda compiler, which is compatible with your hardware and OS, on your computer. 

# How to build? 
Here you will learn how to build PhasicFlow for single execution mode. Follow the steps below to install it on your computer. 
Tested operating systems are:
* Ubuntu 18.04 LTS
* Ubuntu 20.04 LTS
* Ubuntu 22.04 LTS

### Step 1: Package check
Make sure that you have installed all the required packages on your computer. See above for more information.

### Step 2: Cloning Kokkos
It is assumed that Kokkos source is located in the home folder of your computer. Clone the latest version of Kokkos into your home folder:
```
$ cd ~
$ mkdir Kokkos
$ cd Kokkos
$ git clone https://github.com/kokkos/kokkos.git
```
or simply download and extract the source code of Kokkos in `~/Kokkos` folder. In the end, the top level CMakeLists.txt file should be located in `~/Kokkos/kokkos` folder. 

### Step 3: Cloning PhasicFlow
Create the PhasicFlow folder in your home folder and then clone the source code into that folder:
```
$ cd ~
$ mkdir PhasicFlow
$ cd PhasicFlow
$ git clone https://github.com/PhasicFlow/phasicFlow.git
```
### Step 4: Environmental variables
Opne the bashrc file using the following command:

`$ gedit ~/.bashrc`

and add the following line to the end of the file, **save** and **close** it.

`source $HOME/PhasicFlow/phasicFlow/cmake/bashrc`

this will introduce a new source file for setting the environmental variables of PhasicFlow. If you want to load these variables in the current open terminal, you need to source it. Or, simply **close the terminal** and **open a new terminal**.

### Step 5: Building PhasicFlow
Follow one of the followings to build PhasicFlow for one mode of execution.
#### Serial build for CPU
In a **new terminal** enter the following commands:
```
$ cd ~/PhasicFlow/phasicFlow
$ mkdir build
$ cd build
$ cmake ../ -DpFlow_Build_Serial=On
$ make install
```
For faster builds, use `make install -j`. This will use all the CPU cores on your computer for building. 
#### OpenMP build for CPU
```
$ cd ~/PhasicFlow/phasicFlow
$ mkdir build
$ cd build
$ cmake ../ -DpFlow_Build_OpenMP=On
$ make install
```
#### GPU build for parallel execution on CUDA-enabled GPUs
```
$ cd ~/PhasicFlow/phasicFlow
$ mkdir build
$ cd build
$ cmake ../ -DpFlow_Build_Cuda=On
$ make install
```

After building, `bin`, `include`, and `lib` folders will be created in `~/PhasicFlow/phasicFlow/` folder. Now you are ready to use PhasicFlow.

**note 1**: When compiling the code in parallel, you need to have enough RAM on your computer. As a rule, you need 1 GB free RAM per each processor in your computer for compiling in parallel.
You may want to use fewer number of cores on your computer by using the following command:
`$ make install -j 3`
the above command uses only 3 cores for compiling. 

**note 2**: By default PhasicFlow is compiled with **double** as floating point variable. You can compile it with **float**. Just in the command line of camke added `-DpFlow_Build_Double=Off` flag to compile it with float. For example if you are building for cuda, you can enter the following command:

`$ cmake ../ -DpFlow_Build_Cuda=On --DpFlow_Build_Double=Off`

### Step 6: Testing
In the current terminal or a new terminal enter the following command:

`$ ~checkPhasicFlow`

This command shows the host and device environments and software version. If PhasicFlow was build correctly, you would get the following output:
```
Initializing host/device execution spaces . . . 
  Host execution space is Serial
  Device execution space is Cuda

  ou are using phasicFlow v-0.1 (copyright(C): www.cemf.ir)
  In this build, double is used for floating point operations.


Finalizing host/device execution space ....
```
