# Simulating a Medium-Scale Rotating Drum (v-1.0)

## Problem Definition

This tutorial demonstrates the simulation of a medium-sized rotating drum with a diameter of 0.24 m and a length of 0.36 m. The drum is filled with 250,000 spherical glass beads with a diameter of 3 mm. The drum rotates at a constant speed, and the simulation captures the flow behavior and mixing of the particles.

<div align="center">
<b>
A view of the rotating drum simulation
</b>
</div>

***

## Setting up the Case

PhasicFlow simulation case setup is based on text-based scripts provided in two folders located in the simulation case folder: `settings` and `caseSetup`. All commands should be entered in the terminal while the current working directory is the simulation case folder.

### Creating Particles

In the file `settings/particlesDict`, two dictionaries, `positionParticles` and `setFields`, define how particles are positioned and what field values they have initially.

The `positionParticles` dictionary specifies the ordered positioning method to place 250,000 particles within a cylindrical region:

```C++
positionParticles
{
    method ordered;                // other options: random and empty
    
    orderedInfo
    {
        distance   0.003;          // minimum distance between particles centers
        numPoints  250000;         // number of particles in the simulation 
        axisOrder  (z y x);        // axis order for filling the space with particles
    }

    regionType cylinder;           // other options: box and sphere  
    
    cylinderInfo
    {
        p1 (0.0 0.0 0.003);        // begin point of cylinder axis
        p2 (0.0 0.0 0.357);        // end point of cylinder axis
        radius     0.117;          // radius of cylinder 
    }
}
```

The `setFields` dictionary defines the initial values for particle fields:

```C++
setFields
{
    defaultValue 
    {
        velocity        realx3  (0 0 0);   // linear velocity (m/s)
        acceleration    realx3  (0 0 0);   // linear acceleration (m/s2)
        rVelocity       realx3  (0 0 0);   // rotational velocity (rad/s)
        shapeName       word    glassBead; // name of the particle shape 
    }
}
```

To create the particles based on these settings, enter the following command in the terminal:

```
> particlesPhasicFlow
```

### Creating Geometry

In the file `settings/geometryDict`, you can find information for creating the rotating drum geometry. The simulation uses the `rotatingAxis` motion model to define rotation around a fixed axis.

The surfaces of the drum are defined in the `surfaces` dictionary, including the cylindrical shell and end walls.

To create the geometry based on these settings, enter the following command in the terminal:

```
> geometryPhasicFlow
```

### Defining Properties and Interactions

In the file `caseSetup/shapes`, the particle shape, diameter, and material are defined:

```C++
names       (glassBead);    // names of shapes 
diameters       (0.003);    // diameter of shapes 
materials    (glassMat);    // material names for shapes
```

In the file `caseSetup/interaction`, the material properties and interaction models are defined:

```C++
materials     (glassMat wallMat);  // a list of materials names
densities     (2500.0 2500);       // density of materials [kg/m3]

model
{
    contactForceModel    nonLinearLimited;
    rollingFrictionModel           normal;
    
    /*
       Property (glassMat-glassMat      glassMat-wallMat
                                        wallMat-wallMat);
    */

    Yeff (1.0e6  1.0e6  
                 1.0e6);          // Young modulus [Pa]
   
    Geff (0.8e6  0.8e6  
                 0.8e6);          // Shear modulus [Pa]

    nu   (0.25   0.25  
                 0.25);           // Poisson's ratio [-]

    en   (0.97   0.85   
                 1.00);           // coefficient of normal restitution

    mu   (0.65   0.65   
                 0.65);           // dynamic friction 

    mur  (0.1    0.1    
                 0.1);            // rolling friction
}
```

The contact search settings are also defined in this file, including the method, update interval, and other parameters.

## Running the Simulation

To run the simulation, follow these steps in order:

1. Create the initial particle fields:

   ```
   > particlesPhasicFlow
   ```

2. Create the geometry:

   ```
   > geometryPhasicFlow
   ```

3. Start the simulation:

   ```
   > sphereGranFlow
   ```

The simulation will run according to the settings defined in `settings/settingsDict`, including the time step, start/end times, and gravity vector.

## Post-Processing

After the simulation is complete, you can visualize the results using ParaView. To convert the simulation results to VTK format, use the following command:

```
> pFlowToVTK --binary
```

This will create VTK files in the `VTK/` folder that can be opened in ParaView for visualization and analysis.
