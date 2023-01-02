
set(SourceFiles 
DEMSystem/DEMSystem.cpp
sphereDEMSystem/sphereDEMSystem.cpp
sphereDEMSystem/sphereFluidParticles.cpp
domainDistribute/domainDistribute.cpp
)

set(link_libs Kokkos::kokkos phasicFlow Particles Geometry Property Interaction Interaction Utilities)

pFlow_add_library_install(DEMSystems SourceFiles link_libs)



