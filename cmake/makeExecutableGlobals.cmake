#add a library to pFlow with source files and target_link_libs (thouse under the main CMakeLists.txt)
macro(pFlow_make_executable_install target_name source_files target_link_libs)

# add library 
add_executable(${target_name} ${${source_files}})

target_link_libraries(${target_name} PUBLIC ${${target_link_libs}})

#get all valid the source files under the current folder
file(GLOB_RECURSE allValidFilePaths RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${validFiles})

set(validFilePathsExcSRCs ${allValidFilePaths})
list(REMOVE_ITEM validFilePathsExcSRCs ${${source_files}})
set(includeDirs)
set(includeFiles ${validFilePathsExcSRCs})

# get the directory names 
foreach(file_path ${validFilePathsExcSRCs})
    GET_FILENAME_COMPONENT(dir_path ${file_path} DIRECTORY)
    list(APPEND includeDirs ${dir_path})
endforeach()

#remove duplicates 
list(REMOVE_DUPLICATES includeDirs)

target_include_directories(${target_name}
        PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}
	${includeDirs}
          )

message(STATUS "\nCreating make file for executable ${target_name}")
message(STATUS "  ${target_name} link libraries are: ${${target_link_libs}}")
message(STATUS "  ${target_name} source files are: ${${source_files}}")
message(STATUS "  ${target_name} include dirs are: ${includeDirs}\n")


install(TARGETS ${target_name} DESTINATION bin)


endmacro()


