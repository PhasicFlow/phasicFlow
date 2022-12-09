
set(SourceFiles 
contactSearch/contactSearch/contactSearch.cpp
contactSearch/ContactSearch/ContactSearchs.cpp
interaction/interaction.cpp
sphereInteraction/sphereInteractions.cpp
)

set(link_libs Kokkos::kokkos phasicFlow Property Particles Geometry)

pFlow_add_library_install(Interaction SourceFiles link_libs)

#additional locations 
target_include_directories(Interaction PUBLIC "./contactSearch")


