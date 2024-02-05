
list(APPEND SourceFiles 
entities/timeInterval/timeInterval.cpp
entities/rotatingAxis/rotatingAxis.cpp
rotatingAxisMotion/rotatingAxisMotion.cpp


entities/vibrating/vibrating.cpp
vibratingMotion/vibratingMotion.cpp
#fixedWall/fixedWall.cpp
#entities/multiRotatingAxis/multiRotatingAxis.cpp
#multiRotatingAxisMotion/multiRotatingAxisMotion.cpp

)

set(link_libs Kokkos::kokkos phasicFlow)

pFlow_add_library_install(MotionModel SourceFiles link_libs)

