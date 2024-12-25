
set(SourceFiles 
domainDistribute/domainDistribute.cpp
DEMSystem/DEMSystem.cpp
sphereDEMSystem/sphereFluidParticles.cpp
#sphereDEMSystem/sphereDEMSystem.cpp
#

)

set(link_libs Kokkos::kokkos phasicFlow Particles Geometry Property Interaction Interaction Utilities)

pFlow_add_library_install(DEMSystems SourceFiles link_libs)



