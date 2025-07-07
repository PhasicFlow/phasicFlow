# Post-processing for Evaluating Segregation of Particles
 
    - Compatibility: phasicFlow-v-1.0
    - Solvers and tools: sphereGranFlow, postprocessPhasicFlow 

## Background

In particle-based simulations, it's often necessary to perform operations on particle fields to obtain bulk properties of the powder system (such as sums or averages). For example, we may need to calculate the mass-averaged velocity of particles in a granular flow. For bulk-processing of particle fields, we can use a mesh structure to obtain bulk properties of the powder in each cell. In this tutorial, we use a rectangular mesh in which a cuboid is divided into equal sub-spaces (cells).

This tutorial demonstrates how to obtain three important bulk properties:

- Mass-averaged solid velocity (and its fluctuations)
- Solid volume fraction
- Volume fraction of small particles (segregation measurement)

## Problem Definition

We will post-process the simulation results from the case [rotating drum with binary particles](../sphereGranFlow/binarySystemOfParticles/). The goal is to create a rectangular mesh with cuboid cells and calculate bulk properties on this mesh to analyze particle segregation patterns.

***

## Case Setup

This tutorial focuses solely on the post-processing aspect of the simulation. It assumes you have already executed the simulation and the results are available for post-processing.

**Important Note:** Postprocessing in phasicFlow can be performed in two modes:
1. **In-simulation postprocessing**: Activated during simulation runtime, allowing calculations on live simulation data
2. **Post-simulation postprocessing**: Performed after the simulation has completed

For more detailed information about different modes of postprocessing, please refer to the [official documentation page](../../../src/PostprocessData/readme.md).

### Configuration File

In `settings/postprocessDataDict`, you configure the post-processing parameters:

```C++
// file: settings/postprocessDataDict

// Enable in-simulation postprocessing (set to "no" for post-simulation only)
runTimeActive       yes;

// Shape type used in the simulation
shapeType           sphere;

components
(
    on_rectMesh
    {
        // Use Gaussian distribution for sampling and distributing 
        // particle properties over cells 
        processMethod   GaussianDistribution;
        
        processRegion   rectMesh;

        // Use time control in settingsDict file
        timeControl     settingsDict;

        // A rectangular mesh is constructed using corner points of the 
        // mesh region and number of divisions in each direction 
        rectMeshInfo 
        {
            min (-0.12 -0.12 0.00);       
            max (0.12   0.12 0.1);
            nx 36;
            ny 36;
            nz 15;
        }

        operations  
        (
            // Calculate mass-averaged particle velocity 
            avVelocity
            {
                function     average;
                
                field        velocity;
                
                fluctuation2 yes;
                
                threshold    4;
                
                phi          mass;
            }

            // Calculate solid volume fraction 
            solidVolFraction
            {
                function    sum;

                field       volume;
                
                divideByVolume yes;
                
                threshold   4;
            }

            // Calculate volume fraction of small particles 
            smallSphereVolFraction
            {
                function        average;
                
                field           one;

                phi             volume;
                
                divideByVolume  no;

                threshold       4;
                
                includeMask     lessThan;
                
                lessThanInfo
                {
                    field   diameter;
                  
                    value   0.0031;  
                }
            }
        );

    }
);
```

### Understanding the Configuration

#### Mesh Configuration
In the `rectMeshInfo` dictionary:
- `min` and `max` define the corner points of the entire domain
- `nx`, `ny`, and `nz` define the number of divisions in each direction

#### Operations
We define three operations to extract bulk properties:

1. **avVelocity**: Calculates mass-averaged particle velocity
   
   The `average` function is defined mathematically as:

   $$\text{average} = \frac{\sum_j w_j \cdot \phi_j \cdot \text{field}_j}{\sum_i w_i \cdot \phi_i}$$

   When `field` is set to velocity and `phi` is set to mass, we calculate the mass-averaged velocity of particles in each cell. With `fluctuation2` set to yes, we also calculate velocity fluctuations around the mean value.
   
   Note that:
   - Cells with fewer than 4 particles are excluded (via the `threshold` parameter)
   - $w_j$ is the weight factor determined by the Gaussian distribution function

2. **solidVolFraction**: Calculates solid volume fraction in each cell
   
   The `sum` function is defined as:

   $$\text{sum} = \sum_j w_j \cdot \phi_j \cdot \text{field}_j$$

   By setting `field` to volume and `divideByVolume` to yes, we sum the volumes of all particles in each cell and divide by the cell volume, giving us the solid volume fraction.

3. **smallSphereVolFraction**: Calculates volume fraction of small particles in each cell
   
   This operation measures the volume fraction of small particles relative to total particle volume. The `includeMask` parameter filters particles by diameter, selecting only those smaller than 0.0031 units. 
   
   The numerator of the `average` function calculates the sum of small particle volumes, while the denominator calculates the total volume of all particles.

### Particle Filtering with includeMask

The `includeMask` parameter allows filtering particles based on field values. Available options include:

* `all`: Include all particles (default)
* `lessThan`: Include particles where field < value
* `lessThanEq`: Include particles where field ≤ value 
* `greaterThan`: Include particles where field > value
* `greaterThanEq`: Include particles where field ≥ value
* `equal`: Include particles where field = value
* `between`: Include particles where value1 < field < value2
* `betweenEq`: Include particles where value1 ≤ field ≤ value2

## Running the Post-processing Tool

After completing the simulation, execute the post-processing tool with:

```bash
postprocessPhasicFlow
```

The results will be stored in the `./postprocessData` folder.

**Tip:** Run `postprocessPhasicFlow -h` to see all available command-line options, including time range selection for post-processing.