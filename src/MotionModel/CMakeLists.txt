
list(APPEND SourceFiles 
entities/rotatingAxis/rotatingAxis.cpp
entities/multiRotatingAxis/multiRotatingAxis.cpp
entities/timeInterval/timeInterval.cpp
entities/vibrating/vibrating.cpp
fixedWall/fixedWall.cpp
rotatingAxisMotion/rotatingAxisMotion.cpp
multiRotatingAxisMotion/multiRotatingAxisMotion.cpp
vibratingMotion/vibratingMotion.cpp
)

set(link_libs Kokkos::kokkos phasicFlow)

pFlow_add_library_install(MotionModel SourceFiles link_libs)

