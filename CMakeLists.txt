cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# set the project name and version
project(phasicFlow VERSION 0.1 )

set(CMAKE_CXX_STANDARD 17 CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INSTALL_PREFIX ${phasicFlow_SOURCE_DIR} CACHE PATH "Install path of phasicFlow" FORCE)
set(CMAKE_BUILD_TYPE Release CACHE STRING "build type" FORCE)

message(STATUS ${CMAKE_INSTALL_PREFIX})

mark_as_advanced(FORCE var Kokkos_ENABLE_CUDA_LAMBDA)
mark_as_advanced(FORCE var Kokkos_ENABLE_OPENMP)
mark_as_advanced(FORCE var Kokkos_ENABLE_SERIAL)
mark_as_advanced(FORCE var Kokkos_ENABLE_CUDA_LAMBDA)
mark_as_advanced(FORCE var BUILD_SHARED_LIBS)

option(USE_STD_PARALLEL_ALG "Use TTB std parallel algorithms" ON)
option(pFlow_Build_Serial "Build phasicFlow and backends for serial execution" OFF)
option(pFlow_Build_OpenMP "Build phasicFlow and backends for OpenMP execution" OFF)
option(pFlow_Build_Cuda   "Build phasicFlow and backends for Cuda execution" OFF)
option(pFlow_Build_Double "Build phasicFlow with double precision variables" ON)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build using shared libraries" FORCE)

if(pFlow_Build_Serial)
	set(Kokkos_ENABLE_SERIAL ON CACHE BOOL "Serial execution" FORCE) 
	set(Kokkos_ENABLE_OPENMP OFF CACHE BOOL "OpenMP execution" FORCE)
	set(Kokkos_ENABLE_CUDA OFF CACHE BOOL "Cuda execution" FORCE)
	set(Kokkos_ENABLE_CUDA_LAMBDA OFF CACHE BOOL "Cuda execution" FORCE)
elseif(pFlow_Build_OpenMP )
	set(Kokkos_ENABLE_SERIAL ON CACHE BOOL "Serial execution" FORCE) 
	set(Kokkos_ENABLE_OPENMP ON CACHE BOOL "OpenMP execution" FORCE)
	set(Kokkos_ENABLE_CUDA OFF CACHE BOOL "Cuda execution" FORCE)
	set(Kokkos_ENABLE_CUDA_LAMBDA OFF CACHE BOOL "Cuda execution" FORCE)
	set(Kokkos_DEFAULT_HOST_PARALLEL_EXECUTION_SPACE SERIAL CACHE STRING "" FORCE)
elseif(pFlow_Build_Cuda)
	set(Kokkos_ENABLE_SERIAL ON CACHE BOOL "Serial execution" FORCE) 
	set(Kokkos_ENABLE_OPENMP OFF CACHE BOOL "OpenMP execution" FORCE)
	set(Kokkos_ENABLE_CUDA ON CACHE BOOL "Cuda execution" FORCE)
	set(Kokkos_ENABLE_CUDA_LAMBDA ON CACHE BOOL "Cuda execution" FORCE)
endif()



include(cmake/globals.cmake)
message(STATUS "Valid file extensions are ${validFiles}")

include(cmake/makeLibraryGlobals.cmake)
include(cmake/makeExecutableGlobals.cmake)

configure_file(phasicFlowConfig.H.in phasicFlowConfig.H)

#add a global include directory 
include_directories(src/setHelpers src/demComponent "${PROJECT_BINARY_DIR}")

#main subdirectories of the code 
set(Kokkos_Source_DIR)

if(DEFINED ENV{Kokkos_DIR})
   set(Kokkos_Source_DIR $ENV{Kokkos_DIR})
#  add_subdirectory($ENV{Kokkos_DIR} ${phasicFlow_BINARY_DIR}/kokkos)
#  message(STATUS "Kokkos directory is $ENV{Kokkos_DIR}")
else()
#  add_subdirectory($ENV{HOME}/Kokkos/kokkos ${phasicFlow_BINARY_DIR}/kokkos)
  set(Kokkos_Source_DIR $ENV{HOME}/Kokkos/kokkos)
endif()

message(STATUS "Kokkos source directory is ${Kokkos_Source_DIR}")
add_subdirectory(${Kokkos_Source_DIR} ${phasicFlow_BINARY_DIR}/kokkos)

add_subdirectory(src)

add_subdirectory(solvers)

add_subdirectory(utilities)

add_subdirectory(DEMSystems)
#add_subdirectory(testIO)


install(FILES "${PROJECT_BINARY_DIR}/phasicFlowConfig.H"
  DESTINATION include
)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${phasicFlow_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${phasicFlow_VERSION_MINOR}")
include(CPack)

