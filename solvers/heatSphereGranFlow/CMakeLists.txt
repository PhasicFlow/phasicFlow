
set(source_files
heatSphereGranFlow.cpp
)
set(link_lib Kokkos::kokkos phasicFlow Particles Geometry Property Interaction Interaction Utilities)

pFlow_make_executable_install(heatSphereGranFlow source_files link_lib)
