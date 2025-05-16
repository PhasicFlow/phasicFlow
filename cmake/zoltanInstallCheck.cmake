# Macro to check for Zoltan installation and build it if needed
# Usage: zoltan_find_or_build(ZOLTAN_DIR)
# Returns: ZOLTAN_INCLUDE_DIR, ZOLTAN_LIBRARY

macro(zoltan_find_or_build ZOLTAN_DIR)
    # Set the Zoltan directory
    set(ZOLTAN_PREFIX "${ZOLTAN_DIR}" CACHE STRING "Zoltan install directory")
    message(STATUS "Zoltan install directory is ${ZOLTAN_PREFIX}")

    # Check if the Zoltan library is already built
    find_path(ZOLTAN_INCLUDE_DIR zoltan.h PATHS "${ZOLTAN_PREFIX}/include")
    message(STATUS "Zoltan include path: ${ZOLTAN_INCLUDE_DIR}")

    find_library(ZOLTAN_LIBRARY zoltan PATHS "${ZOLTAN_PREFIX}/lib")
    message(STATUS "Zoltan lib path: ${ZOLTAN_LIBRARY}")
    
    # Check if Zoltan library exists, if not compile it using buildlib script
    if(NOT ZOLTAN_LIBRARY)
        message(STATUS "Zoltan library not found. Compiling from source using buildlib script...")

        # Execute the buildlib bash script
        execute_process(
            COMMAND bash ${ZOLTAN_PREFIX}/buildlib
            WORKING_DIRECTORY ${ZOLTAN_PREFIX}
            RESULT_VARIABLE ZOLTAN_BUILD_RESULT
            OUTPUT_VARIABLE ZOLTAN_BUILD_OUTPUT
            ERROR_VARIABLE ZOLTAN_BUILD_ERROR
        )

        if(NOT ZOLTAN_BUILD_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to build Zoltan library using buildlib script. Error: ${ZOLTAN_BUILD_ERROR}")
        endif()

        # Try to find the library again after building
        find_library(ZOLTAN_LIBRARY zoltan PATHS "${ZOLTAN_PREFIX}/lib" NO_DEFAULT_PATH)
        find_path(ZOLTAN_INCLUDE_DIR zoltan.h PATHS "${ZOLTAN_PREFIX}/include" NO_DEFAULT_PATH)

        if(NOT ZOLTAN_LIBRARY)
            message(FATAL_ERROR "Failed to locate Zoltan library after building")
        endif()

        message(STATUS "Successfully built Zoltan library at ${ZOLTAN_LIBRARY}")
    endif()
endmacro()