# phasicFlow `processDataDict` File Explanation
## Problem definition 
 There is a rotating drum with the diameter 0.24 m and the length 0.1 m rotating at 11.6 rpm. It is filled with 30,000 4-mm spherical particles. The timestep for integration is 0.00001 s.The problem is to postprocessing some data such as velocity, position, one, mass. 



## Setting up the case 
 
  To postprocess these data, we should configurate `processDataDict`  file. The code is divided into the following two main parts: 1. Time Control Configuration 2. Components. 

## Time Control Configuration

### `defaultTimeControl`
Defines default time management for processing tasks:


```cpp
   defaultTimeControl

   {
    timeControl           timeStep;    //Time stepping mode
	startTime             0;           //Start time for post-processing
	executionInterval     10000;        //Interval between processing events
    }
 ```



## Components

Each entry in the `components` section specifies a different post-processing task as shown below. After running the code, a file with the same name is generated in the simulation folder, containing the data.

### 1. `velocityProb`
- **Purpose**: Probe the y-component of velocity for specific particles.


```cpp
velocityProb
    {
        processMethod   particleProbe;              //probes at selected particles
        processRegion   centerPoints;               //at particle centers

        timeControl     default;                   //Uses default time control.
        
        selector        id;                        //selected particles by particle ID
        
        ids             (0 10 20 100 1000);        //list of particle IDs that has beeen selected arbitrarily.

        field           component(velocity,y);    //y-component of velocity
        
    }

```

### 2. `positionProb`
- **Purpose**: Probe positions at randomly selected points.

```cpp
positionProb
    {
        processMethod   particleProbe;
        processRegion   centerPoints;
        
        timeControl     settingsDict;            //Uses `settingsDict` time control.
        
        selector        randomPoints;           //selects by random points.
                
        randomPointsInfo
        {
            begin        0;                          //start of rang
            end          20000;                      //end of rang
            number       20;                         //number of points
        }

        field             position;             
        
    }
    

```

### 3. `alongALine`
- **Purpose**: Compute statistics along a line, dividing it into spheres (probes).

```cpp
alongALine
    {
        processMethod    arithmetic; 
        processRegion    line;

        // 10 spheres with radius 0.01 along the straight line defined by p1 and  p2 
        lineInfo
        {
            p1             ( 0     0     0.05);      
            p2             (0.084 -0.084 0.05);      
            nSpheres        10;                      
            radius          0.01;
        }

        timeControl         simulationTime;         //custom time control.
        
        executionInterval   0.1;
          
        startTime           0.4;

        operations
        (
            avVelocity1                             //   Average velocity magnitude (mass-weighted, with fluctuation)
            {
                function        average;            
                field           mag(velocity);
                fluctuation2    yes;
                phi             mass;
            }

            numberDensity1                         //Number density (normalized by volume, threshold 10)
            {
                function        sum;
                field           one;
                divideByVolume  yes;
                threshold       10;
            }

            parCount1                              //Particle count (sum)
            {
                function       sum;
                field          one;
            }

            volFrac1                              //Volume fraction (normalized by volume, threshold 4)
            {
                function         sum;
                field            volume;
                divideByVolume   yes;
                threshold        4;
            }
        );
    }


```

### 4. `alongALine2`
- **Purpose**: Compute Gaussian-distributed statistics along a line.



```cpp
alongALine2
    {
        processMethod    GaussianDistribution; 
        processRegion    line;

        // 10 spheres with radius 0.01 along the straight line defined by p1 and  p2 
        lineInfo
        {
            p1             ( 0     0     0.05);
            p2             (0.084 -0.084 0.05);
            nSpheres        10;
            radius          0.01;
        }

        timeControl         timeStep;

        executionInterval   100;
          
        startTime            0;

        operations
        (
            avVelocity                       //Average velocity magnitude (mass-weighted)
            {
                function    average;
                field       mag(velocity);
                phi         mass;
            }

            bulkDensity                      //Bulk density (sum of mass, volume-normalized, threshold 5)
            {
                function              sum;
                field                 mass;
                phi                   one;
                divideByVolume        yes;
                threshold             5;
            }

            parCount                       //Particle count
            {
                function    sum;
                field       one;
            }

            volFrac                       //Volume fraction (volume-normalized, threshold 4)

            {
                function             sum;
                field                volume;
                divideByVolume       yes;
                threshold            4;
            }
        );
    }
    




```