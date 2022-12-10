
list(APPEND SourceFiles 
entities/rotatingAxis/rotatingAxis.cpp
fixedWall/fixedWall.cpp
rotatingAxisMotion/rotatingAxisMotion.cpp
)

set(link_libs Kokkos::kokkos phasicFlow)

pFlow_add_library_install(MotionModel SourceFiles link_libs)



