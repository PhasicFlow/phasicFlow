
list(APPEND SourceFiles 
types/basicTypes/bTypesFunctions.C
types/basicTypes/Logical.C
types/types.C

globals/error.C

streams/token/tokenIO.C
streams/token/token.C
streams/iStream/IOstream.C
streams/iStream/iIstream.C
streams/iStream/iOstream.C
streams/Stream/Istream.C
streams/Stream/Ostream.C
streams/Fstream/iFstream.C
streams/Fstream/oFstream.C
streams/Fstream/fileStream.C 
streams/TStream/iTstream.C
streams/TStream/oTstream.C
streams/streams.C 

dictionary/dictionary.C
dictionary/entry/iEntry.C
dictionary/entry/dataEntry.C 
dictionary/twoPartEntry/twoPartEntry.C

fileSystem/fileSystem.C

commandLine/commandLine.C

random/randomReal/randomReal.C
random/randomReal/randomReals.C

Timer/Timer.C
Timer/Timers.C

repository/Time/Time.C
repository/Time/timeControl.C
repository/systemControl/systemControl.C
repository/repository/repository.C
repository/IOobject/objectFile.C
repository/IOobject/IOobject.C
repository/IOobject/IOfileHeader.C

structuredData/box/box.C
structuredData/cylinder/cylinder.C
structuredData/sphere/sphere.C
structuredData/iBox/iBoxs.C
structuredData/line/line.C
structuredData/zAxis/zAxis.C
structuredData/pointStructure/pointStructure.C
structuredData/pointStructure/selectors/pStructSelector/pStructSelector.C
structuredData/pointStructure/selectors/boxAll/boxAll.C
structuredData/pointStructure/selectors/rangeAll/rangeAll.C 
structuredData/trisurfaceStructure/triSurface.C
structuredData/trisurfaceStructure/multiTriSurface.C
structuredData/trisurfaceStructure/stlFile.C
structuredData/peakableRegion/sphereRegion/sphereRegion.C
structuredData/peakableRegion/cylinderRegion/cylinderRegion.C
structuredData/peakableRegion/boxRegion/boxRegion.C
structuredData/peakableRegion/peakableRegion/peakableRegion.C
structuredData/peakableRegion/peakableRegions.C

containers/Vector/Vectors.C
containers/Field/Fields.C
containers/symArrayHD/symArrays.C
containers/triSurfaceField/triSurfaceFields.C
containers/bitsetHD/bitsetHDs.C
containers/indexContainer/indexContainer.C

setFieldList/setFieldList.C
setFieldList/setFieldEntry.C

eventSubscriber/eventSubscriber.C
eventSubscriber/eventObserver.C)

set(link_libs Kokkos::kokkos tbb)

pFlow_add_library_install(phasicFlow SourceFiles link_libs)

target_include_directories(phasicFlow PUBLIC ./Kokkos ./algorithms ./globals)

