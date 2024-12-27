#add a library to pFlow with source files and target_link_libs (thouse under the main CMakeLists.txt)
macro(pFlow_add_library_install target_name src_files target_link_libs)

set(source_files ${${src_files}})

# add library 
add_library(${target_name} ${source_files})

set_target_properties(${target_name} PROPERTIES
                      POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
                     )

target_link_libraries(${target_name} PUBLIC ${${target_link_libs}})

#get all valid the source files under the current folder
file(GLOB_RECURSE allValidFilePaths RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${validFiles})

set(validFilePathsExcSRCs ${allValidFilePaths})
foreach(file ${source_files})
	list(REMOVE_ITEM validFilePathsExcSRCs ${file})
endforeach()

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
        PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}
	${includeDirs}
          )

message(STATUS "\nCreating make file for library ${target_name}")
message(STATUS "  ${target_name} link libraries are: ${${target_link_libs}}")
#message(STATUS "  ${target_name} source files are: ${source_files}")
#message(STATUS "  ${target_name} include dirs are: ${includeDirs}\n \n")

install(TARGETS ${target_name} DESTINATION lib)
install(FILES ${includeFiles}  DESTINATION include/${target_name})

endmacro()


