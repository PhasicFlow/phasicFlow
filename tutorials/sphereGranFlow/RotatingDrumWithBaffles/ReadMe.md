# Problem Definition (v-1.0)
The problem is to simulate a rotating drum with a diameter of 0.24 m, a length of 0.1 m and 6 baffles rotating at 15 rpm. This drum is filled with 20000 particles, the integration time step is 0.00001 s. There are 2 types of particles in this drum, each of which is inserted during the simulation to fill the drum.
* **12500** Particles with **4 mm** diameter, at the rate of 12500 particles/s for 1 sec. 
* **7500** Particles with **5mm** diameter, at the rate of 7500 particles/s for 1 sec.   

<html>
<body>
<div align="center"><b>
	a view of the drum while rotating 
</div></b>
<div align="center">
<img src="https://github.com/PhasicFlow/phasicFlow/blob/media/media/rotatingDrumBffl.gif", width=700px>
</div>
</body>
</html>

# Setting up the Case
As it has been explained in the previous cases, the simulation case setup is based on text-based scripts. Here, the simulation case setup are sorted in three folders: `caseSetup`, `setting` and `stl`. 

## Defining small and large particles 
Then in the `caseSetup/shapes` the diameter and the material name of the particles are defined.  Two sizes are defined: 4 and 5 mm. 
```C++
// names of shapes 
names 		(smallSphere largeSphere);
// diameter of shapes (m) 	
diameters 	(0.004 0.005);
// material names for shapes 			
materials	(lightMat heavyMat);
```


## Particle Insertion
In this case we have two regions for inserting the particles. In both regions we define the insertion rate, the start and end time of the insertion, information about the volume of space through which the particles are inserted. The insertion phase in the simulation is performed between times 0 and 1 second.   
For example, for the insertion region for inserting light particles is shown below.

<div align="center"> 
in <b>caseSetup/particleInsertion</b> file
</div>


```C++
// Right Layer Region
layerrightregion 
{
// type of insertion region
   timeControl           simulationTime;
   regionType	           cylinder;
// insertion rate (particles/s)    
   rate 	  12500;
// Start time of LightParticles insertion (s)
   startTime 	  0; 
// End time of LightParticles insertion (s)      
   endTime   	  1;
// Time Interval of LightParticles insertion (s)
   insertionInterval       0.025; 

   cylinderInfo 
   {
// Coordinates of cylinderRegion (m,m,m)
   	p2 (-0.15 0.25 0.05);
   	p1 (-0.15 0.24	0.05);
// radius of cylinder (m)
   	radius 0.035;
   }
}
```
## Interaction between particles and walls
The `caseSetup/interaction` file defines the material names and properties as well as the interaction parameters: the interaction between the particles and the shell of the rotating drum. Since we define 3 materials for simulation, the interaction matrix is 3x3, while we only need to enter upper triangle elements (interactions are symmetric). 

```C++
// a list of materials names
materials   (lightMat heavyMat wallMat); 
// density of materials [kg/m3]
densities   (1000 1500 2500);     

 /*
   Property (lightMat-lightMat   lightMat-heavyMat    lightMat-wallMat
                                 heavyMat-heavyMat    heavyMat-wallMat
                                                      wallMat-wallMat );
 */
// Young modulus [Pa]
   Yeff (1.0e6 1.0e6 1.0e6  
               1.0e6 1.0e6
                     1.0e6);
// Shear modulus [Pa]   
   Geff (0.8e6 0.8e6 0.8e6  
               0.8e6 0.8e6
                     0.8e6);
// Poisson's ratio [-]
   nu    (0.25 0.25  0.25  
               0.25  0.25
                     0.25);
// coefficient of normal restitution
   en (0.97  0.97    0.85   
             0.97    0.85
                     1.00);
// dynamic friction 
   mu (0.65   0.65 0.35   
              0.65 0.35
                   0.35);
// rolling friction 
   mur (0.1  0.1 0.1    
             0.1 0.1
                 0.1);      
```
## Settings
### Geometry
In the `settings/geometryDict` file, the geometry and axis of rotation is defined for the drum. The geometry is composed of a body, front and rear ends. 

```C++
surfaces
{
	body
	{
		// type of the wall
		type 	 stlWall;
		// file name in stl folder  	
		file 	 Body.stl;
		// material name of this wall		
		material wallMat;
		// motion component name   
		motion 	 rotAxis;		 
	}
	/*	This is a Cylinder Wall at the rear of cylinder	*/
	rearEnd
	{
		// type of the wall
		type cylinderWall;
		// first point for the axis of rotation			
		p1 (-0.1974  0.2269  -0.001);
		// second point for the axis of rotation	 
		p2 (-0.1974  0.2269   0.0);
		// Radius of p1	
		radius1 0.0001;
		// Radius of p2
		radius2 0.12;
		// material name of the wall
		material wallMat;
		// motion component name          
		motion rotAxis;			 
	}
	/*	This a cylinder Wall at the front of Cylinder */
	frontEnd
	{
		// type of the wall
		type cylinderWall;
		// first point for the axis of rotation			
		p1 (-0.1974  0.2269   0.0989);
		// second point for the axis of rotation	 
		p2 (-0.1974  0.2269   0.0990);	
		// Radius of p1
		radius1 0.0001;
		// Radius of p2
		radius2 0.12;
		// material name of the wall
		material wallMat; 
		// motion component name          
		motion rotAxis;			 
	}
}
```
### Rotating Axis Info
In this part of `geometryDict` the information of rotating axis and speed of rotation are defined. The start of rotation is at 2 s. The first 2 seconds of simulation is for allowing particles to settle donw in the drum. 

```C++
motionModel rotatingAxis;   
rotatingAxisInfo
{
    rotAxis 
    {
        // first point for the axis of rotation
	p1 (-0.1974  0.2269  0);
	// second point for the axis of rotation	
	p2 (-0.1974  0.2269  0.1);
	// rotation speed (rad/s) => 15 rpm	
	omega 2.38733;
	// Start time of Geometry Rotating 	
	startTime 2;
	// End time of Geometry Rotating
	endTime 9.5;
    }
}
```
## Performing Simulation
To run simulations, type the following commands in the terminal one at a time.

Enter `particlesPhasicFlow` command to create the initial fields for particles.  
Enter `geometryPhasicFlow` command to create the Geometry.  
At last, enter `sphereGranFlow` command to start the simulation.  
After finishing the simulation, you can use  `pFlowtoVTK` to convert the results into vtk format stored in ./VTK folder. 