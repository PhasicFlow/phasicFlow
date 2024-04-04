cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# set the project name and version
project(phasicFlow VERSION 1.0 )

set(CMAKE_CXX_STANDARD 17 CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INSTALL_PREFIX ${phasicFlow_SOURCE_DIR} CACHE PATH "Install path of phasicFlow" FORCE)
set(CMAKE_BUILD_TYPE Relase CACHE STRING "build type" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build using shared libraries" FORCE)
mark_as_advanced(FORCE var BUILD_SHARED_LIBS)

message(STATUS ${CMAKE_INSTALL_PREFIX})

include(cmake/globals.cmake)

#Kokkos directory to be included 
set(Kokkos_Source_DIR)

if(DEFINED ENV{Kokkos_DIR})
   set(Kokkos_Source_DIR $ENV{Kokkos_DIR})
else()
  set(Kokkos_Source_DIR $ENV{HOME}/Kokkos/kokkos)
endif()
message(STATUS "Kokkos source directory is ${Kokkos_Source_DIR}")
add_subdirectory(${Kokkos_Source_DIR} ${phasicFlow_BINARY_DIR}/kokkos)
Kokkos_cmake_settings()


option(pFlow_STD_Parallel_Alg "Use TTB std parallel algorithms" ON)
option(pFlow_Build_Serial "Build phasicFlow and backends for serial execution" OFF)
option(pFlow_Build_OpenMP "Build phasicFlow and backends for OpenMP execution" OFF)
option(pFlow_Build_Cuda   "Build phasicFlow and backends for Cuda execution" OFF)
option(pFlow_Build_Double "Build phasicFlow with double precision floating-oint variables" ON)
option(pFlow_Build_MPI    "Build for MPI parallelization. This will enable multi-gpu run, CPU run on clusters (distributed memory machine). Use this combination Cuda+MPI, OpenMP + MPI or Serial+MPI " OFF)


if(pFlow_Build_Serial)
	set(Kokkos_ENABLE_SERIAL ON CACHE BOOL "Serial execution" FORCE) 
	set(Kokkos_ENABLE_OPENMP OFF CACHE BOOL "OpenMP execution" FORCE)
	set(Kokkos_ENABLE_CUDA OFF CACHE BOOL "Cuda execution" FORCE)
	set(Kokkos_ENABLE_CUDA_LAMBDA OFF CACHE BOOL "Cuda execution" FORCE)
	set(Kokkos_ENABLE_CUDA_CONSTEXPR OFF CACHE BOOL "Enable constexpr on cuda code" FORCE)
elseif(pFlow_Build_OpenMP )
	set(Kokkos_ENABLE_SERIAL ON CACHE BOOL "Serial execution" FORCE) 
	set(Kokkos_ENABLE_OPENMP ON CACHE BOOL "OpenMP execution" FORCE)
	set(Kokkos_ENABLE_CUDA OFF CACHE BOOL "Cuda execution" FORCE)
	set(Kokkos_ENABLE_CUDA_LAMBDA OFF CACHE BOOL "Cuda execution" FORCE)
	set(Kokkos_DEFAULT_HOST_PARALLEL_EXECUTION_SPACE SERIAL CACHE STRING "" FORCE)
	set(Kokkos_ENABLE_CUDA_CONSTEXPR OFF CACHE BOOL "Enable constexpr on cuda code" FORCE)
elseif(pFlow_Build_Cuda)
	set(Kokkos_ENABLE_SERIAL ON CACHE BOOL "Serial execution" FORCE) 
	set(Kokkos_ENABLE_OPENMP OFF CACHE BOOL "OpenMP execution" FORCE)
	set(Kokkos_ENABLE_CUDA ON CACHE BOOL "Cuda execution" FORCE)
	set(Kokkos_ENABLE_CUDA_LAMBDA ON CACHE BOOL "Cuda execution" FORCE)
	set(Kokkos_ENABLE_CUDA_CONSTEXPR ON CACHE BOOL "Enable constexpr on cuda code" FORCE)
endif()

if(pFlow_Build_MPI)
	find_package(MPI REQUIRED)
endif()

include(cmake/makeLibraryGlobals.cmake)
include(cmake/makeExecutableGlobals.cmake)

configure_file(phasicFlowConfig.H.in phasicFlowConfig.H)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#add a global include directory 
include_directories(src/setHelpers src/demComponent "${PROJECT_BINARY_DIR}")


add_subdirectory(src)

add_subdirectory(solvers)

add_subdirectory(utilities)

#add_subdirectory(DEMSystems)
add_subdirectory(testIO)

install(FILES "${PROJECT_BINARY_DIR}/phasicFlowConfig.H"
  DESTINATION include)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${phasicFlow_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${phasicFlow_VERSION_MINOR}")
include(CPack)

