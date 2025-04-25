# pFlowToVTK Utility

## Overview

pFlowToVTK is a utility for converting phasicFlow simulation data into VTK (Visualization Toolkit) file format. This enables visualization of simulation results in applications like ParaView. The utility handles two primary types of data:

1. **Particle Data** (pointField) - Converts particle properties such as position, velocity, forces, etc.
2. **Geometry Data** (triSurfaceField) - Converts surface geometries and their properties

## Basic Usage

```bash
pFlowToVTK [OPTIONS]
```

After simulation is complete, run this command from the case directory to convert all simulation data to VTK format, which will be stored in the `./VTK/` folder.

## Command Line Options

| Option | Description |
|--------|-------------|
| `--no-geometry` | Skip conversion of geometry data to VTK format |
| `--no-particles` | Skip conversion of particle data to VTK format |
| `-b, --binary` | Write VTK files in binary format (default is ASCII). Using binary format accelerates conversion (5-10x) and visualization in ParaView |
| `-o, --out-folder <path>` | Specify the output directory path (default: `./VTK/`) |
| `-s, --separate-surfaces` | Create separate files for each sub-surface in geometry |
| `-f, --fields <field1> <field2>...` | Specify which particle fields to convert (space-separated list). If not specified, all fields are converted |
| `-t, --time <times>` | Process only specific time folders. Accepts multiple formats: <br> - Space-separated list of times (e.g., `0.1 0.2 0.3`) <br> - Strided range with format `<begin>:<stride>:<end>` (e.g., `0.1:0.1:0.5`) <br> - Interval with format `<begin>:<end>` (e.g., `0.1:0.5`) |
| `-h, --help` | Display help message with all available options |

## Examples

Convert all data with default settings:

```bash
pFlowToVTK
```

Convert only geometry data:

```bash
pFlowToVTK --no-particles
```

Convert only particle data in binary format:

```bash
pFlowToVTK --no-geometry --binary
```

Convert specific fields for particles:

```bash
pFlowToVTK -f velocity diameter contactForce
```

Convert data for specific time steps:

```bash
pFlowToVTK -t 0.1 0.2 0.3
```

Convert data for a range of time steps:

```bash
pFlowToVTK -t 0.1:0.1:1.0
```

Write output to a custom directory:

```bash
pFlowToVTK -o /path/to/custom/output
```

## Output Structure

The utility creates the following directory structure:
```
simulationCase/
├── 0/              # Contains the initial time step data
├── 0.1/            # time-folder for simulation data at 0.1 s
├── 0.2/            # time-folder for simulation data at 0.2 s
├── caseSetup/      # Contains the case setup files
├── settings/       # Contains the settings files
└── VTK/
    ├── geometry/           # Contains VTK files for geometry
    │   └── surface-*.vtk   # Geometry at different time steps
    └── particles/          # Contains VTK files for particles
        └── particles-*.vtk # Particle data at different time steps
```

Additionally, the utility generates `.vtk.series` files in the VTK root folder, which can be used by ParaView for loading time series data automatically:
```
VTK/
├── particles.vtk.series
└── surface.vtk.series  # (and possibly other surface names if --separate-surfaces is used)
```

## Recommendations

- Use binary format (`-b` flag) for large simulations to significantly improve conversion speed and reduce file size
- For complex geometries with multiple sub-surfaces, consider using `-s` flag to keep surfaces in separate files for better visualization
- When working with large datasets, use the `-t` option to convert only the time steps you need to visualize

## See Also
## Related Utilities
- [`tutorials`](../../tutorials): Contains example cases and step-by-step guides for using phasicFlow
- [`particlesPhasicFlow`](../particlesPhasicFlow): Creates the initial fields for particles
- [`geometryPhasicFlow`](../geometryPhasicFlow): Creates the geometry
- [`postprocessPhasicFlow`](../postprocessPhasicFlow): Tool for performing various cell-based averaging on fields
