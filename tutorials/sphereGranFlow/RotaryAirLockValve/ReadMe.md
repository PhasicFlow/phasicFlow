# Problem Definition
The problem is to simulate a Rotary Air-Lock Valve. The external diameter of rotor is about 21 cm. There is one type of particle in this simulation. Particles are inserted into the inlet of the valve from t=**0** s.
* **28000** particles with **5 mm** diameter are inserted into the valve with the rate of **4000 particles/s**. 
* The rotor starts its ortation at t = 1.25 s at the rate of 2.1 rad/s. 

<html>
<body>
<div align="center"><b>
	a view of the Rotary Air-Lock Valve while rotating 
</div></b>
<div align="center">
<img src="https://github.com/PhasicFlow/phasicFlow/blob/media/media/rotaryAirLock.gif", width=700px>
</div>
<div align="center"><i>
	particles are colored according to time of insertion
</div></i>
</body>
</html>

# Setting up the Case
As it has been explained in the previous simulations, the simulation case setup is based on text-based scripts. Here, the simulation case setup files are stored into three folders: `caseSetup`, `setting`, and `stl`  (see the above folders). See next the section for more information on how we can setup the geometry and its rotation.
  
## Geometry 

### Defining rotation axis
In file `settings/geometryDict` the information of rotating axis of rotor and its speed of rotation are defined. The rotation starts at t = **1.25 s** and ends at t = **7 s**.

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
In `settings/geometryDict` file, the surfaces component are defined to form a Rotating Air-Lock Valve. All surface components are supplied in stl file format. All stl files should be stored under 'stl' folder. 

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
### Insertion of Particles 
Insertion of particles starts from t = 0 s and ends at t = 7 s. A box is defined for the port from which particles are being inderted. The rate of insertion is 4000 particles per second. 

<div align="center"> 
in <b>settings/particleInsertion</b> file
</div>

```C++
topRegion
{

	// type of insertion region
	type  boxRegion;

	// insertion rate (particles/s)
	rate 	  4000;

	// Start time of Particles insertion (s)
	startTime 	  0;

	// End time of Particles insertion (s)	
	endTime   	  7;

	// Time interval between each insertion (s)
	interval       0.025;

	// Coordinates of BoxRegion (m,m,m)
	boxRegionInfo 
	{
		min ( 0.48 0.58 0.01 ); // (m,m,m)
		max ( 0.64 0.59 0.05 ); // (m,m,m)
	}

	setFields
	{
		// initial velocity of inserted particles
		velocity    realx3 (0.0 -0.6 0.0);  
	}

	mixture
	{
		sphere 1;  
	}
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
# Performing simulation and seeing the results 
To perform simulations, enter the following commands one after another in the terminal. 

Enter `$ particlesPhasicFlow` command to create the initial fields for particles (here the simulaiton has no particle at the beginning).  
Enter `$ geometryPhasicFlow` command to create the geometry.  
At last, enter `$ sphereGranFlow` command to start the simulation.  
After finishing the simulation, you can use  `$ pFlowtoVTK` to convert the results into vtk format stored in ./VTK folder.
