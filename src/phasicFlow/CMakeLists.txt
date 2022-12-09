
list(APPEND SourceFiles 
types/basicTypes/bTypesFunctions.cpp
types/basicTypes/Logical.cpp
types/types.cpp

globals/error.cpp

streams/token/tokenIO.cpp
streams/token/token.cpp
streams/iStream/IOstream.cpp
streams/iStream/iIstream.cpp
streams/iStream/iOstream.cpp
streams/Stream/Istream.cpp
streams/Stream/Ostream.cpp
streams/Fstream/iFstream.cpp
streams/Fstream/oFstream.cpp
streams/Fstream/fileStream.cpp 
streams/TStream/iTstream.cpp
streams/TStream/oTstream.cpp
streams/streams.cpp 

dictionary/dictionary.cpp
dictionary/entry/iEntry.cpp
dictionary/entry/dataEntry.cpp 
dictionary/twoPartEntry/twoPartEntry.cpp

fileSystem/fileSystem.cpp

commandLine/commandLine.cpp

random/randomReal/randomReal.cpp
random/randomReal/randomReals.cpp

Timer/Timer.cpp
Timer/Timers.cpp

repository/Time/Time.cpp
repository/Time/timeControl.cpp
repository/systemControl/systemControl.cpp
repository/repository/repository.cpp
repository/IOobject/objectFile.cpp
repository/IOobject/IOobject.cpp
repository/IOobject/IOfileHeader.cpp

structuredData/box/box.cpp
structuredData/cylinder/cylinder.cpp
structuredData/sphere/sphere.cpp
structuredData/iBox/iBoxs.cpp
structuredData/line/line.cpp
structuredData/zAxis/zAxis.cpp
structuredData/pointStructure/pointStructure.cpp
structuredData/pointStructure/selectors/pStructSelector/pStructSelector.cpp
structuredData/pointStructure/selectors/selectBox/selectBox.cpp
structuredData/pointStructure/selectors/selectRange/selectRange.cpp 
structuredData/pointStructure/selectors/selectRandom/selectRandom.cpp
structuredData/trisurfaceStructure/triSurface.cpp
structuredData/trisurfaceStructure/multiTriSurface.cpp
structuredData/trisurfaceStructure/stlFile.cpp
structuredData/peakableRegion/sphereRegion/sphereRegion.cpp
structuredData/peakableRegion/cylinderRegion/cylinderRegion.cpp
structuredData/peakableRegion/boxRegion/boxRegion.cpp
structuredData/peakableRegion/peakableRegion/peakableRegion.cpp
structuredData/peakableRegion/peakableRegions.cpp

containers/Vector/Vectors.cpp
containers/Field/Fields.cpp
containers/symArrayHD/symArrays.cpp
containers/triSurfaceField/triSurfaceFields.cpp
containers/bitsetHD/bitsetHDs.cpp
containers/indexContainer/indexContainer.cpp

setFieldList/setFieldList.cpp
setFieldList/setFieldEntry.cpp

eventSubscriber/eventSubscriber.cpp
eventSubscriber/eventObserver.cpp)

set(link_libs Kokkos::kokkos tbb)

pFlow_add_library_install(phasicFlow SourceFiles link_libs)

target_include_directories(phasicFlow PUBLIC ./Kokkos ./algorithms ./globals)

