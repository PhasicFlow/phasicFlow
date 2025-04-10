
set(SourceFiles 
dynamicPointStructure/dynamicPointStructure.cpp
particles/shape/baseShapeNames.cpp
particles/shape/shape.cpp
particles/particles.cpp
particles/particleIdHandler/particleIdHandler.cpp
particles/regularParticleIdHandler/regularParticleIdHandler.cpp

globalDamping/globalDamping.cpp

SphereParticles/sphereShape/sphereShape.cpp
SphereParticles/sphereParticles/sphereParticles.cpp
SphereParticles/sphereParticles/sphereParticlesKernels.cpp

GrainParticles/grainShape/grainShape.cpp
GrainParticles/grainParticles/grainParticles.cpp
GrainParticles/grainParticles/grainParticlesKernels.cpp

SphereParticles/boundarySphereParticles.cpp
SphereParticles/boundarySphereParticlesList.cpp

GrainParticles/boundaryGrainParticles.cpp
GrainParticles/boundaryGrainParticlesList.cpp

Insertion/collisionCheck/collisionCheck.cpp
Insertion/insertionRegion/insertionRegion.cpp
Insertion/insertion/insertion.cpp
Insertion/shapeMixture/shapeMixture.cpp
Insertion/Insertion/Insertions.cpp
)

if(pFlow_Build_MPI)
    list(APPEND SourceFiles 
    particles/MPIParticleIdHandler/MPIParticleIdHandler.cpp
    SphereParticles/processorBoundarySphereParticles.cpp)
endif()

set(link_libs Kokkos::kokkos phasicFlow Integration Property)

pFlow_add_library_install(Particles SourceFiles link_libs)



