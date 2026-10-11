set(source_files
multiSpeciesGranFlow.cpp
)
set(link_lib Kokkos::kokkos phasicFlow Particles Geometry Property Interaction Interaction Utilities DEMSystems)
pFlow_make_executable_install(multiSpeciesGranFlow source_files link_lib)
