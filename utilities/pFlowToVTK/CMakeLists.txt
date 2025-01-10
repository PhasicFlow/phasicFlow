
set(source_files 
fileSeries.cpp
pointFieldToVTK.cpp
triSurfaceFieldToVTK.cpp
pFlowToVTK.cpp 

#geometric.cpp 
)
set(link_lib phasicFlow Kokkos::kokkos Utilities)

pFlow_make_executable_install(pFlowToVTK source_files link_lib)
