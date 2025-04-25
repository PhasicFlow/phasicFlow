# particlesPhasicFlow Utility

## 1. Overview

`particlesPhasicFlow` is a pre-processing utility for Discrete Element Method (DEM) simulations in phasicFlow. It is used to:

1. Position particles in the simulation domain using different methods
2. Set initial field values (such as velocity, acceleration, shape) for particles
3. Apply field values to selected subsets of particles

The utility reads its configuration from `settings/particlesDict` in your simulation case directory.

## Table of Contents

- [1. Overview](#1-overview)
- [2. Command Line Options](#2-command-line-options)
- [3. The particlesDict File Structure](#3-the-particlesdict-file-structure)
  - [3.1. positionParticles Sub-dictionary](#31-positionparticles-sub-dictionary)
    - [3.1.1. Positioning Methods](#311-positioning-methods)
    - [3.1.2. Region Types](#312-region-types)
  - [3.2. setFields Dictionary](#32-setfields-dictionary)
    - [3.2.1. Selector Types](#321-selector-types)
- [4. Usage Examples](#4-usage-examples)
  - [4.1. Example 1: Positioning Particles in an Ordered Pattern](#41-example-1-positioning-particles-in-an-ordered-pattern)
  - [4.2. Example 2: Setting Field Values for a Binary Mixture of Particles](#42-example-2-setting-field-values-for-a-binary-mixture-of-particles)
  - [4.3. Example 3: Empty Initial Particle System (for Insertion Later)](#43-example-3-empty-initial-particle-system-for-insertion-later)
- [5. Workflow Tips](#5-workflow-tips)
- [6. Common Field Types](#6-common-field-types)
- [7. Shape Handling](#7-shape-handling)
- [8. See Also](#8-see-also)

## 2. Command Line Options

The utility can be run with the following command line options:

```
particlesPhasicFlow [OPTIONS]
```

Where `[OPTIONS]` can include:

- `--positionParticles-only`: Execute only the positioning part and store the created pointStructure.
- `--setFields-only`: Execute only the field setting part, reading an existing pointStructure file from the time folder.
- `-c, --coupling`: Specify if this is a fluid-particle coupling simulation.

## 3. The particlesDict File Structure

The `particlesDict` file consists of two main sections:

1. `positionParticles`: Defines how particles are positioned in the domain.
2. `setFields`: Defines initial field values for particles and selective field assignments.

### 3.1. positionParticles Sub-dictionary

This section defines how particles are positioned in the simulation domain:

```C++
positionParticles
{
    method        <method>;        // Options: ordered, random, file, empty
    
    regionType    <region>;        // Options: box, cylinder, sphere, domain
    
    // Method-specific settings
    <method>Info
    {
        // Parameters for the selected method
    }
    
    // Region-specific settings
    <region>Info
    {
        // Parameters for the selected region
    }
}
```

#### 3.1.1. Positioning Methods

1. **ordered**: Positions particles in an ordered pattern along specified axes

   ```C++
   orderedInfo
   {
       distance    <value>;        // Minimum space between centers of particles
       numPoints   <value>;        // Number of particles to position
       axisOrder   (<axis1> <axis2> <axis3>);  // Order of axes for filling (x, y, z)
   }
   ```

2. **random**: Positions particles randomly in the specified region

   ```C++
   randomInfo
   {
       distance       <value>;     // Minimum distance between particle centers
       numPoints      <value>;     // Number of particles to position
       maxIterations  <value>;     // Maximum iterations for random positioning (default: 10)
   }
   ```

3. **file**: Reads particle positions from a file

   ```C++
   fileInfo
   {
       fileName       <path>;      // Path to file containing positions
       commaSeparated Yes/No;      // Whether file is comma-separated
   }
   ```

4. **empty**: Creates an empty pointStructure with no particles. No additional sub-dictionary is required.

#### 3.1.2. Region Types

1. **box**: Rectangular region

   ```C++
   boxInfo
   {
       min (<x> <y> <z>);          // Lower corner point coordinates
       max (<x> <y> <z>);          // Upper corner point coordinates
   }
   ```

2. **cylinder**: Cylindrical region

   ```C++
   cylinderInfo
   {
       p1     (<x> <y> <z>);       // Begin point of cylinder axis
       p2     (<x> <y> <z>);       // End point of cylinder axis
       radius <value>;             // Radius of cylinder
   }
   ```

3. **sphere**: Spherical region

   ```C++
   sphereInfo
   {
       center (<x> <y> <z>);       // Center of sphere
       radius <value>;             // Radius of sphere
   }
   ```

4. **domain**: Uses the global domain defined in the simulation (in file `settings/domainDict`)

   ```C++
   // No additional information needed when using domain
   ```

### 3.2. setFields Dictionary

This section defines the initial field values for particles and allows selective field assignments:

```C++
setFields
{
    defaultValue
    {
        // Default field values for all particles
        velocity      realx3 (<x> <y> <z>);    // Initial linear velocity (m/s)
        acceleration  realx3 (<x> <y> <z>);    // Initial linear acceleration (m/s²)
        rVelocity     realx3 (<x> <y> <z>);    // Initial rotational velocity (rad/s)
        shapeName     word   <shape>;          // Particle shape name
        // Additional fields as required
    }
    
    selectors
    {
        <selectorName>
        {
            selector <selectorType>;           // Type of selector
            
            <selectorType>Info
            {
                // Parameters specific to the selector type
            }
            
            fieldValue
            {
                // Field values to set for selected particles
                <fieldName> <fieldType> <value>;
                // Additional fields as required
            }
        }
        // Additional selectors as required
    }
}
```

#### 3.2.1. Selector Types

1. **stridedRange**: Selects particles based on index ranges

   ```C++
   stridedRangeInfo
   {
       begin  <value>;             // Beginning index
       end    <value>;             // Ending index
       stride <value>;             // Step size (default: 1)
   }
   ```

2. **box**: Selects particles within a box region

   ```C++
   boxInfo
   {
       min (<x> <y> <z>);          // Lower corner point coordinates
       max (<x> <y> <z>);          // Upper corner point coordinates
   }
   ```

3. **cylinder**: Selects particles within a cylindrical region

   ```C++
   cylinderInfo
   {
       p1     (<x> <y> <z>);       // Begin point of cylinder axis
       p2     (<x> <y> <z>);       // End point of cylinder axis
       radius <value>;             // Radius of cylinder
   }
   ```

4. **sphere**: Selects particles within a spherical region

   ```C++
   sphereInfo
   {
       center (<x> <y> <z>);       // Center of sphere
       radius <value>;             // Radius of sphere
   }
   ```

5. **randomPoints**: Selects a random subset of particles

   ```C++
   randomPointsInfo
   {
       begin   <value>;      // Beginning index
       end     <value>;      // Ending index
       number  <value>;      // Number of random points to select
   }
   ```

## 4. Usage Examples

### 4.1. Example 1: Positioning Particles in an Ordered Pattern

```C++
positionParticles
{
    method        ordered;
    mortonSorting Yes;
    
    orderedInfo
    {
        distance    0.005;
        numPoints   30000;
        axisOrder   (z x y);
    }
    
    regionType cylinder;
    
    cylinderInfo
    {
        p1     (0.0 0.0 0.003);
        p2     (0.0 0.0 0.097);
        radius 0.117;
    }
}
```

### 4.2. Example 2: Setting Field Values for a Binary Mixture of Particles

```C++
setFields
{
    defaultValue
    {
        velocity      realx3 (0 0 0);
        acceleration  realx3 (0 0 0);
        rVelocity     realx3 (0 0 0);
        shapeName     word   smallSphere;
    }
    
    selectors
    {
        shapeAssigne
        {
            selector stridedRange;
            
            stridedRangeInfo
            {
                begin  0;
                end    30000;
                stride 3;
            }
            
            fieldValue
            {
                shapeName word largeSphere;
            }
        }
    }
}
```

### 4.3. Example 3: Empty Initial Particle System (for Insertion Later)

```C++
positionParticles
{
    method empty;
}
```

## 5. Workflow Tips

1. To create particles at the beginning of a simulation:

   ```
   particlesPhasicFlow
   ```

2. To only position particles without setting fields:

   ```
   particlesPhasicFlow --positionParticles-only
   ```

3. To modify fields on existing particles (e.g., at a later time step):

   ```
   particlesPhasicFlow --setFields-only
   ```

4. For fluid-particle coupling simulations:

   ```
   particlesPhasicFlow -c
   ```

5. To change particle properties at a later time step in a simulation:
   - Change `startTime` in the settings dictionary to the desired time
   - Run `particlesPhasicFlow --setFields-only`
   - Restart the simulation from that time step

## 6. Common Field Types

- `realx3`: 3D vector of real values, e.g., `(0 0 0)`
- `real`: Single real value
- `word`: Text string
- `uint32`: Unsigned 32-bit integer

## 7. Shape Handling

When using the `shapeName` field, ensure that the shapes are defined in the `shape` file in the `caseSetup` directory. The utility will automatically convert shape names to shape indices and hashes for the simulation.

For any further assistance, please refer to the tutorial examples in the phasicFlow distribution.

## 8. See Also

- [Tutorials](../../tutorials) - Example cases demonstrating phasicFlow capabilities and more about particlesPhasicFlow
- [geometryPhasicFlow](../geometryPhasicFlow) - Utility for geometry creation and manipulation in phasicFlow
- [pFlowToVTK](../pFlowToVTK) - Utility for converting phasicFlow data to VTK format for visualization
- [postprocessPhasicFlow](../postprocessPhasicFlow) - Utility for post-processing simulation results