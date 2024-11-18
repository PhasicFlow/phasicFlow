
set(source_files 
cellMapper.cpp
rectangleMesh.cpp
countField.cpp
countFields.cpp
postprocess.cpp
processField.cpp
ProcessFields.cpp
includeMask.cpp
IncludeMasks.cpp
postprocessPhasicFlow.cpp
)
set(link_lib phasicFlow Interaction Kokkos::kokkos Utilities)

pFlow_make_executable_install(postprocessPhasicFlow source_files link_lib)
