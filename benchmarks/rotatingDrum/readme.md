# Benchmarks of rotating drum (v-1.0) 
## Problem definition 
Compare the computational efficiency by rotatingDrum tutorials using phasicFlow in different calculation loads. The calculation time and memory usage when the number of particles tested was 250 k, 500 k, 1 m, 2 m, 4 m and 8 m.
<div align="center">
<b>
    
A view of rotating drum
</b>
<b>

![](https://github.com/PhasicFlow/phasicFlow/blob/media/media/rotating-drum-s.png)

</b></div>

Performance tests were conducted separately on laptops and workstations, and the configuration information for laptops and workstations is shown in Table 1.
<div align="center">Table 1. The parameters of laptop and workstation.</div>

|  computer   |           CPU            |             GPU              | Operating system |
| :---------: | :----------------------: | :--------------------------: | :--------------: |
|   Laptop    | Intel i9-13900HX 2.2 GHz | NVIDIA Geforce RTX 4050Ti 6G |    Ubuntu 22.04  |
| workstation | Intel xeon 4210 2.2 GHz  |     NVIDIA RTX A4000 16G     |   Ubuntu 22.04   |


A total of 6 sets of operating condition tests were conducted, with particle quantities of 250 k, 500 k,  1 m,  2 m,  4 m and 8 m in sequence, the particle diameter, drum length, and drum diameter under each operating condition are shown in Table 2.
<div align="center">Table 2. The parameters setting of different rotating drum.</div>

| Case name | Particle diameter | Particle numbers | Rotating drum length | Rotating drum radius |
| :-------: | :---------------: | :--------------: | :------------------: | :------------------: |
|   250 k   |       6 mm        |     250,000      |        0.8 m         |        0.2 m         |
|   500 k   |       5 mm        |     500,000      |        0.8 m         |        0.2 m         |
|    1 m    |       4 mm        |    1,000,000     |        0.8 m         |        0.2 m         |
|    2 m    |       3 mm        |    2,000,000     |        1.2 m         |        0.2 m         |
|    4 m    |       3 mm        |    4,000,000     |        1.6 m         |        0.2 m         |
|    8 m    |       2 mm        |    8,000,000     |        1.6 m         |        0.2 m         |

Table 3 shows the calculation time of phasicFlow under different operating conditions on laptops and desktops. It can be seen from the table that the increase in calculation time is linearly proportional to the increase in particle count. Using an A4000 graphics card results in a calculation speed approximately 30% faster than using a 4050Ti.

<div align="center">Table 3  The calculation time of phasicFlow.</div>

|     Software      | 250 k  |  500 k  |   1 m   |   2 m   |   4 m   |   8 m    |
| :---------------: | :----: | :-----: | :-----: | :-----: | :-----: | :------: |
| phasicFlow-4050Ti | 54 min | 111 min | 216 min | 432 min |         |          |
| phasicFlow-A4000  | 38 min | 73 min  | 146 min | 293 min | 589 min | 1188 min |


Table 4 shows the memory usage of phasicFlow under different operating conditions on laptops and workstations. According to the table, phasicFlow uses approximately 0.7 GB of memory per million particles.

<div align="center">Table 4  The memory usage of phasicFlow.</div>

|     Software      |  250 k  |  500 k  |   1 m   |   2 m   |   4 m   |   8 m   |
| :---------------: | :-----: | :-----: | :-----: | :-----: | :-----: | :-----: |
| phasicFlow-4050Ti | 252 MB  | 412 MB  | 710 MB  | 1292 MB |         |         |
| phasicFlow-A4000  | 344 MB  | 480 MB  | 802 MB  | 1386 MB | 2590 MB | 4966 MB |

