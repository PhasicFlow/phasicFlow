<div align="center">
  <img src="doc/phasicFlow_logo_github.png" style="width: 400px;" alt="PhasicFlow Logo">
</div>

## **PhasicFlow: High-Performance Discrete Element Method Simulations**

PhasicFlow is a robust, open-source C++ framework designed for the efficient simulation of granular materials using the Discrete Element Method (DEM). Leveraging parallel computing paradigms, PhasicFlow is capable of executing simulations on shared-memory multi-core architectures, including CPUs and NVIDIA GPUs (CUDA-enabled). The core parallelization strategy focuses on loop-level parallelism, enabling significant performance gains on modern hardware. Users can seamlessly transition between serial execution on standard PCs, parallel execution on multi-core CPUs (OpenMP), and accelerated simulations on GPUs. Currently, PhasicFlow supports simulations involving up to 80 million particles on a single desktop workstation. Detailed performance benchmarks are available on the [PhasicFlow Wiki](https://github.com/PhasicFlow/phasicFlow/wiki/Performance-of-phasicFlow).

**Scalable Parallelism: MPI Integration**

Ongoing development includes the integration of MPI-based parallelization with dynamic load balancing. This enhancement will extend PhasicFlow's capabilities to distributed memory environments, such as multi-GPU workstations and high-performance computing clusters. Upon completion, PhasicFlow will offer six distinct execution modes:

1.  **Serial Execution:** Single-core CPU.
2.  **Shared-Memory Parallelism:** Multi-core CPU (OpenMP).
3.  **GPU Acceleration:** NVIDIA GPU (CUDA).
4.  **Distributed-Memory Parallelism:** MPI.
5.  **Hybrid Parallelism:** MPI + OpenMP.
6.  **Multi-GPU Parallelism:** MPI + CUDA.

# **Build and Installation**

PhasicFlow can be compiled for both CPU and GPU execution.

* **Current Development (v-1.0):** Comprehensive build instructions are available [here](https://github.com/PhasicFlow/phasicFlow/wiki/How-to-build-PhasicFlow%E2%80%90v%E2%80%901.0).
* **Latest Release (v-0.1):** Detailed build instructions are available [here](https://github.com/PhasicFlow/phasicFlow/wiki/How-to-Build-PhasicFlow).

# **Comprehensive Documentation**

In-depth documentation, including code structure, features, and usage guidelines, is accessible via the [online documentation portal](https://phasicflow.github.io/phasicFlow/).

## **Tutorials and Examples**

Practical examples and simulation setups are provided in the [tutorials directory](./tutorials). For detailed explanations and step-by-step guides, please refer to the [tutorial section on the PhasicFlow Wiki](https://github.com/PhasicFlow/phasicFlow/wiki/Tutorials).

# **PhasicFlowPlus: Coupled CFD-DEM Simulations**

PhasicFlowPlus is an extension of PhasicFlow that facilitates the simulation of particle-fluid systems using resolved and unresolved CFD-DEM methods. The repository for PhasicFlowPlus can be found [here](https://github.com/PhasicFlow/PhasicFlowPlus).

# Contributing to PhasicFlow
We welcome contributions to PhasicFlow! Whether you're a developer or a new user, there are many ways to get involved. Here's how you can help:
1. Bug Reports
2. Suggestions for better user experience
3. Feature request and algorithm improvements
4. Tutorials, Simulation Case Setups and documentation
5. Direct Code Contributions

For more details on how you can contribute to PhasicFlow see [this page](https://github.com/PhasicFlow/phasicFlow/wiki/How-to-contribute-to-PhasicFlow). 

# How to cite PhasicFlow?

If you are using PhasicFlow in your research or industrial work, cite the following [article](https://www.sciencedirect.com/science/article/pii/S0010465523001662):

```
@article
{
  NOROUZI2023108821,
  title = {PhasicFlow: A parallel, multi-architecture open-source code for DEM simulations},
  journal = {Computer Physics Communications},
  volume = {291},
  pages = {108821},
  year = {2023},
  issn = {0010-4655},
  doi = {https://doi.org/10.1016/j.cpc.2023.108821},
  url = {https://www.sciencedirect.com/science/article/pii/S0010465523001662},
  author = {H.R. Norouzi},
  keywords = {Discrete element method, Parallel computing, CUDA, GPU, OpenMP, Granular flow}
}
```

# **Dependencies**

PhasicFlow relies on the following external libraries:

* **Kokkos:** A performance portability ecosystem developed by National Technology & Engineering Solutions of Sandia, LLC (NTESS). ([https://github.com/kokkos/kokkos](https://github.com/kokkos/kokkos))
* **CLI11 1.8:** A command-line interface parser developed by the University of Cincinnati. ([https://github.com/CLIUtils/CLI11](https://github.com/CLIUtils/CLI11))
