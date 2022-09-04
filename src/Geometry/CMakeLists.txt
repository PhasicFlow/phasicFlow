
list(APPEND SourceFiles 
geometry/geometry.C
geometryMotion/geometryMotions.C
)

set(link_libs Kokkos::kokkos phasicFlow MotionModel Property)

pFlow_add_library_install(Geometry SourceFiles link_libs)



