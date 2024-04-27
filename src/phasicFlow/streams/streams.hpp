#ifndef __streams_hpp__
#define __streams_hpp__

#include "Istream.hpp"

#include "Ostream.hpp"

#include "iFstream.hpp"

#include "oFstream.hpp"

#include "oTstream.hpp"

#include "iTstream.hpp"

#include "processorOstream.hpp"

#include "masterOstream.hpp"

namespace pFlow
{

	extern Ostream output;

	extern masterOstream mOutput;

	extern processorOstream pOutput;

	extern Istream input; 

	extern processorOstream errReport;

}


#define INFORMATION pFlow::pOutput<<boldChar<<magentaColor<<"> INFO: "<<defaultColor<<magentaColor
#define END_INFO defaultColor<<pFlow::endl

#define REPORT(n) pFlow::mOutput.space(2*n)
#define END_REPORT pFlow::endl


#define WARNING pFlow::errReport<<boldChar<<yellowColor<<"> WARNING\n"<<defaultColor<<yellowColor<<"  " 
#define END_WARNING defaultColor<<pFlow::endl

#define ERR pFlow::errReport<<boldChar<<redColor<<"> ERROR\n"<<defaultColor<<redColor<<"  " 
#define END_ERR defaultColor<<pFlow::endl

#endif