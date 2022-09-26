
set(source_files 
pFlowToVTK.C 
geometric.C 
)
set(link_lib phasicFlow Kokkos::kokkos Utilities)

pFlow_make_executable_install(pFlowToVTK source_files link_lib)
