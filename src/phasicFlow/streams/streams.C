#include "streams.H"


dFlow::Ostream dFlow::output(std::cout, "dFlow Ostream");

dFlow::Istream dFlow::input( std::cin, "sFlow Istream"); 

dFlow::Ostream dFlow::errReport( std::cerr, "dFlow error report stream");
