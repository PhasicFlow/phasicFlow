
set(source_files 
geometryPhasicFlow.C
Wall/Wall.C 
planeWall/planeWall.C 
stlWall/stlWall.C
cylinderWall/cylinderWall.C
)
set(link_lib phasicFlow Geometry Kokkos::kokkos)

pFlow_make_executable_install(geometryPhasicFlow source_files link_lib)
