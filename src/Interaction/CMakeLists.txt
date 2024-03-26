
set(SourceFiles 
contactSearch/methods/cellBased/NBS/mapperNBS.cpp
contactSearch/methods/cellBased/NBS/mapperNBSKernels.cpp
contactSearch/methods/cellBased/NBS/NBSLevel0.cpp
contactSearch/methods/cellBased/NBS/NBS.cpp
contactSearch/methods/cellBased/NBS/cellsWallLevel0.cpp
contactSearch/contactSearch/contactSearch.cpp
contactSearch/boundaries/searchBoundary.cpp
contactSearch/ContactSearch/ContactSearchs.cpp
interaction/interaction.cpp
sphereInteraction/sphereInteractions.cpp
)

set(link_libs Kokkos::kokkos phasicFlow Property Particles Geometry)

pFlow_add_library_install(Interaction SourceFiles link_libs)

#additional locations 
target_include_directories(Interaction PUBLIC "./contactSearch")


