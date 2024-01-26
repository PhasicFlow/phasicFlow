
set(SourceFiles 
dynamicPointStructure/dynamicPointStructure.cpp
particles/baseShapeNames.cpp
particles/shape.cpp
particles/particles.cpp
#particles/particleIdHandler.cpp
SphereParticles/sphereShape/sphereShape.cpp
SphereParticles/sphereParticles/sphereParticles.cpp
SphereParticles/sphereParticles/sphereParticlesKernels.cpp
#Insertion/shapeMixture/shapeMixture.cpp
#Insertion/insertion/insertion.cpp
#Insertion/Insertion/Insertions.cpp
#Insertion/insertionRegion/insertionRegion.cpp
#Insertion/insertionRegion/timeFlowControl.cpp
)

set(link_libs Kokkos::kokkos phasicFlow Integration Property)

pFlow_add_library_install(Particles SourceFiles link_libs)



