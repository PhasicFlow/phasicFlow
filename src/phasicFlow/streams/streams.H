#ifndef __streams_H__
#define __streams_H__

#include "Istream.H"

#include "Ostream.H"

#include "iFstream.H"

#include "oFstream.H"

#include "oTstream.H"

#include "iTstream.H"

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
