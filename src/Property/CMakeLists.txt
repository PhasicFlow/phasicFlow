
set(SourceFiles 
property.cpp
)

set(link_libs Kokkos::kokkos phasicFlow)

pFlow_add_library_install(Property SourceFiles link_libs)



