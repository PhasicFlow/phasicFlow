
set(SourceFiles 
vtkFile.C
readFromTimeFolder.C
readControlDict.C
)

set(link_libs Kokkos::kokkos phasicFlow Particles Geometry)

pFlow_add_library_install(Utilities SourceFiles link_libs)




