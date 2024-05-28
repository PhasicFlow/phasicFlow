# Simulating a screw conveyor {#screwConveyor}
## Problem definition

The problem is to simulate a screw conveyor with a diameter of 0.2 m, a length of 1 m and a pitch of 20 cm. It is filled with 30,000 4 mm spherical particles. The integration time step is 0.00001 s.

<div align="center"><b>
a view of rotating drum

![]()
</b></div>

***

## Setting up the case 

The PhasicFlow simulation case setup is based on the text-based scripts that we provide in two folders located in the simulation case folder: `settings` and `caseSetup` (You can find the case setup files in the above folders.
All commands should be entered in the terminal with the current working directory being the simulation case folder (at the top of the `caseSetup` and `settings` folders).
 
### Creating particles

Open the file `settings/particlesDict`. Two dictionaries, `positionParticles` and `setFields`, position particles and set field values for the particles. 
In the dictionary `positionParticles`, the positioning method is `positionOrdered`, which positions particles in order in the space defined by `box`. The `box` space is defined by two corner points `min` and `max`. In the dictionary `positionOrderedInfo`, `numPoints` defines the number of particles, `diameter` the distance between two adjacent particles, and `axisOrder` the axis order for filling the space with particles. 

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
positionParticles
{
	method empty;     		        // other options: ordered and random 

        maxNumberOfParticles 	50000;          // maximum number of particles in the simulation

	regionType                box;          // other options: cylinder and sphere

	boxInfo                                 // box for positioning particles 
	{
		min    (-0.1 -0.08 0.015);          // lower corner point of the box 

		max       (0.1 0.0 0.098);          // upper corner point of the box 
	} 
}
```

Enter the following command in the terminal to create the particles and store them in `0` folder.

`> particlesPhasicFlow`
 
### Creating geometry

In the `settings/geometryDict` file you can provide information for creating geometry. Each simulation should have a `motionModel` which defines a model for moving the surfaces in the simulation. The `rotatingAxisMotion' model defines a fixed axis that rotates around itself. The dictionary `rotAxis` defines a motion component with `p1` and `p2` as the end points of the axis and `omega` as the speed of rotation in rad/s. You can define more than one motion component in a simulation. 

<div align="center"> 
in <b>settings/geometryDict</b> file
</div>

```C++
motionModel rotatingAxis; 
.
.
.
rotatingAxisInfo
{
	rotAxis 
	{
		p1 (1.09635 0.2010556 0.22313511);	// first point for the axis of rotation

		p2 (0.0957492 0.201556 0.22313511);	// second point for the axis of rotation

		omega 3; 		// rotation speed (rad/s)

		startTime 5;

		endTime 30;
	}
}

```
In the dictionary `surfaces` you can define all surfaces (shell) in the simulation. There are two main options: built-in geometries in PhasicFlow and providing surfaces with stl file. Here we will use built-in geometries. In the `cylinder` dictionary a cylindrical shell with end helix, `material` name `prop1`, `motion` component `none` is defined. In `helix` we define a plane helix at the center of the cylindrical shell, `material` name `prop1` and `motion` component `rotAxis`. `rotAxis` is used for the helix because it is rotating and `none` is used for the shell because it is not moving.

<div align="center"> 
in <b>settings/geometryDict</b> file
</div>

```C++
surfaces
{
	helix
	{
		type 	 stlWall;  	// type of the wall
		file 	 helix.stl;	// file name in stl folder		
		material prop1;         // material name of this wall
		motion 	 rotAxis;	// motion component name 
	}

	shell
	{
		type 	 stlWall;  	// type of the wall
		file 	 shell.stl;	// file name in stl folder		
		material prop1;   // material name of this wall
		motion 	 none;		// motion component name 
	}
}

```

Enter the following command in the terminal to create the geometry and store it in `0/geometry` folder.

`> geometryPhasicFlow`

### Defining properties and interactions 

The `caseSetup/interaction' file contains material properties. `materials` defines a list of material names in the simulation and `densities` sets the corresponding density of each material name. model dictionary defines the interaction model for particle-particle and particle-wall interactions. ContactForceModel selects the model for mechanical contacts (here nonlinear model with limited tangential displacement) and `rollingFrictionModel` selects the model for the calculation of rolling friction. Other required properties should be defined in this dictionary. 

<div align="center"> 
in <b>caseSetup/interaction</b> file
</div>

```C++
materials      (prop1);    // a list of materials names
densities      (1000.0);   // density of materials [kg/m3]

contactListType   sortedContactList; 

model
{
   contactForceModel nonLinearNonLimited;
   rollingFrictionModel normal;

   Yeff  (1.0e6);       // Young modulus [Pa]

   Geff  (0.8e6);       // Shear modulus [Pa]

   nu    (0.25);        // Poisson's ratio [-]

   en    (0.7);         // coefficient of normal restitution

   et    (1.0);         // coefficient of tangential restitution 

   mu    (0.3);         // dynamic friction 

   mur   (0.1);         // rolling friction 
        
}
```

Dictionary `contactSearch` sets the methods for particle-particle and particle-wall contact search. method' specifies the algorithm for finding the neighbor list for particle-particle contacts and `wallMapping' specifies how particles are mapped to walls for finding the neighbor list for particle-wall contacts. `updateFrequency` specifies the frequency for updating the neighbor list and `sizeRatio` specifies the size of enlarged cells (with respect to particle diameter) for neighbor list search. Larger `sizeRatio` includes more particles in the neighbor list and you need to update it less frequently. 

<div align="center"> 
in <b>caseSetup/interaction</b> file
</div>

```C++
contactSearch
{
   method         NBS;          // method for broad search particle-particle
   
   updateInterval  10;

   sizeRatio      1.1;

   cellExtent    0.55;

   adjustableBox   No;
}
```
In the file `caseSetup/sphereShape`, you can define a list of `names` for shapes (`shapeName` in particle field), a list of diameters for shapes and their `properties` names. 

<div align="center"> 
in <b>caseSetup/sphereShape</b> file
</div>

```C++
names 		(sphere1); 	// names of shapes 
diameters 	(0.01);	        // diameter of shapes 
materials	(prop1);	// material names for shapes 
```

Other settings for the simulation can be set in the `settings/settingsDict` file. The `domain' dictionary defines a rectangular bounding box with two corner points for the simulation. Any particle that leaves this box will be automatically deleted. 

<div align="center"> 
in <b>settings/settingsDict</b> file
</div>

```C++
dt 		0.0001; 	// time step for integration (s)
startTime 	0; 		// start time for simulation 
endTime 	20;	 	// end time for simulation 
saveInterval 	0.05; 		// time interval for saving the simulation
timePrecision   6;		// maximum number of digits for time folder 
g 		(0 -9.8 0);     // gravity vector (m/s2) 

domain 
{
	min (0.0 -0.06 0.001);
	max (1.2 1 0.5);
}

integrationMethod 		AdamsBashforth2; 	// integration method 

timersReport 			Yes;  	                // report timers?

timersReportInterval    	0.01;	                // time interval for reporting timers
```

## Running the case 

The solver for this simulation is `sphereGranFlow`. Enter the following command in the terminal. Depending on the computational power, it may take a few minutes to a few hours to complete. 

`> sphereGranFlow`

## Post processing 
After finishing the simulation, you can render the results in Paraview. To convert the results to VTK format, just enter the following command in the terminal. This will converts all the results (particles and geometry) to VTK format and store them in folder `VTK/`. 

`> pFlowToVTK`
