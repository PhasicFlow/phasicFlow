# Simulating a small rotating drum {#rotatingDrumSmall}
## Problem definition
The problem is to simulate a rotating drum with a diameter of 0.24 m and a length of 0.1 m, rotating at 11.6 rpm. It is filled with 30,000 spherical particles with a diameter of 4 mm.  The time step for integration is 0.00001 s.
<div align="center"><b>
a view of rotating drum

![](https://github.com/PhasicFlow/phasicFlow/blob/media/media/rotating-drum-s.png)
</b></div>

***

## Setting up the case 
The PhasicFlow simulation case setup is based on the text-based scripts that we provide in two folders located in the simulation case folder: `settings` and `caseSetup` (You can find the case setup files in the above mentioned folders.
All commands should be entered in the terminal with the current working directory being the simulation case folder (at the top of the `caseSetup` and `settings` folders).
 

### Creating particles

Open the file  `settings/particlesDict`. Two dictionaries, `positionParticles` and `setFields` position particles and set the field values for the particles. 
In dictionary `setFields`, dictionary `defaultValue` defines the initial value for particle fields (here, `velocity`, `acceleration`, `rotVelocity`, and `shapeName`). Note that `shapeName` field should be consistent with the name of shape that you later set for shapes (here one shape with name `sphere1`).

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
setFields
{
    defaultValue 
    {
        velocity    realx3  (0 0 0); // linear velocity (m/s)
        acceleration    realx3  (0 0 0); // linear acceleration (m/s2)
        rotVelocity     realx3  (0 0 0); // rotational velocity (rad/s)
        shapeName   word    sphere1; // name of the particle shape 
    }
    selectors
    {
    shapeAssigne
        {
            selector    stridedRange;            // other options: box, cylinder, sphere, randomPoints 
 
            stridedRangeInfo
            {
                begin       0;                   // begin index of points

                end     30000;                   // end index of points 

                stride      3;                   // stride for selector 
            }

            fieldValue                           // fields that the selector is applied to 
            {
                shapeName   word    sphere1;     // sets shapeName of the selected points to largeSphere
            }
        }
        }
}
In dictionary `positionParticles`, the positioning `method` is `ordered`, which position particles in order in the space defined by `box`. `box` space is defined by two corner points `min` and `max`. In dictionary `orderedInfo`, `numPoints` defines number of particles; `diameter`, the distance between two adjacent particles, and `axisOrder` defines the axis order for filling the space by particles. 

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
positionParticles
{
    method ordered;             // other options: random and empty
    mortonSorting Yes;          // perform initial sorting based on morton code

  orderedInfo
    {
        diameter 0.004;        // minimum space between centers of particles
        numPoints 30000;       // number of particles in the simulation 
        axisOrder (z y x);     // axis order for filling the space with particles
    }   

    regionType box;                              // other options: cylinder and sphere  

    boxInfo                                      // box information for positioning particles 
    {
        min (-0.08 -0.08 0.015);                 // lower corner point of the box 

        max ( 0.08  0.08 0.098);                 // upper corner point of the box 

  
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
        velocity        realx3  (0 0 0); // linear velocity (m/s)
        acceleration    realx3  (0 0 0); // linear acceleration (m/s2)
        rotVelocity     realx3  (0 0 0); // rotational velocity (rad/s)
        shapeName     word      sphere1; // name of the particle shape 
    }
    selectors
    {}
}
```

Enter the following command in the terminal to create the particles and store them in `0` folder.

`> particlesPhasicFlow`
 
### Creating geometry
In file `settings/geometryDict` , you can provide information for creating geometry. Each simulation should have a `motionModel` that defines a model for moving the surfaces in the simulation. `rotatingAxis` model defines a fixed axis which rotates around itself. The dictionary `rotAxis` defines an motion component with `p1` and `p2` as the end points of the axis and `omega` as the rotation speed in rad/s. You can define more than one motion component in a simulation. 

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
        p1 (0.0 0.0 0.0);   // first point for the axis of rotation 
        p2 (0.0 0.0 1.0);   // second point for the axis of rotation
        omega 1.214;        // rotation speed (rad/s)
    }
}
```
In the dictionary `surfaces` you can define all the surfaces (walls) in the simulation. Two main options are available: built-in geometries in PhasicFlow, and providing surfaces with stl file. Here we use built-in geometries. In `cylinder` dictionary, a cylindrical shell with end radii, `radius1` and `radius2`, axis end points `p1` and `p2`, `material` name `prop1`, `motion` component `rotAxis` is defined. `resolution` sets number of division for the cylinder shell. `wall1` and `wall2` define two plane walls at two ends of cylindrical shell with coplanar corner points `p1`, `p2`, `p3`, and `p4`, `material` name `prop1` and `motion` component `rotAxis`.  

<div align="center"> 
in <b>settings/geometryDict</b> file
</div>

```C++
surfaces
{
    cylinder
    {
        type cylinderWall;  // type of the wall
        p1 (0.0 0.0 0.0);   // begin point of cylinder axis
        p2 (0.0 0.0 0.1);   // end point of cylinder axis
        radius1 0.12;       // radius at p1
        radius2 0.12;       // radius at p2
        resolution 24;      // number of divisions
        material prop1;     // material name of this wall
        motion rotAxis;     // motion component name 
    }
    wall1
    {
        type planeWall;         // type of the wall
        p1 (-0.12 -0.12 0.0);   // first point of the wall
        p2 ( 0.12 -0.12 0.0);   // second point
        p3 ( 0.12  0.12 0.0);   // third point
        p4 (-0.12  0.12 0.0);   // fourth point 
        material prop1;         // material name of the wall  
        motion rotAxis;         // motion component name 
    }
    wall2
    {
        type planeWall;
        p1 (-0.12 -0.12 0.1);
        p2 ( 0.12 -0.12 0.1);
        p3 ( 0.12  0.12 0.1);
        p4 (-0.12  0.12 0.1);
        material prop1;
        motion rotAxis;
    }
}
```
Enter the following command in the terminal to create the geometry and store it in `0/geometry` folder.

`> geometryPhasicFlow`

### Defining properties and interactions 
In the file `caseSetup/interaction` , you find properties of materials. `materials` defines a list of material names in the simulation and `densities` sets the corresponding density of each material name. model dictionary defines the interaction model for particle-particle and particle-wall interactions. `contactForceModel` selects the model for mechanical contacts (here nonlinear model with limited tangential displacement) and `rollingFrictionModel` selects the model for calculating rolling friction. Other required Properties should be defined in this dictionary. 

<div align="center"> 
in <b>caseSetup/interaction</b> file
</div>

```C++
materials      (prop1);    // a list of materials names
densities      (1000.0);   // density of materials [kg/m3]
.
.
.
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

Dictionary `contactSearch` sets the methods for particle-particle and particle-wall contact search. Larger `sizeRatio` include more particles in the neighbor list and you require to update it less frequent. 

<div align="center"> 
in <b>caseSetup/interaction</b> file
</div>

```C++
contactSearch
{
  
   method          NBS;                          // method for broad search 
    
   updateInterval   10;

   sizeRatio       1.1;

   cellExtent     0.55;

   adjustableBox   Yes;
} 
```

In the file `caseSetup/shapes`, you can define a list of `names` for shapes (`shapeName` in particle field), a list of diameters for shapes and their `properties` names. 

<div align="center"> 
in <b>caseSetup/shapes</b> file
</div>

```C++
names     (sphere1);    // names of shapes 
diameters   (0.004);    // diameter of shapes 
materials   (prop1);    // material names for shapes 
```

Other settings for the simulation can be set in file `settings/settingsDict`.

<div align="center"> 
in <b>settings/settingsDict</b> file
</div>

```C++
dt                 0.00001;    // time step for integration (s)
startTime                0;          // start time for simulation 
endTime                 10;         // end time for simulation 
saveInterval           0.1;        // time interval for saving the simulation
timePrecision            6;          // maximum number of digits for time folder 
g               (0 -9.8 0);          // gravity vector (m/s2) 

includeObjects (diameter);                       // save necessary (i.e., required) data on disk
// exclude unnecessary data from saving on disk
excludeObjects (rVelocity.dy1 pStructPosition.dy1 pStructVelocity.dy1); 
integrationMethod         AdamsBashforth2;     // integration method 
writeFormat                       ascii;         // data writting format (ascii or binary)
timersReport                        Yes;         // report timers (Yes or No)
timersReportInterval               0.01;         // time interval for reporting timers
```

## Running the case 
The solver for this simulation is `sphereGranFlow`. Type the following command in a terminal. Depending on your computer's computation power, it may take from a few minutes to a few hours to complete. 

`> sphereGranFlow`

## Post processing 
When the simulation is finished, you can render the results in Paraview. To convert the results to VTK format, just type the following command in a terminal. This will convert all results (particles and geometry) to VTK format and save them in the folder `VTK/`. 

`> pFlowToVTK`