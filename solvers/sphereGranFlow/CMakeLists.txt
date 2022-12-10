
set(source_files
sphereGranFlow.cpp
)
set(link_lib Kokkos::kokkos phasicFlow Particles Geometry Property Interaction Interaction Utilities)

pFlow_make_executable_install(sphereGranFlow source_files link_lib)
