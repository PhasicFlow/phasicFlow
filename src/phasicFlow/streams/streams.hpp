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

#define Info pFlow::output<<boldChar<<magentaColor<<"> Info: "<<defaultColor<<magentaColor
#define endInfo defaultColor<<pFlow::nl

#define Report(n) pFlow::output.space(2*n)
#define endReport pFlow::nl

#define Warning pFlow::output<<boldChar<<yellowColor<<"> Warning\n"<<defaultColor<<yellowColor<<"  " 
#define endWarning defaultColor<<pFlow::nl

#define Err pFlow::output<<boldChar<<redColor<<"> Error\n"<<defaultColor<<redColor<<"  " 
#define endErr defaultColor<<pFlow::nl

#endif
