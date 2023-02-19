# Problem Definition 
The problem is to simulate a Rotating Drum with **6** Baffles with the diameter **0.24m** and the length **0.1m** rotating at **15 rad/s**. This Rotating Drum is filled with **20000** Particles.The timestep for integration is **0.00001 s**. There are 2 types of Particles in this Rotating Drum:
* **12500** Particles with **4 mm** diameter
* **7500** Particles with **5mm** diameter  

<html>
<body>
<img src="https://github.com/PhasicFlow/phasicFlow/blob/media/media/MixedparticlesRDB.png", width: 400px>

</body>
</html>

## Setting up the Case
As it has been explained in the previous Cases, these Tutorials are based on text-based scripts. There are three parts in this case to study `caseSetup`, `setting` and `stl`.
## Particle Insertion
In this case we have two region for inserting our particles. In the both region we define rate of Insertion, start and end time of Insertion, coordinates of Insertion and radius of Insertion.  
An example for the Right Layer Region of insertion of Particles is shown below.  
```C++
// Right Layer Region
layerrightregion 
{
// type of insertion region
   type	          cylinderRegion;
// insertion rate (particles/s)    
   rate 	  12500;
// Start time of LightParticles insertion (s)
   startTime 	  0; 
// End time of LightParticles insertion (s)      
   endTime   	  1;
// Time Interval of LightParticles insertion (s)
   interval       0.025; 

   cylinderRegionInfo 
   {
// Coordinates of cylinderRegion (m,m,m)
   	p2 (-0.15 0.25 0.05);
   	p1 (-0.15 0.24	0.05);
// radius of cylinder (m)
   	radius 0.035;
   }
}
```
Then in the `sphereShape` the diameter and the material of our Particles are defined.  
```C++
// names of shapes 
names 		(lightSphere heavySphere);
// diameter of shapes (m) 	
diameters 	(0.004 0.005);
// material names for shapes 			
materials	(lightMat heavyMat);
```
In this Case we have two types of Particle with 4mm and 5mm diameters.  
At the end of `caseSetup`, the interaction between the particles and the Shell of Rotating Drum is defined. You can see the Coefficients of the Interactions between the particles and shell of Rotating Drum in `interaction`. Because we have 3 kind of interactions between these Particles and the Drum, we need to define a 3*3 Matrix.
```C++
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
// coefficient of tangential restitution
   et (1.0   1.0  1.0    
             1.0  1.0
                  1.0);
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
In the Settings folder the Specifications of our Rotating Drum and the information of rotating axis are brought. In this case we use two solid cylinders to keep our rotating drum isolated. This is to prevent particles, from being thrown out.  
For example the codes for the rear cylinder is brought below.  
```C++
/*This is a Cylinder Wall at the rear of cylinder	*/
CylinderRear1
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
```
### Rotating Axis Info
In this part of `geometryDict` the information of `rotating axis` and `velocity` of this Rotating Drum is defined. Also in purpose to settle down Particles after they were inserted we use a `startTime` and `endTime` function. This shows the start time of rotation.  
```C++
rotatingAxisMotionInfo
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
## Starting Simulation
To start Simulation we have to create our Particles at first.  
Enter `>particlesPhasicFlow` command to create the initial fields for particles.  
Enter `>geometryPhasicFlow` command to create the Geometry.  
At last, enter `>sphereGranFlow` command to start the simulation.  
After finishing the simulation, you can use  `>pFlowtoVTK` to convert the results into vtk format storred in ./VTK folder. 
