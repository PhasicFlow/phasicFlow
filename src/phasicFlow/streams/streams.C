#include "streams.H"


pFlow::Ostream pFlow::output(std::cout, "pFlow Ostream");

pFlow::Istream pFlow::input( std::cin, "sFlow Istream"); 

pFlow::Ostream pFlow::errReport( std::cerr, "pFlow error report stream");
