
set(source_files
grainGranFlow.cpp
)
set(link_lib Kokkos::kokkos phasicFlow Particles Geometry Property Interaction Interaction Utilities)

pFlow_make_executable_install(grainGranFlow source_files link_lib)
