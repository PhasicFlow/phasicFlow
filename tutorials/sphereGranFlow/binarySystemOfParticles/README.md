# Problem definition

A rotating drum is randomly filled with two particle sizes and rotated to observe particle segregation. The focus of this tutorial is to show how to use the preprocessing tool `particlesPhasicFlow` to create the initial mixture of small and large particles. 

**Note:** It is supposed that you have reviewed [simulating a rotating drum](https://github.com/PhasicFlow/phasicFlow/wiki/Simulating-a-rotating-drum) tutorial before starting this tutorial.

<div align="center"><b>
a view of the rotating drum with small and large particles after 7 seconds of rotation</b>
</div>
<div align="center">
<img src="https://github.com/PhasicFlow/phasicFlow/blob/media/media/rotating-drum-binary-system.png" width="400">
</div>

***

# Case setup
[Simulation case setup files can be found in tutorials/sphereGranFlow folder.](https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/sphereGranFlow/binarySystemOfParticles)
### Shape definition 

In the file `caseSetup/shapes` two particle types with the names `smallSphere` and `largeSphere` and the diameters 3 and 5 mm are defined. 

<div align="center"> 
in <b>caseSetup/sphereShape</b> file
</div>

```C++
names        (smallSphere largeSphere); // names of shapes 
diameters    (0.003     0.005);    // diameter of shapes (m)
materials    (prop1  prop1);       // material names for shapes 
```
### Positioning and initial mixture 

In the dictionary `positionParticles` located in file `settings/particlesDict`, 30000 particles are located in a cylindrical region. These particles are positioned in order along `z`, `x` and then `y` axis with 0.005 m distance between their centers. 

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>


```C++

positionParticles       // positions particles 
{
    method ordered;     // other options: random or empty

    orderedInfo
    {        
        diameter 0.005;     // minimum space between centers of particles
        numPoints 30000;    // number of particles in the simulation 
        axisOrder (z x y);  // axis order for filling the space with particles
    }

    regionType cylinder;                                   // other options: box and sphere 

    cylinder                    // cylinder region for positioning particles 
    {
        p1 (0.0 0.0 0.003);     // begin point of cylinder axis (m m m)
        p2 (0.0 0.0 0.097);     // end point of cylinder axis (m m m)
        radius       0.117;     // radius of cylinder (m)
    }
}
```
In the `setFields` dictionary, located in the `settings/particlesDict` file, you define the initial `velocity`, `acceleration`, `rotVelocity` and `shapeName` fields for all 30000 particles in the simulation. In the `selectors' dictionary, you can select subsets of particles and set the field value for those subsets. The `selectRange` selector is defined in the `shapeAssigne` subdictionary. It defines a range with `begin`, `end` and `stride` to select particles. And in the `fieldValue` subdictionary the field values for selected particles are set (any number of field values can be set here).

**Note:** Other selectors are: `selectBox` that selects particles inside a box and `randomSelect` that selects particles randomly from a given index range. 

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
setFields
{
    /*
        Default value for fields defined for particles
    These fields should always be defined for simulations with 
    spherical particles.
*/

    defaultValue 
    {
        velocity        realx3     (0 0 0); // linear velocity (m/s)
        acceleration    realx3     (0 0 0); // linear acceleration (m/s2)
        rotVelocity     realx3     (0 0 0); // rotational velocity (rad/s)
        shapeName       word   smallSphere; // name of the particle shape 
    }

    selectors
    {
        shapeAssigne
        {
            selector     stridedRange;                     // other options: box, cylinder, sphere, randomPoints

            stridedRangeInfo
            {
                begin   0;            // begin index of points
                end     30000;        // end index of points 
                stride  3;            // stride for selector 
            }
            fieldValue  // fields that the selector is applied to 
            {
                /*
                    sets shapeName of the selected points to largeSphere
                */
                shapeName     word     largeSphere;
            }
    }
}

```

# Running the simulation 
Enter the following command in terminal:

`> geometryPhasicFlow`

`> particlesPhasicFlow`

`> sphereGranFlow`

  

### Note on using particlesPhasicFlow
Each executable in PhasicFlow comes with some command line options that you can see them by using flag `-h` in front of that command.

`> particlesPhasicFlow -h` prints out the following output:

```
Usage: particlesPhasicFlow [OPTIONS]

Options:
  -h,--help                   Help for using createParticles of phasicFlow v-0.1
  -v,--version                Program version information
  --discription               What does this app do?
  --positionParticles-only    Exectue the positionParticles part only and store the created pointStructure in the time folder.
  --setFields-only            Exectue the setFields part only. Read the pointStructure from time folder and setFields and save the result in the same time folder.
```

so, with flag `--setFields-only`, you can execute the `setFields` part of `particlesDict`. Now suppose that you have a simulation case which proceeded up to 2 seconds and for any reason you want to change some field value at time 3 s and continue the simulation from 3 s. To this end, you need to change `startTime` in settings dictionary to 3, execute `particlesPhasicFlow --setFields-only`, and start the simulation. 