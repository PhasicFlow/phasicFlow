
set(SourceFiles 
domainDistribute/domainDistribute.cpp
DEMSystem/DEMSystem.cpp
sphereDEMSystem/sphereDEMSystem.cpp

sphereDEMSystem/thermalSphereDEMSystem.cpp
sphereDEMSystem/multiReactiveShrinkingDEMSystem.cpp

grainDEMSystem/grainFluidParticles.cpp
grainDEMSystem/grainDEMSystem.cpp
)

set(link_libs Kokkos::kokkos phasicFlow Particles Geometry Property Interaction Interaction Utilities)

pFlow_add_library_install(DEMSystems SourceFiles link_libs)



