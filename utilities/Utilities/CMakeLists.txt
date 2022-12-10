
set(SourceFiles 
vtkFile.cpp
readFromTimeFolder.cpp
readControlDict.cpp
)

set(link_libs Kokkos::kokkos phasicFlow Particles Geometry)

pFlow_add_library_install(Utilities SourceFiles link_libs)




