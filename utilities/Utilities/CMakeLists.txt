
set(SourceFiles 
readFromTimeFolder.cpp
#readControlDict.cpp
vtkFile/vtkFile.cpp
geometryPhasicFlow/Wall/Wall.cpp 
geometryPhasicFlow/planeWall/planeWall.cpp 
#geometryPhasicFlow/stlWall/stlWall.cpp
geometryPhasicFlow/cylinderWall/cylinderWall.cpp
geometryPhasicFlow/cuboidWall/cuboidWall.cpp
)

#set(link_libs Kokkos::kokkos phasicFlow Particles Geometry)

set(link_libs Kokkos::kokkos phasicFlow)

pFlow_add_library_install(Utilities SourceFiles link_libs)




