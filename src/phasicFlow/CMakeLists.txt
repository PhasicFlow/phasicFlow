
list(APPEND SourceFiles 
types/basicTypes/bTypesFunctions.cpp
types/basicTypes/Logical.cpp
types/types.cpp

globals/error.cpp
globals/globalSettings.cpp

processors/processors.cpp
processors/localProcessors.cpp
processors/pFlowProcessors.cpp
processors/MPITimer.cpp


streams/token/tokenIO.cpp
streams/token/token.cpp
streams/iStream/IOstream.cpp
streams/iStream/iOstream.cpp
streams/iStream/iIstream.cpp
streams/Stream/Ostream.cpp
streams/Stream/Istream.cpp
streams/processorOstream/processorOstream.cpp
streams/masterOstream/masterOstream.cpp
streams/TStream/iTstream.cpp
streams/TStream/oTstream.cpp
streams/Fstream/iFstream.cpp
streams/Fstream/oFstream.cpp
streams/Fstream/fileStream.cpp
streams/dataIO/dataIORegulars.cpp
streams/streams.cpp 

fileSystem/fileSystem.cpp

dictionary/dictionary.cpp
dictionary/fileDictionary.cpp
dictionary/entry/iEntry.cpp
dictionary/entry/dataEntry.cpp 
dictionary/twoPartEntry/twoPartEntry.cpp

repository/IOobject/objectFile.cpp
repository/IOobject/IOfileHeader.cpp
repository/IOobject/IOobject.cpp
repository/IOobject/IOPattern.cpp
repository/repository/repository.cpp
repository/Time/Time.cpp
repository/Time/timeControl.cpp
repository/Time/baseTimeControl.cpp
repository/systemControl/systemControl.cpp
repository/systemControl/auxFunctions/auxFunctions.cpp
repository/systemControl/dynamicLinkLibs.cpp
repository/systemControl/timeFolder.cpp

commandLine/commandLine.cpp

eventManagement/subscriber.cpp
eventManagement/observer.cpp

demComponent/demComponent.cpp

Timer/Timer.cpp
Timer/Timers.cpp


containers/Vector/Vectors.cpp
containers/VectorHD/wordVectorHost.cpp
containers/VectorHD/VectorSingles.cpp
containers/Field/Fields.cpp
containers/symArrayHD/symArrays.cpp
containers/List/anyList/anyList.cpp

structuredData/zAxis/zAxis.cpp
structuredData/box/box.cpp
structuredData/sphere/sphere.cpp
structuredData/cylinder/cylinder.cpp
structuredData/peakableRegion/geometricRegion/geometricRegions.cpp
structuredData/peakableRegion/PeakableRegion/PeakableRegions.cpp
structuredData/peakableRegion/peakableRegion/peakableRegion.cpp

structuredData/pointStructure/internalPoints/internalPointsKernels.cpp
structuredData/pointStructure/internalPoints/internalPoints.cpp

structuredData/line/line.cpp
structuredData/infinitePlane/infinitePlane.cpp
structuredData/plane/plane.cpp
structuredData/domain/domain.cpp
structuredData/domain/simulationDomain.cpp
structuredData/domain/regularSimulationDomain.cpp
structuredData/boundaries/boundaryBase/boundaryBase.cpp
structuredData/boundaries/boundaryBase/boundaryBaseKernels.cpp
structuredData/boundaries/boundaryExit/boundaryExit.cpp
structuredData/boundaries/boundaryNone/boundaryNone.cpp
structuredData/boundaries/boundaryPeriodic/boundaryPeriodic.cpp
structuredData/boundaries/boundaryReflective/boundaryReflective.cpp
structuredData/boundaries/boundaryList.cpp
structuredData/pointStructure/pointStructure/pointStructure.cpp
structuredData/pointStructure/selectors/pStructSelector/pStructSelector.cpp
structuredData/pointStructure/selectors/selectorStridedRange/selectorStridedRange.cpp 
structuredData/pointStructure/selectors/selectorRandomPoints/selectorRandomPoints.cpp
structuredData/pointStructure/selectors/selectBox/selectBox.cpp
structuredData/pointStructure/selectors/selectorGeometric/selectorGeometrics.cpp
structuredData/pointStructure/pointStructure/pointSorting/mortonIndexing.cpp
structuredData/pointStructure/pointStructure/pointSorting/pointSorting.cpp

triSurface/subSurface.cpp
triSurface/triSurface.cpp
triSurface/multiTriSurface.cpp

containers/pointField/boundaryField/generalBoundary/generalBoundary.cpp
containers/pointField/pointField/pointFields.cpp
containers/triSurfaceField/triSurfaceFields.cpp

setFieldList/setFieldList.cpp
setFieldList/setFieldEntry.cpp

)

set(link_libs)
set(link_libs Kokkos::kokkos tbb)
	


if(pFlow_Build_MPI)

	set(Zoltan_Install_DIR)
	if(DEFINED ENV{Zoltan_DIR})
	   set(Zoltan_Install_DIR $ENV{Zoltan_DIR})
	else()
	  set(Zoltan_Install_DIR $ENV{HOME}/PhasicFlow/Zoltan)
	endif()
	message(STATUS "Zoltan install directory is ${Zoltan_Install_DIR}")

	set(ZOLTAN_PREFIX "${Zoltan_Install_DIR}" CACHE STRING "Zoltan install directory")

	find_path(ZOLTAN_INCLUDE_DIR zoltan.h PATHS "${ZOLTAN_PREFIX}/include")

	message(STATUS "Zoltan include path: ${ZOLTAN_INCLUDE_DIR}")

	find_library(ZOLTAN_LIBRARY zoltan PATHS "${ZOLTAN_PREFIX}/lib")
	message(STATUS "Zoltan lib path: ${ZOLTAN_LIBRARY}")
	
	list(APPEND SourceFiles
	MPIParallelization/domain/partitioning/partitioning.cpp 
	MPIParallelization/domain/partitioning/rcb1DPartitioning.cpp
	MPIParallelization/domain/MPISimulationDomain.cpp
    MPIParallelization/dataIOMPI/dataIOMPIs.cpp
    MPIParallelization/MPI/procCommunication.cpp
    MPIParallelization/MPI/scatteredMasterDistributeChar.cpp
	MPIParallelization/pointStructure/boundaries/boundaryProcessor.cpp
	MPIParallelization/pointField/processorBoundaryFields.cpp
	)
	
	list(APPEND link_libs MPI::MPI_CXX ${ZOLTAN_LIBRARY} -lm )
	pFlow_add_library_install(phasicFlow SourceFiles link_libs)
	target_include_directories(phasicFlow PUBLIC ./globals ${ZOLTAN_INCLUDE_DIR})

else()
	pFlow_add_library_install(phasicFlow SourceFiles link_libs)
	target_include_directories(phasicFlow PUBLIC ./globals)
endif()


