
list(APPEND SourceFiles 
types/basicTypes/bTypesFunctions.cpp
types/basicTypes/Logical.cpp
types/types.cpp

processors/processors.cpp

globals/error.cpp)

set(link_libs Kokkos::kokkos tbb)

pFlow_add_library_install(phasicFlow SourceFiles link_libs)

target_include_directories(phasicFlow PUBLIC ./globals)

