
set(SourceFiles 
contactSearch/contactSearch/contactSearch.C
contactSearch/ContactSearch/ContactSearchs.C
interaction/interaction.C
sphereInteraction/sphereInteractions.C
)

set(link_libs Kokkos::kokkos phasicFlow Property Particles Geometry)

pFlow_add_library_install(Interaction SourceFiles link_libs)

#additional locations 
target_include_directories(Interaction PUBLIC "./contactSearch")


