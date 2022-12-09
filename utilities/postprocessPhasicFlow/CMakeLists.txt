
set(source_files 
postprocessPhasicFlow.cpp 
postprocess.cpp
processField.cpp
ProcessFields.cpp
includeMask.cpp
IncludeMasks.cpp

)
set(link_lib phasicFlow Interaction Kokkos::kokkos Utilities)

pFlow_make_executable_install(postprocessPhasicFlow source_files link_lib)
