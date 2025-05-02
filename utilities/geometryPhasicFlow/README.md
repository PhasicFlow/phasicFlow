# geometryPhasicFlow Utility

## Overview

`geometryPhasicFlow` is a preprocessing utility for Discrete Element Method (DEM) simulations in phasicFlow. It converts wall geometry definitions from the `geometryDict` file into the internal geometry data structures used by the phasicFlow simulation engine.

This utility reads geometry definitions including wall types, material properties, and motion models from the `geometryDict` file located in the `settings` folder of your simulation case directory. It then processes these definitions to create the necessary triangulated surfaces and motion models that will be used during the simulation.

## Usage

Run the utility from your case directory containing the `settings` folder:

```bash
geometryPhasicFlow
```

For fluid-particle coupling simulations:

```bash
geometryPhasicFlow -c
```

## Wall Types

phasicFlow supports several built-in wall types that can be defined in the `geometryDict`:

1. **planeWall** - Flat wall defined by four points (p1, p2, p3, p4)
2. **cylinderWall** - Cylindrical wall defined by two axis points and radius
3. **cuboidWall** - Box-shaped wall defined by center point and dimensions
4. **stlWall** - Complex geometry imported from an STL file

## Motion Models

Walls can be associated with different motion models:

1. **stationary** - Fixed walls (no movement)
2. **rotatingAxis** - Rotation around a specified axis
3. **multiRotatingAxis** - Multiple rotations (for complex motions)
4. **vibrating** - Oscillating motion with specified frequency and amplitude
5. **conveyorBelt** - Creates a conveyor belt effect with constant tangential velocity

## geometryDict File Structure

The geometryDict file requires the following structure:

```C++

// Motion model selection
motionModel <motionModelName>;

// Motion model specific information
<motionModelName>Info
{
    // Motion model parameters
    // ...
}

// Wall surfaces definitions
surfaces
{
    <wallName1>
    {
        type        <wallType>;      // Wall type (planeWall, cylinderWall, etc.)
        // Wall type specific parameters
        // ...
        material    <materialName>;  // Material name for this wall
        motion      <motionName>;    // Motion component name
    }
    
    <wallName2>
    {
        // Another wall definition
        // ...
    }
    
    // Additional walls as needed
}
```

## Example

Here's a simple example of a `geometryDict` file for a rotating drum:

```C++

// Rotation around an axis
motionModel rotatingAxis;

rotatingAxisInfo
{
    rotAxis 
    {
        p1        (0.0 0.0 0.0);    // First point for axis of rotation
        p2        (0.0 0.0 1.0);    // Second point for axis of rotation
        omega     1.214;            // Rotation speed (rad/s)
    }
}

surfaces
{
    cylinder
    {
        type        cylinderWall;    // Type of wall
        p1          (0.0 0.0 0.0);   // Begin point of cylinder axis
        p2          (0.0 0.0 0.1);   // End point of cylinder axis
        radius1     0.12;            // Radius at p1
        radius2     0.12;            // Radius at p2
        resolution  24;              // Number of divisions
        material    prop1;           // Material name
        motion      rotAxis;         // Motion component name
    }
    
    wall1
    {
        type        planeWall;       // Type of wall
        p1          (-0.12 -0.12 0.0); // First point
        p2          ( 0.12 -0.12 0.0); // Second point
        p3          ( 0.12  0.12 0.0); // Third point
        p4          (-0.12  0.12 0.0); // Fourth point
        material    prop1;           // Material name
        motion      rotAxis;         // Motion component name
    }
}
```

## STL File Support

For complex geometries, you can use STL files:

```C++
wallName
{
    type        stlWall;         // Type is STL wall
    file        filename.stl;    // File name in ./stl folder
    scale       1.0;             // Optional scale for changing the size of surface
    transform   (0 0 0);         // Optional translation vector
    scaleFirst  Yes;             // Scale first or translate first
    material    wallMat;         // Material name
    motion      rotAxis;         // Motion component name
}
```

STL files should be placed in an `stl` folder in your case directory.

## See Also

- [particlesPhasicFlow](../particlesPhasicFlow) - Utility for creating initial particle configurations
- [pFlowToVTK](../pFlowToVTK) - Utility for converting simulation results to VTK format
- [Tutorials](../../tutorials) - Example cases demonstrating phasicFlow capabilities