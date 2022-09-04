
set(SourceFiles 
dynamicPointStructure/dynamicPointStructure.C
particles/particles.C
SphereParticles/sphereShape/sphereShape.C
SphereParticles/sphereParticles/sphereParticles.C
Insertion/shapeMixture/shapeMixture.C
Insertion/insertion/insertion.C
Insertion/Insertion/Insertions.C
Insertion/insertionRegion/insertionRegion.C
Insertion/insertionRegion/timeFlowControl.C
)

set(link_libs Kokkos::kokkos phasicFlow Integration Property)

pFlow_add_library_install(Particles SourceFiles link_libs)



