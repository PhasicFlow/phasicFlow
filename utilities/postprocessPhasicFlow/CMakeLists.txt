
set(source_files

    postprocessPhasicFlow.cpp
    postSimulationFieldsDataBase/postSimulationFieldsDataBase.cpp
)
set(link_lib phasicFlow PostprocessData Kokkos::kokkos Utilities)

pFlow_make_executable_install(postprocessPhasicFlow source_files link_lib)
