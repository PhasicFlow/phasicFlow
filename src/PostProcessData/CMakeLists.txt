
set(SourceFiles 
fieldsDataBase/fieldsDataBase.cpp

postprocessComponent/postprocessComponent.cpp
postprocessComponent/PostprocessComponents.cpp

operation/postprocessOperation.cpp
operation/PostprocessOperationSum.cpp


postprocessData.cpp

postprocessComponent/postprocessComponent.cpp
postprocessComponent/PostprocessComponents.cpp
postprocessComponent/particleProbePostprocessComponent.cpp 

operation/includeMask/includeMask.cpp
operation/includeMask/IncludeMasks.cpp
operation/postprocessOperation.cpp
operation/PostprocessOperationSum.cpp

region/regionPoints/sphereRegionPoints.cpp
region/regionPoints/regionPoints.cpp
region/regionPoints/lineRegionPoints.cpp
region/regionPoints/centerPointsRegionPoints.cpp

)

set(link_libs Kokkos::kokkos phasicFlow Particles)

pFlow_add_library_install(PostProcessData SourceFiles link_libs)

add_subdirectory(testPostprocess)

