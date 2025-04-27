# Benchmarks of rotating drum (v-1.0) 
## Problem definition 
**The rotatingDrum case was calculated using well-known discrete element commercial software and phasicFlow, and the calculation time and memory usage were tested when the number of particles was 250 k, 500 k, 1 m, 2 m, 4 m and 8 m.**

<div align="center">
    <img src="./images/commerical DEM snapshot.png" alt="description" width="400" />
</div>


<div align="center">Figure 1. The snapshot of commerical DEM simulation</div>


<div align="center">
    <img src="./images/phasicFlow snapshot.png" alt="description" height="600" />
</div>


<div align="center">Figure 2. The snapshot of phasicFlow simulation</div>



**The performance tests were conducted on laptop computers and workstation computers respectively. The configuration information of the laptop computers and workstation computers is shown in Table 1.**

<div align="center">Table 1. The parameters of laptop and workstation.</div>

|  computer   |           CPU            |             GPU              | Operating system |
| :---------: | :----------------------: | :--------------------------: | :--------------: |
|   Laptop    | Intel i9-13900HX 2.2 GHz | NVIDIA Geforce RTX 4050Ti 6G | Windows 11 24H2  |
| workstation | Intel xeon 4210 2.2 GHz  |     NVIDIA RTX A4000 16G     |   Ubuntu 22.04   |

**A total of 6 groups of working condition tests were carried out, with the particle numbers being 250 k, 500 k, 1 m, 2 m, 4 m and 8 m respectively. The particle diameter, drum length and drum diameter under each working condition are shown in Table 2.**

<div align="center">Table 2. The parameters setting of different rotating drum.</div>

| Case name | Particle diameter | Particle numbers | Rotating drum length | Rotating drum radius |
| :-------: | :---------------: | :--------------: | :------------------: | :------------------: |
|   250 k   |       6 mm        |     250,000      |        0.8 m         |        0.2 m         |
|   500 k   |       5 mm        |     500,000      |        0.8 m         |        0.2 m         |
|    1 m    |       4 mm        |    1,000,000     |        0.8 m         |        0.2 m         |
|    2 m    |       3 mm        |    2,000,000     |        1.2 m         |        0.2 m         |
|    4 m    |       3 mm        |    4,000,000     |        1.6 m         |        0.2 m         |
|    8 m    |       2 mm        |    8,000,000     |        1.6 m         |        0.2 m         |

**Table 3 and Figure 3 show the calculation time of phasicFlow and well-known discrete element commercial software under different working conditions on a laptop. It can be seen from the table that the increase in calculation time is linearly proportional to the increase in the number of particles. The calculation speed of phasicFlow is about 20% faster than that of well-known discrete element commercial software. The calculation speed of using A4000 graphics card is about 30% faster than that of 4050Ti.**

<div align="center">Table 3  The calculation time of different software.</div>

|     Software      | 250 k  |  500 k  |   1 m   |   2 m   |   4 m   |   8 m    |
| :---------------: | :----: | :-----: | :-----: | :-----: | :-----: | :------: |
| phasicFlow-4050Ti | 54 min | 111 min | 216 min | 432 min |         |          |
| commerical DEM-4050Ti    | 68 min | 136 min | 275 min | 570 min |         |          |
| phasicFlow-A4000  | 38 min | 73 min  | 146 min | 293 min | 589 min | 1188 min |

<div align="center">
    <img src="../_static/images/Calculation time under different particle numbers using commerical DEM and phasicFlow.png" alt="description" height="600" />
</div>

<div align="center">Figure 3  The calculation time of different software.</div>



**Table 4 and Figure 4 show the memory usage of phasicFlow and well-known discrete element commercial software on a laptop under different working conditions. It can be seen from the table that phasicFlow uses about 0.7 GB of memory per million particles, while the well-known discrete element commercial software uses about 1.2 GB of memory per million particles.**

<div align="center">Table 4  The memory usage of different software.</div>

|     Software      |  250 k  |  500 k  |   1 m   |   2 m   |   4 m   |   8 m   |
| :---------------: | :-----: | :-----: | :-----: | :-----: | :-----: | :-----: |
| phasicFlow-4050Ti | 252 MB  | 412 MB  | 710 MB  | 1292 MB |         |         |
| commerical DEM-4050Ti    | 485 MB | 897 MB | 1525 MB | 2724 MB |         |         |
| phasicFlow-A4000  | 344 MB  | 480 MB  | 802 MB  | 1386 MB | 2590 MB | 4966 MB |



 <img src="../_static/images/Memory usage under different particle numbers using commerical DEM and phasicFlow.png" alt="description" height="600" />

<div align="center">Figure 4  The memory usage of different software.</div>