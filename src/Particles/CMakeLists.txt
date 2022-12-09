
set(SourceFiles 
dynamicPointStructure/dynamicPointStructure.cpp
particles/particles.cpp
SphereParticles/sphereShape/sphereShape.cpp
SphereParticles/sphereParticles/sphereParticles.cpp
Insertion/shapeMixture/shapeMixture.cpp
Insertion/insertion/insertion.cpp
Insertion/Insertion/Insertions.cpp
Insertion/insertionRegion/insertionRegion.cpp
Insertion/insertionRegion/timeFlowControl.cpp
)

set(link_libs Kokkos::kokkos phasicFlow Integration Property)

pFlow_add_library_install(Particles SourceFiles link_libs)



