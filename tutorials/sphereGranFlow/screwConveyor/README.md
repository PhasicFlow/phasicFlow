# Simulating a screw conveyor {#screwConveyor}
## Problem definition
The problem is to simulate a screw conveyorwith the diameter 0.2 m and the length 1 m and 20 cm pitch. It is filled with 30,000 4-mm spherical particles. The timestep for integration is 0.00001 s.
<div align="center"><b>
a view of rotating drum

![]()
</b></div>

***

## Setting up the case 
PhasicFlow simulation case setup is based on the text-based scripts that we provide in two folders located in the simulation case folder: `settings` and `caseSetup` (You can find the case setup files in the above folders.
All the commands should be entered in the terminal while the current working directory is the simulation case folder (at the top of the `caseSetup` and `settings`).
 

### Creating particles

Open the file  `settings/particlesDict`. Two dictionaries, `positionParticles` and `setFields` position particles and set the field values for the particles. 
In dictionary `positionParticles`, the positioning `method` is `positionOrdered`, which position particles in order in the space defined by `box`. `box` space is defined by two corner points `min` and `max`. In dictionary `positionOrderedInfo`, `numPoints` defines number of particles; `diameter`, the distance between two adjacent particles, and `axisOrder` defines the axis order for filling the space by particles. 

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
positionParticles
{
	method empty;     		// creates the required fields with zero particles (empty). 

	maxNumberOfParticles 	50000; // maximum number of particles in the simulation
	mortonSorting 			Yes;    // perform initial sorting based on morton code?   

}
```
In dictionary `setFields`, dictionary `defaultValue` defines the initial value for particle fields (here, `velocity`, `acceleration`, `rotVelocity`, and `shapeName`). Note that `shapeName` field should be consistent with the name of shape that you later set for shapes (here one shape with name `sphere1`).

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
setFields
{
	defaultValue 
	{
		velocity 		realx3 	(0 0 0); 		// linear velocity (m/s)
		acceleration 	realx3 	(0 0 0); 		// linear acceleration (m/s2)
		rotVelocity 	realx3 	(0 0 0);  		// rotational velocity (rad/s)
		shapeName 		word	lightSphere; 	// name of the particle shape 
	}

	selectors
	{}
}
```

Enter the following command in the terminal to create the particles and store them in `0` folder.

`> particlesPhasicFlow`
 
### Creating geometry
In file `settings/geometryDict` , you can provide information for creating geometry. Each simulation should have a `motionModel` that defines a model for moving the surfaces in the simulation. `rotatingAxisMotion` model defines a fixed axis which rotates around itself. The dictionary `rotAxis` defines an motion component with `p1` and `p2` as the end points of the axis and `omega` as the rotation speed in rad/s. You can define more than one motion component in a simulation. 

<div align="center"> 
in <b>settings/geometryDict</b> file
</div>

```C++
motionModel rotatingAxisMotion; 
.
.
.
rotatingAxisMotionInfo
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
In the dictionary `surfaces` you can define all the surfaces (shell) in the simulation. Two main options are available: built-in geometries in PhasicFlow, and providing surfaces with stl file. Here we use built-in geometries. In `cylinder` dictionary, a cylindrical shell with end helix, `material` name `prop1`, `motion` component `none` is defined. `helix` define  plane helix at center of  cylindrical shell, `material` name `prop1` and `motion` component `rotAxis`.  

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
		material prop1;   // material name of this wall
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
In the file `caseSetup/interaction` , you find properties of materials. `materials` defines a list of material names in the simulation and `densities` sets the corresponding density of each material name. model dictionary defines the interaction model for particle-particle and particle-wall interactions. `contactForceModel` selects the model for mechanical contacts (here nonlinear model with limited tangential displacement) and `rollingFrictionModel` selects the model for calculating rolling friction. Other required prosperities should be defined in this dictionary. 

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

Dictionary `contactSearch` sets the methods for particle-particle and particle-wall contact search. `method` specifies the algorithm for finding neighbor list for particle-particle contacts and `wallMapping` shows how particles are mapped onto walls for finding neighbor list for particle-wall contacts. `updateFrequency` sets the frequency for updating neighbor list and `sizeRatio` sets the size of enlarged cells (with respect to particle diameter) for finding neighbor list. Larger `sizeRatio` include more particles in the neighbor list and you require to update it less frequent. 

<div align="center"> 
in <b>caseSetup/interaction</b> file
</div>

```C++
contactSearch
{
   method         NBS;           // method for broad search particle-particle
   wallMapping    cellMapping;   // method for broad search particle-wall 

   NBSInfo
   {
      updateFrequency 10;        // each 20 timesteps, update neighbor list 
      sizeRatio      1.1;        // bounding box size to particle diameter (max)
   }

   cellMappingInfo
   {
      updateFrequency 10;        // each 20 timesteps, update neighbor list  
      cellExtent     0.6;        // bounding box for particle-wall search (> 0.5)
   }

}
```

In the file `caseSetup/sphereShape`, you can define a list of `names` for shapes (`shapeName` in particle field), a list of diameters for shapes and their `properties` names. 

<div align="center"> 
in <b>caseSetup/sphereShape</b> file
</div>

```C++
names 		(sphere1); 	// names of shapes 
diameters 	(0.01);	// diameter of shapes 
materials	(prop1);	// material names for shapes 
```

Other settings for the simulation can be set in file `settings/settingsDict`. The dictionary `domain` defines the a rectangular bounding box with two corner points for the simulation. Each particle that gets out of this box, will be deleted automatically. 

<div align="center"> 
in <b>settings/settingsDict</b> file
</div>

```C++
dt 				0.0001; 	// time step for integration (s)
startTime 		0; 			// start time for simulation 
endTime 		20;	 		// end time for simulation 
saveInterval 	0.05; 		// time interval for saving the simulation
timePrecision   6;			// maximum number of digits for time folder 
g 				(0 -9.8 0); // gravity vector (m/s2) 

domain 
{
	min (0.0 -0.06 0.001);
	max (1.2 1 0.5);
}

integrationMethod 		AdamsBashforth3; 	// integration method 

timersReport 			Yes;  	// report timers?

timersReportInterval   	0.01;	// time interval for reporting timers
```

## Running the case 
The solver for this simulation is `sphereGranFlow`. Enter the following command in the terminal. Depending on the computational power, it may take a few minutes to a few hours to complete. 

`> sphereGranFlow`

## Post processing 
After finishing the simulation, you can render the results in Paraview. To convert the results to VTK format, just enter the following command in the terminal. This will converts all the results (particles and geometry) to VTK format and store them in folder `VTK/`. 

`> pFlowToVTK`
