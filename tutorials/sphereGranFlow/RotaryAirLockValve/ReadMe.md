# Problem Definition
The problem is to simulate a Rotary Air-Lock Valve with below diminsions:
* Size of Cone:
    * Cone Gate: 29.17 cm
    * Cone Exit: 10.37 cm
* Size of Outer Exit: 9.42 cm
* External diameter of Circle: 20.74 cm
There is one type of particle in this blender. Particles are poured into the inlet valve from t=**0** s.
* **28000** particles with **5 mm** diameter poured into the valve with rate of **4000 particles/s**.

<html>
<body>
<div align="center"><b>
	a view of the Rotary Air-Lock Valve while rotating 
</div></b>
<div align="center">
<img src="sample sample sample sample", width=700px>
</div>
<div align="center"><i>
	particles are colored according to their velocity
</div></i>
</body>
</html>

# Setting up the Case
As it has been explained in the previous cases, the simulation case setup is based on text-based scripts. Here, the simulation case setup files are stored into three folders: `caseSetup`, `setting`, `stl`  (see the above folders). See next the section for more information on how we can setup the geometry and its rotation.
## Geometry 

### Defining rotation axis
In file `settings/geometryDict` the information of rotating axis and speed of rotation are defined. The rotation of this blender starts at time=**0 s** and ends at time=**7 s**.
```C++
// information for rotatingAxisMotion motion model 
rotatingAxisMotionInfo
{
	rotAxis 
	{

		// first point for the axis of rotation
		p1 (0.561547 0.372714 0.000);

		// second point for the axis of rotation
		p2 (0.561547 0.372714 0.010);

		// rotation speed (rad/s)
		omega 2.1;

		// Start time of Geometry Rotating (s)
		startTime 1.25;
		
		// End time of Geometry Rotating (s)
		endTime 7;
	}
}
```
### Surfaces 
In `settings/geometryDict` file, the surfaces component are defined to form a Rotating Air-Lock Valve.
```C++
surfaces
{
	gear
	{
		// type of the wall
		type 	 stlWall;

		// file name in stl folder
		file 	 gear.stl;

		// material name of this wall
		material wallMat;

		// motion component name 
		motion 	 rotAxis;
	}
surfaces
	{
		// type of the wall
		type 	 stlWall;

		// file name in stl folder
		file 	 surfaces.stl;

		// material name of this wall
		material wallMat;

		// motion component name 
		motion 	 none;
}
```
## Defining particles 
### Diameter and material of spheres 
In the `caseSetup/sphereShape` the diameter and the material name of the particles are defined.

<div align="center"> 
in <b>caseSetup/sphereShape</b> file
</div>

```C++
// names of shapes
names 		(sphere);

// diameter of shapes 
diameters 	(0.005);

// material names for shapes 
materials	(sphereMat);
```
### Particle positioning before start of simulation

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
// positions particles 
positionParticles
{

	// creates the required fields with zero particles (empty).
	method empty;

	// maximum number of particles in the simulation
	maxNumberOfParticles 	50000;

	// perform initial sorting based on morton code?
	mortonSorting 		Yes;
}
```

## Interaction between particles
 In `caseSetup/interaction` file, material names and properties and interaction parameters are defined. Since we are defining 1 material type in the simulation, the interaction matrix is 2x2 (interactions are symmetric).
 ```C++
 // a list of materials names
materials   (sphereMat  wallMat);

// density of materials [kg/m3]
densities   (1000    2500);

contactListType   sortedContactList;

model
{
    contactForceModel nonLinearNonLimited;

    rollingFrictionModel normal;
   
    /*
    Property (sphereMat-sphereMat  sphereMat-wallMat
                                  wallMat-wallMat);
    */
   
    // Young modulus [Pa]
    Yeff  (1.0e6 1.0e6
                 1.0e6);

    // Shear modulus [Pa]
    Geff  (0.8e6 0.8e6
                 0.8e6);

    // Poisson's ratio [-]
    nu    (0.25 0.25
                0.25);

    // coefficient of normal restitution
    en    (0.7  0.8
                1.0);

    // coefficient of tangential restitution
   et    (1.0   1.0
                1.0);

    // dynamic friction
    mu    (0.3  0.35
                0.35);

    // rolling friction
    mur   (0.1  0.1
                0.1);     
}
```
# Performing Simulation and previewing the results 
To perform simulations, enter the following commands one after another in the terminal. 

Enter `$ particlesPhasicFlow` command to create the initial fields for particles.  
Enter `$ geometryPhasicFlow` command to create the geometry.  
At last, enter `$ sphereGranFlow` command to start the simulation.  
After finishing the simulation, you can use  `$ pFlowtoVTK` to convert the results into vtk format stored in ./VTK folder.