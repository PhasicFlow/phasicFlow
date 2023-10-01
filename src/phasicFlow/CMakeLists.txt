
list(APPEND SourceFiles 
types/basicTypes/bTypesFunctions.cpp
types/basicTypes/Logical.cpp
types/types.cpp



globals/error.cpp

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
streams/streams.cpp 

fileSystem/fileSystem.cpp

processors/processors.cpp

dictionary/dictionary.cpp
dictionary/entry/iEntry.cpp
dictionary/entry/dataEntry.cpp 
dictionary/twoPartEntry/twoPartEntry.cpp

)

set(link_libs)
if(MPI_FOUND)
	set(link_libs Kokkos::kokkos tbb PRIVATE MPI::MPI_CXX)
else()
	set(link_libs Kokkos::kokkos tbb)
endif()

pFlow_add_library_install(phasicFlow SourceFiles link_libs)
target_include_directories(phasicFlow PUBLIC ./globals)
