
list(APPEND SourceFiles 
entities/rotatingAxis/rotatingAxis.C
fixedWall/fixedWall.C
rotatingAxisMotion/rotatingAxisMotion.C
)

set(link_libs Kokkos::kokkos phasicFlow)

pFlow_add_library_install(MotionModel SourceFiles link_libs)



