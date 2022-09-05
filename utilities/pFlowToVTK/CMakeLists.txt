
set(source_files 
pFlowToVTK.C 
vtkFile.C 
geometric.C 
)
set(link_lib phasicFlow Kokkos::kokkos)

pFlow_make_executable_install(pFlowToVTK source_files link_lib)
