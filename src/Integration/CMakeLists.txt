
list(APPEND SourceFiles 
integration/integration.cpp 
AdamsBashforth2/AdamsBashforth2.cpp
#AdamsBashforth5/AdamsBashforth5.cpp
#AdamsBashforth4/AdamsBashforth4.cpp
#AdamsBashforth3/AdamsBashforth3.cpp
#AdamsMoulton3/AdamsMoulton3.cpp
#AdamsMoulton4/AdamsMoulton4.cpp
#AdamsMoulton5/AdamsMoulton5.cpp
)

set(link_libs Kokkos::kokkos phasicFlow)

pFlow_add_library_install(Integration SourceFiles link_libs)



