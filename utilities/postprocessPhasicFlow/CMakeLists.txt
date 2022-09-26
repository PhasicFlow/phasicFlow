
set(source_files 
postprocessPhasicFlow.C 
postprocess.C
processField.C
ProcessFields.C
includeMask.C
IncludeMasks.C

)
set(link_lib phasicFlow Interaction Kokkos::kokkos Utilities)

pFlow_make_executable_install(postprocessPhasicFlow source_files link_lib)
