# PostprocessData Module in phasicFlow

The `PostprocessData` module in phasicFlow provides powerful tools for analyzing particle-based simulations both during runtime (in-simulation) and after simulation completion (post-simulation). This document explains how to configure and use the postprocessing features through the dictionary-based input system.

- **In-simulation**: This is postprocessing that is active during simulation. When running a solver, it allows for real-time data analysis and adjustments based on the simulation's current state. See below to learn how you can activate in-simulation postprocessing.
- **Post-simulation**: This is postprocessing that is done after the simulation is completed. It allows for detailed analysis of the simulation results, including data extraction and visualization based on the results stored in time folders. If you want to use post-simulation, you need to run the utility `postprocessPhasicFlow` in the terminal (in the simulation case setup folder) to execute the postprocessing. This utility reads the `postprocessDataDict` file and performs the specified operations on the simulation data.

### Important Notes

* **NOTE 1:**  
Postprocessing for in-simulation is not implemented for MPI execution. So, do not use it when using MPI execution. For post-simulation postprocessing, you can use the `postprocessPhasicFlow` utility without MPI, even though the actual simulation has been done using MPI.

* **NOTE 2:**  
In post-simulation mode, all `timeControl` settings are ignored. The postprocessing will be done for all the time folders that are available in the case directory, or if you specify the time range in the command line, the postprocessing will be done for the time folders within the specified range.

## Table of Contents

- [1. Overview](#1-overview)
- [2. Setting Up Postprocessing](#2-setting-up-postprocessing)
  - [2.1. Basic Configuration](#21-basic-configuration)
- [3. Time Control Options](#3-time-control-options)
- [4. Processing Methods](#4-processing-methods)
- [5. Region Types](#5-region-types)
- [6. Processing Operations](#6-processing-operations)
  - [6.1. Available Functions in average](#61-available-functions-in-average)
  - [6.2. About fluctuation2 in average function](#62-about-fluctuation2-in-average-function)
  - [6.3. Derived Functions](#63-derived-functions)
  - [6.4. Available Fields](#64-available-fields)
  - [6.5. Optional Parameters](#65-optional-parameters)
- [7. Examples](#7-examples)
  - [7.1. Example 1: Probing Individual Particles](#71-example-1-probing-individual-particles)
  - [7.2. Example 2: Processing in a Spherical Region](#72-example-2-processing-in-a-spherical-region)
  - [7.3. Example 3: Processing Along a Line](#73-example-3-processing-along-a-line)
  - [7.4. Example 4: Processing in a Rectangular Mesh](#74-example-4-processing-in-a-rectangular-mesh)
  - [7.5. Example 5: Tracking particles](#75-example-5-tracking-particles)
- [8. Advanced Features](#8-advanced-features)
  - [8.1. Special Functions Applied on Fields](#81-special-functions-applied-on-fields)
  - [8.2. Particle Filtering with IncludeMask](#82-particle-filtering-with-includemask)
  - [8.3. Implementation Notes](#83-implementation-notes)
- [9. Mathematical Formulations](#9-mathematical-formulations)
- [10. A Complete Dictionary File (postprocessDataDict)](#10-a-complete-dictionary-file-postprocessdatadict)

## 1. Overview

Postprocessing in phasicFlow allows you to:

- Extract information about particles in specific regions of the domain
- Calculate statistical properties such as averages and sums of particle attributes
- Track specific particles throughout the simulation
- Apply different weighting methods when calculating statistics
- Perform postprocessing at specific time intervals

## 2. Setting Up Postprocessing

Postprocessing is configured through a dictionary file named `postprocessDataDict`, which should be placed in the `settings` directory. Below is a detailed explanation of the configuration options.

### 2.1. Basic Configuration

The input dictionary, **settings/postprocessDataDict**, may look like this:

```cpp
// PostprocessData dictionary

// Enable/disable postprocessing during simulation
runTimeActive yes;  // Options: yes, no

// Shape type - only needed when doing post-simulation processing
shapeType sphere;   // Options depend on the simulation type: sphere, grain, etc.

// Default time control for postprocessing components
defaultTimeControl
{
    timeControl         timeStep;  // Options: timeStep, simulationTime, settingsDict
    startTime           0;         // Start time for postprocessing
    endTime             1000;      // End time for postprocessing
    executionInterval   150;       // How frequently to run postprocessing
}

// List of postprocessing components
components
(
    // Component definitions here...
);
```

If you want to activate in-simulation postprocessing, you need to add these lines to the `settings/settingsDict` file:

```cpp
libs   ("libPostprocessData.so");

auxFunctions    postprocessData;
```

This will link the postprocessing library to your simulation, allowing you to use its features. Note that anytime you want to deactivate the in-simulation postprocessing, you can simply change the `runTimeActive` option to `no` in the `postprocessDataDict` file.

## 3. Time Control Options

Each postprocessing component can either use the default time control settings or define its own. There are three main options for time control:

| Option | Description | Required Parameters |
|--------|-------------|---------------------|
| `timeStep` | Controls execution based on simulation time steps | `startTime`, `endTime`, `executionInterval` |
| `simulationTime` | Controls execution based on simulation time | `startTime`, `endTime`, `executionInterval` |
| `settingsDict` | Uses parameters from settingsDict file | None (defined elsewhere) |
| `default` | Uses the default time control settings (uses `defaultTimeControl` settings) | None (uses default) |

If no time control is specified, the `default` option is used automatically.

## 4. Processing Methods

The postprocessing module provides several methods for processing particle data. They are categorized into two main groups: bulk and individual methods.

- **Bulk Methods**: Operate on all particles that are located in specified locations/regions (cells, spheres, etc.).
- **Individual Methods**: Operate on specific particles, allowing for targeted particle property extraction.

| Method | Property Type | Description | Formula |
|--------|---------------|-------------|---------|
| `arithmetic` | bulk | Simple arithmetic mean/sum with equal weights | Each particle contributes equally |
| `uniformDistribution` | bulk | Each particle contributes inversely proportional to the total number of particles | $w_i = 1/n$ where $n$ is the number of particles |
| `GaussianDistribution` | bulk | Weight contribution based on distance from the center with Gaussian falloff | $w_i = \exp(-\|x_i - c\|^2/(2\sigma^2))/\sqrt{2\pi\sigma^2}$ |
| `particleProbe` | individual | Extracts values from specific particles | Direct access to particle properties |

## 5. Region Types

Regions define where in the domain the postprocessing operations are applied:

| Region Type | Description | Required Parameters | Compatible with |
|-------------|-------------|---------------------|-----------------|
| `sphere` | A spherical region | `radius`, `center` defined in `sphereInfo` dict| bulk |
| `multipleSpheres` | Multiple spherical regions | `centers`, `radii` defined in `multiplSpheresInfo` dict | bulk |
| `line` | Spheres along a line with specified radius | `p1`, `p2`, `nSpheres`, `radius` defined in `lineInfo` dict| bulk |
| `box`| A cuboid region | `min`, `max` defined in `boxInfo` dict | bulk |
| `rectMesh`** | creates a rectangular mesh and each direction is divided into equal spaces| corner points of mesh, and `nx`, `ny`, `nz`: number of divisions in each direction | bulk |
| `centerPoints`* | if `selector` is set to `id`, particles selected by ID list | `ids`: a list of particle ids | individual |
| `centerPoints`* | if `selector` is set to `box`, particles are selected by center points located in a box | corner points of the box are given in `boxInfo` sub-dict | individual |
| `centerPoints`* | if `selector` is set to `sphere`, particles are selected by center points located in a sphere | center and radius of a sphere given in `sphereInfo` sub-dict | individual |
| `centerPoints`* | if `selector` is set to `cylinder`, particles are selected by center points located in a cylinder | axis info and radius of cylinder at end points that are given in `cylinderInfo` sub-dict | individual |
| <td colspan="3">\* Particles selection is done when simulation reaches the time that is specified by `startTime` of the post-process component and this selection remains intact up to the end of simulation. This is very good for particle tracking purposes or when you want to analyze specific particles behavior over time.</td> |
| <td colspan="3">\** This region creates a rectangular mesh and particles are located into cells according to their center points. When using `GaussianDistribution` as `processMethod`, a larger neighbor radius is considered for each cell and particles inside this neighbor radius are included in the calculations.</td> |

## 6. Processing Operations for Bulk Properties

Within each processing region of type `bulk`, you can define multiple operations to be performed:

### 6.1. Available Functions in average

| Function | Property type | Description | Formula | Required Parameters |
|----------|---------------|-------------|---------|---------------------|
| `average` | bulk | Weighted average of particle field values | see Equation 1 | `field`, `phi` (optional), `threshold` (optional), `includeMask` (optional), `divideByVolume` (optional), `fluctuation2` (optional) |
| `sum` | bulk | Weighted sum of particle field values | see Equation 2 | `field`, `phi` (optional),`threshold` (optional), `includeMask` (optional), `divideByVolume` (optional) |

Equation 1:

$$\text{mean} = \frac{\sum_j w_j \cdot \phi_j \cdot \text{field}_j}{\sum_i w_i \cdot \phi_i}$$

Equation 2:

$$\text{sum} = \sum_j w_j \cdot \phi_j \cdot \text{field}_j$$

where:

- $j$ is the index of the particle in the region that also satisfies the `includeMask`
- $i$ is the index of the particle in the region
- $w_j$ is the weight of particle $j$ based on the selected processing method
- $\phi_j$ is the value of the `phi` field for particle $j$ (default is 1)
- $field_j$ is the value of the specified field for particle $j$

### 6.2. About fluctuation2 in average function

`fluctuation2` is an optional parameter that can be used to account for fluctuations in the particle field values with respect to mean value of the field.
It is used in the `average` function to calculate the fluctuation of the field values around the mean. The formula for fluctuation2 is:

$$\text{fluctuation}^2 = \frac{\sum_j w_j \cdot \phi_j \cdot (\text{field}_j - \text{mean})^2}{\sum_i w_i \cdot \phi_i}$$

where:

- `mean`: is the average value of the field in the region.
- `field`: The field to be processed (e.g., `velocity`, `mass`, etc.)
- `fluctuation2`: Optional parameter to account for fluctuations in the particle field values.

### 6.3. Derived Functions

In addition to the above basic functions, some derived functions are available for specific calculations:

| Function | Property type | Description | Formula | Required Parameters |
|----------|---------------|-------------|---------|---------------------|
|`avMassVelocity` | bulk | Average velocity weighted by mass | $\frac{\sum_{i \in \text{region}} w_i \cdot m_i \cdot v_i}{\sum_{i \in \text{region}} w_i \cdot m_i}$ | - |

### 6.4. Available Fields

All the pointFields in the simulation database (for in-simulation processing), or the ones stored in the time folders (for post-simulation processing) can be referenced in the operations. In addition to them, some extra fields are available for use in the operations. The following fields are available for use in the operations:

1. Extra fileds to be used in post-processing operations:

| Field | Field Type | Description | Default Value |
|-------|------------|-------------|---------------|
| `position` | `realx3` | Particle positions | - |
| `one` | `real` | Value 1 for each particle | 1 |
| `mass` | `real` | Particle mass | - |
| `density` | `real` | Particle density | - |
| `volume` | `real` | Particle volume | - |
| `diameter` | `real` | Particle diameter | - |
| `I` | `real` | Moment of inertia | - |

2. Common fields which are available in the simulation database/time folders:

| Field | Field Type | Description |
|-------|------------|-------------|
| `velocity` | `realx3` | Particle velocity |
| `rVelocity` | `realx3` | Particle rotational velocity |
| `acceleration` | `realx3` | Particle acceleration |
| `rAcceleration` | `realx3` | Particle rotational acceleration |
| `contactForce` | `realx3` | Particle contact force |
| `contactTorque` | `realx3` | Particle contact torque |
| `id` | `integer` | Particle ID |
| `shapeIndex` | `integer` | Particle shape index |

The above fields may vary from one type of simulation to other. Pleas note that this is only a tentative list.

### 6.5. Optional Parameters

| Parameter | Description | Default | Options |
|-----------|-------------|---------|---------|
| `divideByVolume` | Divide result by region volume | `no` | `yes` or `no` |
| `threshold` | Exclude regions with fewer particles | 1 | Integer value |
| `includeMask` | Filter particles based on a field value | `all` | `all`, `lessThan`, `greaterThan`, `between`, `lessThanOrEq`, `greaterThanOrEq`, `betweenEq` |
|`fluctuation2` (in average only)| Calculate fluctuation of field values | `no` | `yes` or `no` |
| `phi` | Field to be used for weighted averaging | `one` | Any valid field name |

## 7. Examples

### 7.1. Example 1: Probing Individual Particles

```cpp
velocityProb
{
    processMethod    particleProbe;
    processRegion    centerPoints;
    selector         id;
    field            component(position,y);
    ids              (0 10 100);
    timeControl      default;
}
```

This example extracts the y-component of the position for particles with IDs 0, 10, and 100.

### 7.2. Example 2: Processing in a Spherical Region

```cpp
on_single_sphere
{
    processMethod    arithmetic;
    processRegion    sphere;
    
    sphereInfo
    {
        radius  0.01;
        center  (-0.08 -0.08 0.015);
    }
    
    timeControl default;
    
    operations
    (
        averageVel
        {
            function        average;
            field           mag(velocity);
            divideByVolume  no;
            fluctuation2    yes;
            threshold       3;
            includeMask     all;
        }
        
        par1Fraction
        {
            function        average;
            field           one;
            phi             one;
            divideByVolume  no;
            includeMask     lessThan;
            
            lessThanInfo
            {
                field       diameter;
                value       0.0031;
            }
        }
        
        numberDensity
        {
            function        sum;
            field           one;
            phi             one;
            divideByVolume  yes;
        }
    );
}
```

This example defines a sphere region and performs three operations:

1. Calculate the average of velocity magnitude of particles
2. Calculate the fraction of particles with diameter less than 0.0031
3. Calculate the number density by summing and dividing by volume

### 7.3. Example 3: Processing Along a Line

In this example, a line region is defined. The `lineInfo` section specifies the start and end points of the line, the number of spheres to create along the line, and the radius of each point. Bulk properties are calculated in each sphere, based on the properties of particles contained in each sphere.

```cpp
along_a_line
{
      processMethod    arithmetic;
      processRegion    line;
      
      timeControl simulationTime;
      startTime   1.0;
      endTime     3.0;
      executionInterval 0.1;
      
      lineInfo
      {
        p1   (0 0 0);
        p2   (0 0.15 0.15);
        nSpheres    10;
        radius    0.01;
      }
      
      operations
      (
          bulkDensity
          {
              function       sum;
              field          mass;
              phi            one;
              divideByVolume yes;
          }
          
          volumeDensity
          {
            function         sum;
            field            volume;
            divideByVolume   yes;
          }
     );
}
```

This example creates 10 spherical regions along a line from (0,0,0) to (0,0.15,0.15) and calculates the bulk density and volume density in each region.

### 7.4 Example 4: Processing in a Rectangular Mesh

In this example, a rectangular mesh is defined. The `rectMeshInfo` section specifies the minimum and maximum corner points of the box, the number of divisions in each direction, and an optional cell extension factor which is effective for GaussianDistribution only. In the `operations` section, two operations are defined: one for calculating the average velocity and another for calculating the solid volume fraction.

```cpp
on_a_rectMesh
{
    processMethod   GaussianDistribution;
    processRegion   rectMesh;

    timeControl     settingsDict; // uses settings from settingsDict file

    rectMeshInfo 
    {
        min (-0.12 -0.12 0.00); // lower corner point of the box 
        max (0.12   0.12 0.11); // upper corner point of the box
        
        nx 30; // number of divisions in x direction
        ny 30; // number of divisions in y direction
        nz 15; // number of divisions in z direction

        // optional (default is 2.0)
        // for each cell, a neighbor radius is considered. This neighbor radius is equal to 
        // cellExtension * equivalent diameter of the cell. 
        // cell extension is only effective when using GaussianDistribution as processMethod.
        cellExtension 3; 
    }

    operations
    (
        avVelocity
        {
            function     average;
            field        velocity;
            fluctuation2 yes;
            threshold    4;
            phi          mass;
        }

        solidVolumeFraction
        {
            function sum;
            field    volume;
            divideByVolume yes;
        }
    );
}
```

### 7.5 Example 5: Tracking particles

Suppose we want to mark and track the position of particles that are located inside a box region at t = 1 s. All particles that are inside the box at t = 1 s will be marked/selected and then the position of them are recorded along the simulation time. The following example shows how to do this. Note that marking/selecting of particles is done at the instance that is defined by `startTime`.  

```C++
particlesTrack
{
    processMethod      particleProbe;
    
    processRegion      centerPoints;
    
    // all particles whose ceters are located inside this box 
    // are selected. Selection occurs at startTime: particles 
    // that are inside the box at t = startTime. 
    selector           box;
    boxInfo
    {
        min            (0 0 0);
        max            (0.1 0.05 0.05);
    }
    
    // center position of selected particles are processed 
    field              position;
    
    timeControl        simulationTime;
    // execution starts at 1.0 s
    startTime           1.0;
    // execution ends at 100 s
    endTime             100;
    // execution interval of this compoenent 
    executionInterval   0.02;

}
```

## 8. Advanced Features

### 8.1. Special functions applied on fields

You can access specific components of vector fields (`realx3`) using the `component` function:

```cpp
field  component(position,y);  // Access the y-component of position
```

Here is a complete list of these special functions: 

| Function Name | Valid field Type | Example |
|-----------------|------------|---------|
| `component` | `realx3` | `component(velocity,x)` |
| `abs` | `real` | `abs(s)` |
| `square` | `real` | `square(I)` |
| `cube` | `real` | `cube(diameter)` |
| `squre root` | `real` | `sqrt(mass)` |
| `magnitude` | `realx3` | `mag(contactForce)` |
| `magnitude square` | `realx3` | `magSquare(velocity)` |
| `magnitude cube` | `realx3` | `magCube(velocity)` |
| `magnitude square root` | `realx3` | `magSqrt(acceleration)` |

### 8.2. Particle Filtering with includeMask

The `includeMask` parameter allows you to filter particles based on field values:

```cpp
includeMask  lessThan;
lessThanInfo
{
    field   diameter;
    value   0.0031;
}
```

Supported masks:

- `all`: Include all particles (default)
- `lessThan`: Include particles where field < value
- `greaterThan`: Include particles where field > value
- `between`: Include particles where value1 < field < value2
- `lessThanOrEq`: Include particles where field ≤ value
- `greaterThanOrEq`: Include particles where field ≥ value
- `betweenEq`: Include particles where value1 ≤ field ≤ value2

### 8.3. Implementation Notes

- The postprocessing system can work both during simulation (`runTimeActive yes`) or after simulation completion.
- When using post-simulation mode, you must specify the correct `shapeType` to properly initialize the shape objects.
- Results are written to output files in the case directory with timestamps.
- The `threshold` parameter helps eliminate statistical noise in regions with few particles.
- Setting `divideByVolume` to `yes` normalizes results by the volume of the region, useful for calculating densities.

## 9. Mathematical Formulations

For weighted `bulk` properties calculation, we have these two general formulations:

- For weighted averaging:

$$ \text{average} = \frac{\sum_j w_j \cdot \phi_j \cdot \text{field}_j}{\sum_i w_i \cdot \phi_i} $$

- For weighted summing:

$$ \text{sum} = \sum_j w_j \cdot \phi_j \cdot \text{field}_j $$

If `divideByVolume` is set to `yes`, the result is divided by the volume of the region:

$$ \text{volumetric result} = \frac{\text{result}}{V_{\text{region}}} $$

## 10. A complete dictionary file (postprocessDataDict)

```C++
/* -------------------------------*- C++ -*--------------------------------- *\ 
|  phasicFlow File                                                            | 
|  copyright: www.cemf.ir                                                     | 
\* ------------------------------------------------------------------------- */  
objectName   postprocessDataDict;
objectType   dictionary;;
fileFormat   ASCII;
/*---------------------------------------------------------------------------*/

// Yes: postprocessing is active during the simulation
// No: postprocessing is not active during the simulation
// and it can be done after simulation
runTimeActive yes;

// shapeType: defines the type of the shape that is used in the simulation
// (for example: sphere, grain, etc).
// shapeType is only used when postprocessing is done after simulation 
// to initialize the shape object for post processing operatoins 
shapeType sphere;

// default time control to be used in the postprocessing components
defaultTimeControl
{
	timeControl         timeStep; // timeStep, simulationTime are the options here 
	startTime           0;
	endTime             1000;
	executionInterval   150; 
}

// list of postprocessing components
components
(
    // probing particles for their state variables, like velocity, position, etc
    velocityProb
    {
        processMethod    particleProbe;
        processRegion    centerPoints;
        selector         id;
        field            component(velocity,y);
        ids              (0 10 100);
        timeControl      default; // other options are settings, timeStep, simulationTime 
        // settingsDict: uses parameters from settingsDict file
        // timeStep: uses the time step of the simulation controlling the execution of postprocessing
        // simulationTime: uses the simulation time of the simulation controlling the execution of postprocessing
        // default: uses the default time control (defined in defaultTimeControl).
        // default behavior: if you do not specify it, parameters in defaultTimeControl is used.
    }

    particlesTrack
    {
        processMethod      particleProbe;
        
        processRegion      centerPoints;
        
        // all particles whose ceters are located inside this box 
        // are selected. Selection occurs at startTime: particles 
        // that are inside the box at t = startTime. 
        selector           box;
        boxInfo
        {
            min            (0 0 0);
            max            (0.1 0.05 0.05);
        }
        
        // center position of selected particles are processed 
        field              position;
        
        timeControl        simulationTime;
        // execution starts at 1.0 s
        startTime           1.0;
        // execution ends at 10 s
        endTime             10;
        // execution interval of this compoenent 
        executionInterval   0.02;

    }

    on_a_rectMesh
    {
        processMethod   GaussianDistribution;
        processRegion   rectMesh;

        timeControl     settingsDict; // uses settings from settingsDict file

        rectMeshInfo 
        {
            min (-0.12 -0.12 0.00); // lower corner point of the box 
            max (0.12   0.12 0.11); // upper corner point of the box
            
            nx 30; // number of divisions in x direction
            ny 30; // number of divisions in y direction
            nz 15; // number of divisions in z direction

            // optional (default is 2.0)
            // for each cell, a neighbor radius is considered. This neighbor radius is equal to 
            // cellExtension * equivalent diameter of the cell. 
            // cell extension is only effective when using GaussianDistribution as processMethod.
            cellExtension 3; 
        }

        operations
        (
            avVelocity
            {
                function     average;
                field        velocity;
                fluctuation2 yes;
                threshold    4;
                phi          mass;
            }

            solidVolumeFraction
            {
                function sum;
                field    volume;
                divideByVolume yes;
            }
        );
    }
    
    on_single_sphere
    {
        // method of performing the sum (arithmetic, uniformDistribution, GaussianDistribution)
        processMethod    arithmetic; 

        // Postprocessing is done on particles whose centers are inside this spehre
        processRegion    sphere;  
        
        sphereInfo
        {
            radius  0.01; // radius of sphere
            center  (-0.08 -0.08 0.015); // center of sphere
        }
        
        timeControl default;
        
        /// all the postprocess operations to be done on sphere region
        operations
        (
            // computes the arithmetic mean of particle velocity 
            averageVel
            {
                function	    average;
                field           velocity;
                fluctuation2    yes;
                divideByVolume 	no;  // default is no
                threshold 	    3;   // default is 1
                includeMask     all; // default is all
            }
            // - function: average, sum, and other derived ones from sum and average 
            // - field: names of the fields in the simulation. Some special fields 
            //   are: mass, density, volume, position, one, I. 
            // - divideByVolume: whether the result is divided by the volume of the region 
            // - threshold: exclude regions that contains particles less than threshold
            // - includeMask: all, lessThan, greaterThan, between, lessThanOrEq, greaterThanEq, betweenEq
            
            
            // computes the fraction of par1 in the region 
            par1Fraction
            {
                function        average;
                field           one; // default 
                phi             one; // default
                divideByVolume  no;
                includeMask     lessThan;
                
                // diameter of par1 is 0.003, so these settings 
                // will select only particles of type par1
                lessThanInfo
                {
                    field      diameter;
                    value      0.0031;  
                }
            }

            numberDensity
            {
                function        sum;
                field           one;
                phi             one;
                divideByVolume  yes;
                
            }
        );
    }
    
    along_a_line
    {
        processMethod    arithmetic;

        processRegion    line;

        // the time interval for executing the post-processing 
        // other options: timeStep, default, and settings
        timeControl simulationTime; 
        startTime   1.0;
        endTime     3.0;
        executionInterval 0.1;
        
        // 10 spheres with radius 0.01 along the straight line defined by p1 and  p2 
        lineInfo
        {
          p1        (0 0 0);
          p2        (0 0.15 0.15);
          nSpheres  10;
          radius    0.01;
        }

        operations
        (
            // computes the arithmetic mean of particle velocity 
            numberDensity
            {
                function	    sum;
                field           one;
                divideByVolume  yes;  //default is no 
            }
            
            volumeDensity
            {
                function         sum;
                field            volume; // 
                divideByVolume   yes;    //default is no 
            }
        );
    }
    
);
```
