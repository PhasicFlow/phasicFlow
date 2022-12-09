
set(source_files
iterateGeometry.cpp
)
set(link_lib phasicFlow Geometry MotionModel Kokkos::kokkos Utilities)

pFlow_make_executable_install(iterateGeometry source_files link_lib)
