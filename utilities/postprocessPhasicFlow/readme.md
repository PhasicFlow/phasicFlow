# postprocessPhasicFlow Utility

This is a documentation for the `postprocessPhasicFlow` utility. This utility is designed to perform post-simulation processing on completed simulation data.

## Table of Contents

- [1. Overview](#1-overview)
- [2. When to Use postprocessPhasicFlow](#2-when-to-use-postprocessphasicflow)
- [3. Basic Usage](#3-basic-usage)
- [4. Command Line Options](#4-command-line-options)
  - [4.1. Available Options](#41-available-options)
  - [4.2. Examples](#42-examples)
- [5. Configuration](#5-configuration)
  - [5.1. Important Configuration Parameters for Post-Simulation Processing](#51-important-configuration-parameters-for-post-simulation-processing)
- [6. Difference Between In-Simulation and Post-Simulation Processing](#6-difference-between-in-simulation-and-post-simulation-processing)
- [7. File Structure](#7-file-structure)
- [8. Advanced Features](#8-advanced-features)
- [9. Example Configuration](#9-example-configuration)
- [10. Troubleshooting](#10-troubleshooting)
- [11. See Also](#11-see-also)

## 1. Overview

`postprocessPhasicFlow` is a utility for performing postprocessing analysis on completed simulation data. It allows you to extract statistical information, track specific particles, and analyze regional particle behavior without needing to re-run your simulations. This utility leverages the functionality provided by phasicFlow's `PostprocessData` module to analyze data stored in time folders.

## 2. When to Use postprocessPhasicFlow

You should use this utility when:

1. Your simulation has completed and you want to analyze the results
2. You want to try different postprocessing configurations on the same simulation data
3. You need to extract additional information that wasn't included in real-time processing
4. You want to focus on specific time periods or regions of your simulation domain

## 3. Basic Usage

To use the `postprocessPhasicFlow` utility, navigate to your simulation case directory and run:

```bash
postprocessPhasicFlow
```

This will read the `postprocessDataDict` file from your case's `settings` directory and perform the configured postprocessing operations on all available time folders.

## 4. Command Line Options

The `postprocessPhasicFlow` utility supports several command line options that allow you to customize its behavior:

```bash
postprocessPhasicFlow [OPTIONS]
```

### 4.1. Available Options

| Option | Description |
|--------|-------------|
| `-o, --out-folder <path>` | Specify the output directory path where postprocessing results will be written. Default is `./postprocessData/` |
| `-t, --time <times>` | Process only specific time folders. Accepts multiple formats: <br> - Space-separated list of times (e.g., `0.1 0.2 0.3`) <br> - Strided range with format `<begin>:<stride>:<end>` (e.g., `0.1:0.1:0.5`) <br> - Interval with format `<begin>:<end>` (e.g., `0.1:0.5`) |
| `-z, --zeroFolder` | Include the zero folder (initial state) in the processing. By default, the zero folder is not processed. |
| `-h, --help` | Display help message with all available options |

### 4.2. Examples

Process all available time folders (except zero folder):

```bash
postprocessPhasicFlow
```

Process only specific time steps:

```bash
postprocessPhasicFlow -t 0.1 0.2 0.5
```

Process a range of time steps from 0.1 to 1.0 with 0.1 increments:

```bash
postprocessPhasicFlow -t 0.1:0.1:1.0
```

Process all time steps including the initial state (zero folder):

```bash
postprocessPhasicFlow -z
```

Write output to a custom directory:

```bash
postprocessPhasicFlow -o /path/to/custom/output
```


## 5. Configuration

The `postprocessPhasicFlow` utility is configured through the same `postprocessDataDict` file used for in-simulation postprocessing. This file should be placed in the `settings` directory of your case.

### 5.1. Important Configuration Parameters for Post-Simulation Processing

When using `postprocessPhasicFlow` for post-simulation processing, pay special attention to these settings:

```cpp
// For post-simulation, runTimeActive should be set to "no"
// This indicates that you're doing post-simulation processing
runTimeActive no;

// Specify the correct shape type used in your simulation
// This is essential for post-simulation processing
shapeType sphere; // Options: sphere, grain, etc.
```

The `shapeType` parameter is particularly crucial for post-simulation processing as it tells the utility what kind of particles were used in the simulation, allowing it to correctly interpret the stored data.

## 6. Difference Between In-Simulation and Post-Simulation Processing

| Feature | In-Simulation Processing | Post-Simulation Processing with postprocessPhasicFlow |
|---------|--------------------------|--------------------------------------------------------|
| When it runs | During simulation execution | After simulation is complete |
| How to activate | Set `runTimeActive yes` in postprocessDataDict, set `auxFunctions  postprocessData` in settingsDict and add library | Run the `postprocessPhasicFlow` utility |
| Data access | Direct access to simulation data in memory | Reads data from time folders |
| Performance impact | May slow down simulation | No impact on simulation performance |
| Iterations | Can only run once per simulation step | Can be run multiple times with different settings |

## 7. File Structure

The results of the postprocessing will be written to files in your case directory (by default, under `postprocessData` folder), with timestamps and naming that matches your configured components and operations. These files can then be used for further analysis or visualization.

## 8. Advanced Features

`postprocessPhasicFlow` supports all the features of the PostprocessData module, including:

- Different processing methods (arithmetic, uniformDistribution, GaussianDistribution)
- Various region types (sphere, multipleSpheres, line, centerPoints)
- Multiple operation types (average, sum, avMassVelocity)
- Special field functions (component, abs, square, magnitude, etc.)
- Particle filtering with includeMask

but it disables/ignores the effect of all `timeControl` settings in postprocess components. 

For detailed information about these features, refer to the [PostprocessData module documentation](../../src/PostprocessData/readme.md).

## 9. Example Configuration

Here's a simple example of a `postprocessDataDict` file configured for post-simulation processing:

```cpp
/* -------------------------------*- C++ -*--------------------------------- *\ 
|  phasicFlow File                                                            | 
|  copyright: www.cemf.ir                                                     | 
\* ------------------------------------------------------------------------- */  
objectName   postprocessDataDict;
objectType   dictionary;;
fileFormat   ASCII;
/*---------------------------------------------------------------------------*/

// Set to "no" for post-simulation processing
runTimeActive no;

// Required for post-simulation processing - specify the shape type used in the simulation
shapeType sphere;

// Default time control settings
defaultTimeControl
{
    timeControl         timeStep;
    startTime           0;
    endTime             1000;
    executionInterval   20;
}

// List of postprocessing components
components
(
    // Track specific particles by ID
    particleTracker
    {
        processMethod    particleProbe;
        processRegion    centerPoints;
        selector         id;
        field            velocity;
        ids              (10 42 87);
        timeControl      default;
    }
    
    // Analyze particle behavior in a spherical region
    centerRegionAnalysis
    {
        processMethod    arithmetic;
        processRegion    sphere;
        
        sphereInfo
        {
            radius  0.05;
            center  (0 0 0.1);
        }
        
        timeControl default;
        
        operations
        (
            velocityMagnitude
            {
                function        average;
                field           mag(velocity);
                threshold       5;
                divideByVolume  no;
                fluctuation2    yes;
            }
            
            particleDensity
            {
                function        sum;
                field           mass;
                divideByVolume  yes;
            }
        );
    }
);
```

## 10. Troubleshooting

If you encounter issues with the `postprocessPhasicFlow` utility, check the following:

1. Make sure your `postprocessDataDict` file specifies the correct `shapeType`
2. Verify that the time folders contain all the required field data
3. Check that field names referenced in your configuration exist in the simulation data
4. Ensure that the simulation case structure is intact and not modified

## 11. See Also

- [PostprocessData module documentation](../../src/PostprocessData/readme.md)
- [phasicFlow simulation tutorials](../../tutorials/)