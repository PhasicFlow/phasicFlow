
list(APPEND SourceFiles 
entities/timeInterval/timeInterval.cpp

entities/rotatingAxis/rotatingAxis.cpp
rotatingAxisMotion/rotatingAxisMotion.cpp

entities/vibrating/vibrating.cpp
vibratingMotion/vibratingMotion.cpp

stationaryWall/stationaryWall.cpp
entities/stationary/stationary.cpp

conveyorBeltMotion/conveyorBeltMotion.cpp
entities/conveyorBelt/conveyorBelt.cpp

#entities/multiRotatingAxis/multiRotatingAxis.cpp
#multiRotatingAxisMotion/multiRotatingAxisMotion.cpp

)

set(link_libs Kokkos::kokkos phasicFlow)

pFlow_add_library_install(MotionModel SourceFiles link_libs)

