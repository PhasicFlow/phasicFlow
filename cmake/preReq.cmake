if(pFlow_STD_Parallel_Alg)
    # Check if libtbb-dev is installed
    execute_process(
        COMMAND dpkg -s libtbb-dev
        RESULT_VARIABLE TBB_IS_INSTALLED
        OUTPUT_QUIET
        ERROR_QUIET)

    if(NOT TBB_IS_INSTALLED EQUAL 0)
        message(STATUS "libtbb-dev not found. Installing libtbb-dev...")
        execute_process(
            COMMAND sudo apt-get update
            COMMAND sudo apt-get install -y libtbb-dev
            RESULT_VARIABLE TBB_INSTALL_RESULT)
            
        if(NOT TBB_INSTALL_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to install libtbb-dev")
        endif()
    else()
        message(STATUS "libtbb-dev is already installed.")
    endif()
endif()


# Kokkos folder creation 
set(Kokkos_Source_DIR $ENV{HOME}/Kokkos/kokkos)

if(NOT EXISTS "${Kokkos_Source_DIR}/CMakeLists.txt")

    # Check CMake version and set policy CMP0169 if CMake version is 3.30 or higher
    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.30")
        cmake_policy(SET CMP0169 OLD)
    endif()

    include(FetchContent)
    FetchContent_Declare(
        kokkos
        GIT_REPOSITORY https://github.com/kokkos/kokkos.git
        GIT_TAG 4.3.01
    )
    
    FetchContent_GetProperties(kokkos)
    if(NOT kokkos_POPULATED)
        message(STATUS "Kokkos source directory not found. Downloading Kokkos version 4.3.01 ...")
        FetchContent_Populate(kokkos)
        set(Kokkos_Source_DIR ${kokkos_SOURCE_DIR})
    endif()

endif()

message(STATUS "Kokkos source directory is ${Kokkos_Source_DIR}")
add_subdirectory(${Kokkos_Source_DIR} ./kokkos)
#Kokkos_cmake_settings()

