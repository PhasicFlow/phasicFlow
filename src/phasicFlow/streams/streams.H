#ifndef __streams_H__
#define __streams_H__

#include "Istream.H"

#include "Ostream.H"

#include "iFstream.H"

#include "oFstream.H"

#include "oTstream.H"

#include "iTstream.H"

namespace dFlow
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

#define Info dFlow::output<<boldChar<<magentaColor<<"> Info: "<<defaultColor<<magentaColor
#define endInfo defaultColor<<dFlow::nl

#define Report(n) dFlow::output.space(2*n)
#define endReport dFlow::nl

#define Warning dFlow::output<<boldChar<<yellowColor<<"> Warning\n"<<defaultColor<<yellowColor<<"  " 
#define endWarning defaultColor<<dFlow::nl

#define Err dFlow::output<<boldChar<<redColor<<"> Error\n"<<defaultColor<<redColor<<"  " 
#define endErr defaultColor<<dFlow::nl

#endif
