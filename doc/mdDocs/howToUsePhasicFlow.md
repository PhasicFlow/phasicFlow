# How to use PhasicFlow {#howToUsePhasicFlow}
Here you will learn how to use PhasicFlow to setup a granular flow simulation. The inputs for simulation are supplied through some text-based files, called file dictionary, located in two folders: `settings` and `caseSetup`. These folders are located under the root case directory. 
All the commands are performed through terminal in which the current working directory is root case directory (you see `settings` and `caseSetup` folders when `ls` command is entered). The states of geometry and particles are stored in time folders with names that represent the time. When simulation is finished, one case use post-processing tool pFlowToVTK to convert the stored results in the time folder into VTK file format. The VTK file format can be read by Paraview. 
A set of tutorials with detailed descriptions are provided to show you how to use PhasicFlow for various granular flow problems. Here is a list of them.
* Tutorials based on sphereGranFlow solver 
	* Small rotating drum [(see on github.com)](https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/sphereGranFlow/rotatingDrumSmall)
	* Rotating drum with baffles [(see on github.com)](https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/sphereGranFlow/RotatingDrumWithBaffles)
	* Layered silo filling [(see on github.com)](https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/sphereGranFlow/layeredSiloFilling)
	* Simulation of a V-blender [(see on github.com)](https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/sphereGranFlow/V-blender)
	* Granular flow of particles with two particlce sizes[(see on github.com)](https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/sphereGranFlow/binarySystemOfParticles)
	* Simulation of a tote-blender [(see on github.com)](https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/sphereGranFlow/toteblender)

* Tutorials based on iterateGeometry solver
	* Defining an inclined screw conveyor [(see on github.com)](https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/iterateGeometry/inclinedScrewConveyor)

* Tutorials based on postprocessPhasicFlow 
	* Particle number fraction in a binray system [(see on github.com)](https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/postprocessPhasicFlow/segregation)
	

