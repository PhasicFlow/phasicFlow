
set(source_files
particlesPhasicFlow.C
positionParticles/positionParticles.C 
positionOrdered/positionOrdered.C 
positionRandom/positionRandom.C
empty/empty.C
)
set(link_lib phasicFlow Kokkos::kokkos Interaction Utilities)

pFlow_make_executable_install(particlesPhasicFlow source_files link_lib)
