
set(source_files
correctionTablePhasicFlow.cpp
)
set(link_lib phasicFlow Kokkos::kokkos Property)

pFlow_make_executable_install(correctionTablePhasicFlow source_files link_lib)
