#ifndef __streams_hpp__
#define __streams_hpp__

#include "Istream.hpp"

#include "Ostream.hpp"

#include "iFstream.hpp"

#include "oFstream.hpp"

#include "oTstream.hpp"

#include "iTstream.hpp"

namespace pFlow
{


	extern Ostream output;

	extern Istream input; 

	extern Ostream errReport;


}

#define redText(text)     	redColor<<text<<defaultColor
#define yellowText(text)  	yellowColor<<text<<defaultColor
#define blueText(text)    	blueColor<<text<<defaultColor
#define greenText(text)   	greenColor<<text<<defaultColor
#define magentaText(text) 	magentaColor<<text<<defaultColor
#define cyanText(text) 		cyanColor<<text<<defaultColor
#define boldText(text) 		boldChar<<text<<defaultColor

#define INFORMATION pFlow::output<<boldChar<<magentaColor<<"> INFO: "<<defaultColor<<magentaColor
#define endINFO defaultColor<<pFlow::nl

#define REPORT(n) pFlow::output.space(2*n)
#define endREPORT pFlow::nl


#define yWARNING pFlow::output<<boldChar<<yellowColor<<"> WARNING\n"<<defaultColor<<yellowColor<<"  " 
#define endyWARNING defaultColor<<pFlow::nl

#define ERR pFlow::output<<boldChar<<redColor<<"> ERROR\n"<<defaultColor<<redColor<<"  " 
#define endERR defaultColor<<pFlow::nl

#endif
