# Problem Definition
The problem is to simulate a double pedestal tote blender with the diameter **0.03 m** and **0.1 m** respectively, the length **0.3 m**, rotating at **28 rpm**. This blender is filled with **20000** Particles. The timestep for integration is **0.00001 s**. There is one type of Particle in this blender that are being inserted during simulation to fill the drum.
* **20000** particles with **4 mm** diameter, at the rate of 20000 particles/s for 1 sec.

<html>
<body>
<div align="center"><b>
	a view of the tote-blender while rotating 
</div></b>
<div align="center">
<img src="sample sample sample sample", width=700px>
</div>
</body>
</html>

# Setting up the Case
As it has been explained in the previous cases, the simulation case setup is based on text-based scripts. Here, the simulation case setup are sotred in two folders: `caseSetup`, `setting`. (see the above folders). Unlike the previous cases, this case does not have the `stl` file. and the geometry is described in the `geometryDict` file.

## Defining particles 
Then in the `caseSetup/sphereShape` the diameter and the material name of the particles are defined.
```C++
// names of shapes 
names 		(sphere1); 	
// diameter of shapes (m)
diameters 	(0.004);
// material names for shapes 	
materials	(prop1);	
```
## Particle Insertion
In this case we have a region for ordering particles. These particles are placed in this blender. For example the script for the inserted particles is shown below.

<div align="center"> 
in <b>caseSetup/particleInsertion</b> file
</div>

```C++
// positions particles 
positionParticles
{
// ordered positioning
	method positionOrdered;     
// maximum number of particles in the simulation
	maxNumberOfParticles 40000;
// perform initial sorting based on morton code? 
	mortonSorting Yes;             
// box for positioning particles 
	box  
	{
// lower corner point of the box 	
		min (-0.06 -0.06 0.08);
// upper corner point of the box 
		max (0.06 0.06 0.18);
	}
```
 ## Interaction between particles
 In `caseSetup/interaction` file, material names and properties and interaction parameters are defined: interaction between the particles of rotating drum. Since we are defining 1 material for simulation, the interaction matrix is 1x1 (interactions are symetric). 
```C++
 // a list of materials names
materials      (prop1);
// density of materials [kg/m3]
densities      (1000.0);   

contactListType   sortedContactList; 

model
{
   contactForceModel nonLinearNonLimited;
   rollingFrictionModel normal;
   /*
   Property (prop1-prop1);
   */
// Young modulus [Pa]
   Yeff  (1.0e6);       
// Shear modulus [Pa]
   Geff  (0.8e6);       
// Poisson's ratio [-]
   nu    (0.25);        
// coefficient of normal restitution
   en    (0.7);         
// coefficient of tangential restitution
   et    (1.0);          
// dynamic friction
   mu    (0.3);          
// rolling friction
   mur   (0.1);          
        
}
```
## Settings
### Geometry
In the `settings/geometryDict` file, the geometry and axis of rotation is defined for the drum. The geometry is composed of a cylinder inlet and outlet, cone shell top and down, a cylinder shell and enter and exit Gate.
```C++
surfaces
{
	enterGate
	{
	// type of wall
		type planeWall;
	// coords of wall
		p1 (-0.05    -0.05    0.3);
		p2 (-0.05    0.05     0.3);
		p3 ( 0.05    0.05     0.3);
		p4 (0.05     -0.05    0.3);
	// material of wall
		material prop1;
	// motion component name
		motion rotAxis;	
	}
	
    cylinderinlet
	{
	// type of the wall
		type 		cylinderWall;
	// begin point of cylinder axis 	
		p1 			(0.0 0.0 0.28);
	// end point of cylinder axis  
		p2 			(0.0 0.0 0.3);
	// radius at p1  
		radius1 	0.03;
	// radius at p2		
		radius2 	0.03;
	// number of divisions		
		resolution 	36;
	// material name of this wall
		material 	prop1;
	// motion component name   	
		motion rotAxis;		
	}

	coneShelltop
	{
	// type of the wall	
		type 		cylinderWall;
	// begin point of cylinder axis  	
		p1 			(0.0 0.0 0.2);
	// end point of cylinder axis  
		p2 			(0.0 0.0 0.28);
	// radius at p1  
		radius1 	0.1;
	// radius at p2		
		radius2 	0.03;
	// number of divisions		
		resolution 	36;
	// material name of this wall      	
		material 	prop1;
	// motion component name   	
		motion rotAxis;		
	}

	cylinderShell
	{
	// type of the wall
		type 		cylinderWall;
	// begin point of cylinder axis  	
		p1 			(0.0 0.0 0.1);
	// end point of cylinder axis
		p2 			(0.0 0.0 0.2);
	// radius at p1	  
		radius1 	0.1;
	// radius at p2			
		radius2 	0.1;
	// number of divisions		
		resolution 	36;
	// material name of this wall	      	
		material 	prop1; 
	// motion component name  	
		motion rotAxis;		
	}

	coneShelldown
	{
	// type of the wall
		type 		cylinderWall;
	// begin point of cylinder axis  	
		p1 			(0.0 0.0 0.02);
	// end point of cylinder axis  
		p2 			(0.0 0.0 0.1);
	// radius at p1  
		radius1 	0.03;
	// radius at p2		
		radius2 	0.1;
	// number of divisions		
		resolution 	36;
	// material name of this wall	      	
		material 	prop1;
	// motion component name   	
		motion rotAxis;		
	}
	/*
	This is a plane wall at the exit of silo
	*/

	    cylinderoutlet
	{
	// type of the wall
		type 		cylinderWall;  	
	// begin point of cylinder axis	
		p1 			(0.0 0.0 0.0);
	// end point of cylinder axis	  
		p2 			(0.0 0.0 0.02);
	// radius at p1  
		radius1 	0.03;
	// radius at p2			
		radius2 	0.03;
	// number of divisions			
		resolution 	36;
	// material name of this wall	      	
		material 	prop1;
	// motion component name	   	
		motion rotAxis;		
	}
	exitGate
	{
		type planeWall;
		p1 (-0.05    -0.05    0);
		p2 (-0.05    0.05     0);
		p3 ( 0.05    0.05     0);
		p4 (0.05     -0.05    0);
		material prop1;
		motion rotAxis;		
	}
		
}
```
### Rotating Axis Info
In this part of `geometryDict` the information of rotating axis and speed of rotation are defined. Unlike the previous cases, the rotation of this blender starts at time=**0 s**.
```C++
rotatingAxisMotionInfo
{
	rotAxis 
	{
		p1 (-0.1 0.0 0.15);	// first point for the axis of rotation 
		p2 (0.1 0.0 0.15);	// second point for the axis of rotation
		omega 3; 		// rotation speed (rad/s)
	}
}
```
## Performing Simulation
To perform simulations, enter the following commands one after another in the terminal. 

Enter `$ particlesPhasicFlow` command to create the initial fields for particles.  
Enter `$ geometryPhasicFlow` command to create the Geometry.  
At last, enter `$ sphereGranFlow` command to start the simulation.  
After finishing the simulation, you can use  `$ pFlowtoVTK` to convert the results into vtk format storred in ./VTK folder.