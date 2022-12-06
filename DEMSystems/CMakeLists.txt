
set(SourceFiles 
sphereDEMSystem/sphereDEMSystem.C
)

set(link_libs Kokkos::kokkos phasicFlow Particles Geometry Property Interaction Interaction Utilities)

pFlow_add_library_install(DEMSystems SourceFiles link_libs)



