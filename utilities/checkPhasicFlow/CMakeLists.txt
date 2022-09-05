
set(source_files 
checkPhasicFlow.C
)
set(link_lib phasicFlow Kokkos::kokkos)

pFlow_make_executable_install(checkPhasicFlow source_files link_lib)
