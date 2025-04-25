# Simulating a Rotating Drum (v-1.0)

## Problem Definition

The problem is to simulate a rotating drum with a diameter of 0.24 m and a length of 0.1 m, rotating at 11.6 rpm. It is filled with 30,000 spherical particles, each with a diameter of 4 mm. The timestep for integration is 0.00001 s. This tutorial demonstrates the basic setup for creating a rotation-based simulation using built-in geometry in PhasicFlow.

<div align="center">
<b>
A view of the rotating drum
</b>
<div>
<img src="https://github.com/PhasicFlow/phasicFlow/blob/media/media/rotating-drum-s.png" width="600px">
</div>
</div>

***

## Setting up the Case 

PhasicFlow simulation case setup is based on text-based scripts provided in two folders located in the simulation case folder: `settings` and `caseSetup`. All commands should be entered in the terminal while the current working directory is the simulation case folder (at the top level of `caseSetup` and `settings`).

### Creating Particles

In the file `settings/particlesDict`, two dictionaries, `positionParticles` and `setFields`, position particles and set the field values for the particles.

The `positionParticles` dictionary uses the `ordered` method to position particles in a space defined by `box`. The box space is defined by two corner points: `min` and `max`. In the `orderedInfo` sub-dictionary, `numPoints` defines the number of particles (30,000), `distance` defines the spacing between adjacent particles (4 mm), and `axisOrder` defines the axis order for filling the space with particles.

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
positionParticles           
{ 
    method         ordered; // other options: random and empty

    mortonSorting  Yes;     // perform initial sorting based on morton code?   

    orderedInfo
    {
        distance  0.004;    // minimum space between centers of particles

        numPoints 30000;    // number of particles in the simulation 

        axisOrder (z y x);  // axis order for filling the space with particles
    }

    regionType    box;      // other options: cylinder and sphere  

    boxInfo                 // box information for positioning particles 
    {
        min (-0.08 -0.08 0.015);  // lower corner point of the box 

        max ( 0.08  0.08 0.098);   // upper corner point of the box 
    }
}
```

In the `setFields` dictionary, the `defaultValue` sub-dictionary defines the initial values for particle fields (velocity, acceleration, rotational velocity, and shape name). The shape name field should be consistent with the name defined in the shapes file (here, "sphere1").

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
setFields
{
    defaultValue 
    {
        velocity        realx3  (0 0 0);         // linear velocity (m/s)

        acceleration    realx3  (0 0 0);         // linear acceleration (m/s2)

        rVelocity       realx3  (0 0 0);         // rotational velocity (rad/s)

        shapeName       word    sphere1;         // name of the particle shape 
    }

    selectors
    {
        // Selectors can be used to modify properties for specific particle groups
    }
}
```

To create the particles and store them in the `0` folder, enter the following command:

```
particlesPhasicFlow
```

### Creating Geometry

In the file `settings/geometryDict`, you define the motion model and geometry for the simulation. The `rotatingAxis` motion model defines a fixed axis which rotates around itself. The `rotAxis` dictionary specifies the axis endpoints and rotation speed.

<div align="center"> 
in <b>settings/geometryDict</b> file
</div>

```C++
motionModel rotatingAxis; 

rotatingAxisInfo 
{
    rotAxis 
    {
        p1 (0.0 0.0 0.0);     // first point for the axis of rotation 

        p2 (0.0 0.0 1.0);     // second point for the axis of rotation

        omega      1.214;     // rotation speed (rad/s)
    }
}
```

The `surfaces` dictionary defines all the walls in the simulation. This tutorial uses built-in geometries provided by PhasicFlow. The geometry consists of:

1. A `cylinder` dictionary defining a cylindrical shell with end radii (`radius1` and `radius2`), axis endpoints (`p1` and `p2`), material name (`prop1`), and motion component (`rotAxis`).
2. Two plane walls (`wall1` and `wall2`) at the ends of the cylindrical shell, each defined with four coplanar corner points, the same material name, and the same motion component.

<div align="center"> 
in <b>settings/geometryDict</b> file
</div>

```C++
surfaces
{
    /*
        A cylinder with begin and end radii 0.12 m and axis points at (0 0 0) and (0 0 0.1)
    */
    cylinder
    {
        type cylinderWall;      // type of the wall

        p1  (0.0 0.0 0.0);     // begin point of cylinder axis

        p2  (0.0 0.0 0.1);     // end point of cylinder axis

        radius1      0.12;     // radius at p1

        radius2      0.12;     // radius at p2

        resolution     24;      // number of divisions

        material    prop1;      // material name of this wall

        motion    rotAxis;      // motion component name 
    }

    /*
        This is a plane wall at the rear end of cylinder
    */
    wall1
    {
        type       planeWall;      // type of the wall

        p1 (-0.12 -0.12 0.0);      // first point of the wall

        p2 ( 0.12 -0.12 0.0);      // second point

        p3 ( 0.12  0.12 0.0);      // third point

        p4 (-0.12  0.12 0.0);      // fourth point 

        material       prop1;      // material name of the wall

        motion       rotAxis;      // motion component name 
    }

    /*
        This is a plane wall at the front end of cylinder
    */
    wall2
    {
        type       planeWall;      // type of the wall

        p1 (-0.12 -0.12 0.1);      // first point of the wall

        p2 ( 0.12 -0.12 0.1);      // second point

        p3 ( 0.12  0.12 0.1);      // third point

        p4 (-0.12  0.12 0.1);      // fourth point 

        material       prop1;      // material name of the wall

        motion       rotAxis;      // motion component name 
    }
}
```

To create the geometry and store it in the `0/geometry` folder, enter:

```
geometryPhasicFlow
```

### Defining Properties and Interactions 

In the file `caseSetup/interaction`, you define properties of materials and their interactions. The `materials` entry lists material names, and `densities` sets the corresponding densities. The `model` dictionary defines the contact force and rolling friction models, along with other required properties.

<div align="center"> 
in <b>caseSetup/interaction</b> file
</div>

```C++
materials                   (prop1);   // a list of materials names

densities                  (1000.0);   // density of materials [kg/m3]

contactListType   sortedContactList; 

model
{
    contactForceModel    nonLinearNonLimited;

    rollingFrictionModel              normal;

    Yeff  (1.0e6);     // Young modulus [Pa]

    Geff  (0.8e6);     // Shear modulus [Pa]

    nu    (0.25);      // Poisson's ratio [-]

    en    (0.7);       // coefficient of normal restitution

    mu    (0.3);       // dynamic friction 

    mur   (0.1);       // rolling friction      
}
```

The `contactSearch` dictionary specifies the algorithm and parameters for finding particle-particle contacts. The `method` determines the broad search algorithm, `updateInterval` sets how often to update the neighbor list, and `sizeRatio` controls the enlarged cell size for finding neighbors.

<div align="center"> 
in <b>caseSetup/interaction</b> file
</div>

```C++
contactSearch
{
    method          NBS;  

    updateInterval   10;

    sizeRatio       1.1;

    cellExtent     0.55;

    adjustableBox   Yes;
} 
```

In the file `caseSetup/shapes`, you define particle shapes, including their names, diameters, and material properties:

<div align="center"> 
in <b>caseSetup/shapes</b> file
</div>

```C++
names       (sphere1);    // names of shapes 

diameters     (0.004);    // diameter of shapes 

materials     (prop1);    // material names for shapes
```

### Simulation Domain and Boundaries

The file `settings/domainDict` defines a rectangular bounding box with boundaries. Particles that exit this box are automatically deleted.

<div align="center"> 
in <b>settings/domainDict</b> file
</div>

```C++
// Simulation domain: every particles that goes outside this domain will be deleted
globalBox                                        
{
    min (-0.12 -0.12 0.00);   // lower corner point of the box 

    max (0.12   0.12 0.11);   // upper corner point of the box 
}

boundaries
{
    left
    {
        type     exit;      // other options: periodic, reflective 
    }

    right
    {
        type     exit;      // other options: periodic, reflective 
    }

    bottom
    {
        type     exit;      // other options: periodic, reflective 
    }

    top
    {
        type     exit;      // other options: periodic, reflective 
    }

    rear
    {
        type     exit;      // other options: periodic, reflective 
    }

    front
    {
        type     exit;      // other options: periodic, reflective 
    }
}
```

### Other Settings

Additional parameters for the simulation are set in `settings/settingsDict`, including timestep, start and end times, saving intervals, and gravity:

<div align="center"> 
in <b>settings/settingsDict</b> file
</div>

```C++
dt              0.00001;     // time step for integration (s)

startTime             0;     // start time for simulation 

endTime              10;     // end time for simulation 

saveInterval        0.1;     // time interval for saving the simulation

timePrecision         6;     // maximum number of digits for time folder 

g            (0 -9.8 0);     // gravity vector (m/s2) 

includeObjects (diameter);   // save necessary (i.e., required) data on disk

// exclude unnecessary data from saving on disk
excludeObjects ();

integrationMethod       AdamsBashforth2;  // integration method

integrationHistory                  off;  // to save space on disk

writeFormat                       ascii;  // data writing format (ascii or binary)

timersReport                        Yes;  // report timers (Yes or No)
```

## Running the Case 

To execute the simulation, follow these steps in order:

1. Create the geometry:

```
geometryPhasicFlow
```

2. Create the initial particle fields:

```
particlesPhasicFlow
```

3. Run the simulation:

```
sphereGranFlow
```

Depending on your computational resources, the simulation may take from a few minutes to several hours to complete.

## Post Processing 

After the simulation completes, you can visualize the results in ParaView by converting them to VTK format:

```
pFlowToVTK --binary
```

This command converts all simulation results (particles and geometry) to VTK format and stores them in a `VTK/` folder. You can then open these files in ParaView for detailed analysis and visualization.

For more specific field output, you can specify fields:

```
pFlowToVTK --binary --fields diameter velocity id
```
