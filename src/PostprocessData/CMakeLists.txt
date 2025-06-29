set(SourceFiles 
    # Main postprocess data
    postprocessData/postprocessData.cpp

    # Fields database
    fieldsDataBase/fieldsDataBase.cpp
    fieldsDataBase/simulationFieldsDataBase.cpp
  
    # Regions
    region/regionPoints/regionPoints/regionPoints.cpp
    region/regionPoints/sphereRegionPoints/sphereRegionPoints.cpp
    region/regionPoints/boxRegionPoints/boxRegionPoints.cpp
    region/regionPoints/lineRegionPoints/lineRegionPoints.cpp
    region/regionPoints/centerPointsRegionPoints/centerPointsRegionPoints.cpp
    region/regionPoints/multipleSpheresRegionPoints/multipleSpheresRegionPoints.cpp
    region/regionPoints/rectMeshRegionPoints/rectMeshRegionPoints.cpp

    # Postprocess components
    postprocessComponent/postprocessComponent/postprocessComponent.cpp
    postprocessComponent/particleProbePostprocessComponent/particleProbePostprocessComponent.cpp 
    postprocessComponent/PostprocessComponent/PostprocessComponents.cpp

    # Operations
    operation/postprocessOperation/postprocessOperation.cpp
    operation/PostprocessOperation/PostprocessOperationSum.cpp
    operation/PostprocessOperation/PostprocessOperationAverage.cpp
    operation/PostprocessOperation/PostprocessOperationAvMassVelocity.cpp
    
    operation/includeMask/includeMask.cpp
    operation/includeMask/IncludeMasks.cpp

    
)

set(link_libs Kokkos::kokkos phasicFlow Particles)

pFlow_add_library_install(PostprocessData SourceFiles link_libs)