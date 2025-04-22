# PostprocessData Module in phasicFlow

The `PostprocessData` module in phasicFlow provides powerful tools for analyzing particle-based simulations both during runtime (in-simulation) and after simulation completion (post-simulation). This document explains how to configure and use the postprocessing features through the dictionary-based input system.

## Overview

Postprocessing in phasicFlow allows you to:

- Extract information about particles in specific regions of the domain
- Calculate statistical properties such as averages and sums of particle attributes
- Track specific particles throughout the simulation
- Apply different weighing methods when calculating statistics
- Perform postprocessing at specific time intervals

## Setting Up Postprocessing

Postprocessing is configured through a dictionary file named `postprocessDataDict` which should be placed in the `settings` directory. Below is a detailed explanation of the configuration options.

### Basic Configuration

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
    timeControl         timeStep;  // Options: timeStep, simulationTime, settings
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

If you want to activate in-simulaiton postprocessing, you need to add these lines to the `settings/settingsDict` file:

```cpp
libs   ("libPostprocessData.so");

auxFunctions    postprocessData;
```

This will link the postprocessing library to your simulation, allowing you to use its features. Note that, anytime you want to deactivate the in-simulation postprocessing, you can simply change the `runTimeActive` option to `no` in `postprocessDataDict` file.

## Time Control Options

Each postprocessing component can either use the default time control settings or define its own. There are three main options for time control:

| Option | Description | Required Parameters |
|--------|-------------|---------------------|
| `timeStep` | Controls execution based on simulation time steps | `startTime`, `endTime`, `executionInterval` |
| `simulationTime` | Controls execution based on simulation time | `startTime`, `endTime`, `executionInterval` |
| `settings` | Uses parameters from settingsDict file | None (defined elsewhere) |
| `default` | Uses the default time control settings (uses `defaultTimeControl` settings)| None (uses default) |

If no time control is specified, the `default` option is used automatically.

## Processing Methods

The postprocessing module provides several methods for processing particle data. They are categorized into two main groups: bulk and individual methods.

- **Bulk Methods**: Operate on all particles that are located in a specified locations/regions (cells, spheres, etc.).
- **Individual Methods**: Operate on specific particles, allowing for targeted particle property extraction.

| Method | Property type | Description | Formula |
|--------|------------------|-------------|---------|
| `arithmetic` | bulk | Simple arithmetic mean/sum with equal weights | Each particle contributes equally |
| `uniformDistribution` | bulk | Each particle contributes inversely proportional to the total number of particles | $w_i = 1/n$ where $n$ is the number of particles |
| `GaussianDistribution` | bulk | Weight contribution based on distance from center with Gaussian falloff | $w_i = \exp(-\|x_i - c\|^2/(2\sigma^2))/\sqrt{2\pi\sigma^2}$ |
| `particleProbe` | individual | Extracts values from specific particles | Direct access to particle properties |

## Region Types

Regions define where in the domain the postprocessing operations are applied:

| Region Type | Description | Required Parameters | Compatible with |
|-------------|-------------|---------------------|-----------------|
| `sphere` | A spherical region | `radius`, `center` | bulk |
| `multipleSpheres` | Multiple spherical regions | `centers`, `radii` | bulk |
| `line` | Spheres along a line with specified radius | `p1`, `p2`, `nSpheres`, `radius` | bulk |
| `centerPoints` | Specific particles selected by ID | `ids` | individual |

## Processing Operations

Within each processing region of type `bulk`, you can define multiple operations to be performed:

### Available Functions

| Function | Property type | Description | Formula | Required Parameters |
|----------|---------------|-------------|---------|---------------------|
| `average` | bulk | Weighted average of particle field values | see Equation 1 | `field`, `phi` (optional), `threshold` (optional), `includeMask` (optional), `divideByVolume` (optional) |
| `sum` | bulk | Weighted sum of particle field values | see Equation 2 | `field`, `phi` (optional),`threshold` (optional), `includeMask` (optional), `divideByVolume` (optional) |

Equation 1:

$\text{result}=\qquad \frac{\sum_{j} w_j \cdot \phi_j \cdot \text{field}_j}{\sum_{j} w_i \cdot \phi_i}$

Equation 2:

$\text{result} = \sum_{j} w_j \cdot \phi_j \cdot \text{field}_j$

where:

- $j$ is the index of the particle in the region that also satisfies the `includeMask`
- $i$ is the index of the particle in the region
- $w_j$ is the weight of particle $j$ based on the selected processing method
- $\phi_j$ is the value of the `phi` field for particle $j$ (default is 1)
- $field_j$ is the value of the specified field for particle $j$


### Derived Functions

In addition to the above basic functions, some derived functions are available for specific calculations:

| Function | Property type | Description | Formula | Required Parameters |
|----------|---------------|-------------|---------|---------------------|
|`avMassVelocity` | bulk | Average velocity weighted by mass | $\frac{\sum_{i \in \text{region}} m_i \cdot v_i}{\sum_{i \in \text{region}} m_i}$ | - |

### Available Fields

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

### Optional Parameters

| Parameter | Description | Default | Options |
|-----------|-------------|---------|---------|
| `divideByVolume` | Divide result by region volume | `no` | `yes`, `no` |
| `threshold` | Exclude regions with fewer particles | 1 | Integer value |
| `includeMask` | Filter particles based on a field value | `all` | `all`, `lessThan`, `greaterThan`, `between`, `lessThanOrEq`, `greaterThanEq`, `betweenEq` |

## Examples

### Example 1: Probing Individual Particles

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

### Example 2: Processing in a Spherical Region

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

### Example 3: Processing Along a Line

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

## Advanced Features

### Special functions applied on fields

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


### Particle Filtering with includeMask

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

## Implementation Notes

- The postprocessing system can work both during simulation (`runTimeActive yes`) or after simulation completion.
- When using post-simulation mode, you must specify the correct `shapeType` to properly initialize the shape objects.
- Results are written to output files in the case directory with timestamps.
- The `threshold` parameter helps eliminate statistical noise in regions with few particles.
- Setting `divideByVolume` to `yes` normalizes results by the volume of the region, useful for calculating densities.

## Mathematical Formulations

For weighted `bulk` properties calculation:

$$ \text{average} = \frac{\sum_{i \in \text{region and includeMask}} w_i \cdot \phi_i \cdot \text{field}_i}{\sum_{i \in \text{region}} w_i \cdot \phi_i} $$

For weighted summing:

$$ \text{sum} = \sum_{i \in \text{region and includeMask}} w_i \cdot \phi_i \cdot \text{field}_i $$

If `divideByVolume` is set to `yes`, the result is divided by the volume of the region:

$$ \text{volumetric result} = \frac{\text{result}}{V_{\text{region}}} $$

