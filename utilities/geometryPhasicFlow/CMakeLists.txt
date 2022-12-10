
set(source_files 
geometryPhasicFlow.cpp
Wall/Wall.cpp 
planeWall/planeWall.cpp 
stlWall/stlWall.cpp
cylinderWall/cylinderWall.cpp
cuboidWall/cuboidWall.cpp
)
set(link_lib phasicFlow Geometry Kokkos::kokkos Utilities)

pFlow_make_executable_install(geometryPhasicFlow source_files link_lib)
