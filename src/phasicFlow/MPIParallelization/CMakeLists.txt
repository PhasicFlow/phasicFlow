#add Zoltan
set(Zoltan_Install_DIR)
if(DEFINED ENV{Zoltan_DIR})
   set(Zoltan_Install_DIR $ENV{Zoltan_DIR})
else()
  set(Zoltan_Install_DIR $ENV{HOME}/PhasicFlow/Zoltan)
endif()
message(STATUS "Zoltan install directory is ${Zoltan_Install_DIR}")

set(ZOLTAN_PREFIX "${Zoltan_Install_DIR}" CACHE STRING "Zoltan install directory")

find_path(ZOLTAN_INCLUDE_DIR zoltan.h PATHS "${ZOLTAN_PREFIX}/include")

message(STATUS "Zoltan include path: ${ZOLTAN_INCLUDE_DIR}")

find_library(ZOLTAN_LIBRARY zoltan PATHS "${ZOLTAN_PREFIX}/lib")
message(STATUS "Zoltan lib path: ${ZOLTAN_LIBRARY}")


set(SourceFiles 
    partitioning.cpp 
    rcb1DPartitioning.cpp
    domain/MPISimulationDomain.cpp)

set(link_libs Kokkos::kokkos phasicFlow PRIVATE MPI::MPI_CXX ${ZOLTAN_LIBRARY} -lm )

pFlow_add_library_install(MPIParallelization SourceFiles link_libs)
target_include_directories(MPIParallelization PUBLIC ${ZOLTAN_INCLUDE_DIR})








