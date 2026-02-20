
set(source_files 
forceChainToVTK.cpp
)
set(link_lib phasicFlow Kokkos::kokkos)

pFlow_make_executable_install(forceChainToVTK source_files link_lib)
