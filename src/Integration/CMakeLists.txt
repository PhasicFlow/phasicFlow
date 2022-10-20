
list(APPEND SourceFiles 
integration/integration.C 
AdamsBashforth4/AdamsBashforth4.C
AdamsBashforth3/AdamsBashforth3.C
AdamsBashforth2/AdamsBashforth2.C
)

set(link_libs Kokkos::kokkos phasicFlow)

pFlow_add_library_install(Integration SourceFiles link_libs)



