#include "streams.hpp"


pFlow::Ostream pFlow::output(std::cout, "pFlow Ostream");

pFlow::masterOstream pFlow::mOutput(cout, "pFlow masterOstream");

pFlow::processorOstream pFlow::pOutput(cout, "pFlow processorOstream");

pFlow::Istream pFlow::input( std::cin, "sFlow Istream"); 

pFlow::processorOstream pFlow::errReport( std::cerr, "pFlow error report stream");

