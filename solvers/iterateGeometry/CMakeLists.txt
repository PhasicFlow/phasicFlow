
set(source_files
iterateGeometry.C
)
set(link_lib phasicFlow Geometry MotionModel Kokkos::kokkos Utilities)

pFlow_make_executable_install(iterateGeometry source_files link_lib)
