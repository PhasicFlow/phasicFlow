
set(SourceFiles 
contactSearch/methods/cellBased/NBS/mapperNBS.cpp
contactSearch/methods/cellBased/NBS/mapperNBSKernels.cpp
contactSearch/methods/cellBased/NBS/NBSLevel0.cpp
contactSearch/methods/cellBased/NBS/NBS.cpp
contactSearch/methods/cellBased/NBS/cellsWallLevel0.cpp

contactSearch/boundaries/boundaryContactSearch/boundaryContactSearch.cpp
#contactSearch/boundaries/twoPartContactSearch/twoPartContactSearchKernels.cpp
#contactSearch/boundaries/twoPartContactSearch/twoPartContactSearch.cpp
contactSearch/boundaries/periodicBoundaryContactSearch/ppwBndryContactSearchKernels.cpp
contactSearch/boundaries/periodicBoundaryContactSearch/ppwBndryContactSearch.cpp
contactSearch/boundaries/periodicBoundaryContactSearch/wallBoundaryContactSearch.cpp
contactSearch/boundaries/periodicBoundaryContactSearch/periodicBoundaryContactSearch.cpp
contactSearch/boundaries/boundaryContactSearchList.cpp

contactSearch/contactSearch/contactSearch.cpp
contactSearch/ContactSearch/ContactSearchs.cpp

interaction/interaction.cpp
sphereInteraction/sphereInteractionsLinearModels.cpp
sphereInteraction/sphereInteractionsNonLinearModels.cpp
sphereInteraction/sphereInteractionsNonLinearModModels.cpp
)

if(pFlow_Build_MPI)
    list(APPEND SourceFiles 
    contactSearch/boundaries/processorBoundaryContactSearch/processorBoundaryContactSearch.cpp
    sphereInteraction/boundaries/processorBoundarySphereInteraction/processorBoundarySphereInteractions.cpp
    )
endif()

set(link_libs Kokkos::kokkos phasicFlow Property Particles Geometry)

pFlow_add_library_install(Interaction SourceFiles link_libs)

#additional locations 
target_include_directories(Interaction PUBLIC "./contactSearch")


