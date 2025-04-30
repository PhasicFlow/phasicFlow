# How to build PhasicFlow-v-1.0

You can build PhasicFlow for CPU or GPU. You can have a single build or oven multiple builds on a machine. Here you learn how to have a single build of PhasicFlow, in various modes of execution. You can install PhasicFlow-v-1.0 on **Ubuntu-22.04 LTS** and **Ubuntu-24.04 LTS**. Installing it on older versions of Ubuntu needs some additional steps to meet the requirements which are not covered here. 

If you want to install PhasicFlow on **Windows OS**, just see [this page](https://www.cemf.ir/installing-phasicflow-v-1-0-on-ubuntu/) for more information. 

# Required packages
You need a list of packages installed on your computer before building PhasicFlow:
* git, for cloning the code and package management
* g++, for compiling the code
* cmake, for generating build system
* Cuda-12.x or above (if GPU is targeted), for compiling the code for CUDA execution.
 

### Installing packages 
Execute the following commands to install the required packages (Except Cuda). tbb is installed automatically. 

```bash
sudo apt update
sudo apt install -y git g++ cmake cmake-qt-gui
```
 

### Installing Cuda for GPU execution 
If you want to build PhasicFlow to be executed on an nvidia-GPU, you need to install the latest version of Cuda compiler (Version 12.x or above), which is compatible with your hardware and OS, on your computer. 

# How to build? 
Here you will learn how to build PhasicFlow for single execution mode. Follow the steps below to install it on your computer. 
Tested operating systems are:
* Ubuntu-22.04 LTS
* Ubuntu-24.04 LTS

### Step 1: Package check
Make sure that you have installed all the required packages on your computer. See above for more information.


### Step 2: Cloning PhasicFlow
Create the PhasicFlow folder in your home folder and then clone the source code into that folder:
```bash
cd ~
mkdir PhasicFlow
cd PhasicFlow
git clone https://github.com/PhasicFlow/phasicFlow.git
mv phasicFlow phasicFlow-v-1.0
```
### Step 3: Environmental variables
Opne the bashrc file using the following command:

```bash
$ gedit ~/.bashrc
```

and add the following line to the end of the file, **save** and **close** it.

```bash
source $HOME/PhasicFlow/phasicFlow-v-1.0/cmake/bashrc
```

this will introduce a new source file for setting the environmental variables of PhasicFlow. If you want to load these variables in the current open terminal, you need to source it. Or, simply **close the terminal** and **open a new terminal**.

### Step 4: Building PhasicFlow
Follow one of the followings to build PhasicFlow for one mode of execution.

#### Serial build for CPU
In a **new terminal** enter the following commands:
```bash
cd ~/PhasicFlow/phasicFlow-v-1.0
mkdir build
cd build
cmake ../ -DpFlow_Build_Serial=On -DCMAKE_BUILD_TYPE=Release
make install -j4
```
For faster builds, use `make install -j`. This will use all the CPU cores on your computer for building. 

#### OpenMP build for CPU

```bash
cd ~/PhasicFlow/phasicFlow-v-1.0
mkdir build
cd build
cmake ../ -DpFlow_Build_OpenMP=On -DCMAKE_BUILD_TYPE=Release
make install -j4
```

#### GPU build for parallel execution on CUDA-enabled GPUs

```bash
cd ~/PhasicFlow/phasicFlow-v-1.0
mkdir build
cd build
cmake ../ -DpFlow_Build_Cuda=On -DCMAKE_BUILD_TYPE=Release
cmake ../ -DpFlow_Build_Cuda=On -DCMAKE_BUILD_TYPE=Release
make install -j4
```

After building, `bin`, `include`, and `lib` folders will be created in `~/PhasicFlow/phasicFlow-v-1.0/` folder. Now you are ready to use PhasicFlow.

**note 1**: When compiling the code in parallel, you need to have enough RAM on your computer. As a rule, you need 1 GB free RAM per each processor on your computer for compiling in parallel.
You may want to use fewer number of cores on your computer by using the following command:

```bash
make install -j3
```

the above command only uses 3 cores for compiling. 

**note 2**: By default PhasicFlow is compiled with **double** as floating point variable. You can compile it with **float**. Just in the command line of camke added `-DpFlow_Build_Double=Off` flag to compile it with float. For example if you are building for cuda, you can enter the following command:

```bash
cmake ../ -DpFlow_Build_Cuda=On -DpFlow_Build_Double=Off
```

### Step 5: Testing
In the current terminal or a new terminal enter the following command:

```bash
checkPhasicFlow
```

This command shows the host and device environments and software version. If PhasicFlow was build correctly, you would get the following output:
```
Initializing host/device execution spaces . . . 
  Host execution space is Serial
  Device execution space is Serial

  You are using phasicFlow v-1.0 (copyright(C): www.cemf.ir)
  In this build, double is used for floating point operations and uint32for indexing.
  This is not a build for MPI execution

Finalizing host/device execution space ....
```
