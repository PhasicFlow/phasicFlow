/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

#include <iostream>
#include <stdlib.h>

#include "error.hpp"
#include "streams.hpp"


static pFlow::Ostream& errorStream   = pFlow::errReport;


pFlow::iOstream& fatalErrorMessage(const char* fileName, int linNumber )
{	
	
	errorStream<<"\n>>> Fatal error in phasicFlow\n" <<
				 "Error occured in source file "<< redText(fileName) <<
				 " at line "<<redText(linNumber)<<'\n';
	return errorStream;

}

pFlow::iOstream& fatalErrorInMessage(const char* fnName, const char* fileName, int linNumber )
{	

	errorStream<<"\n>>> Fatal error in phasicFlow\n" <<
	 			 " Error is issued in function " << redText(fnName)<<
	 			 ", located in file "<< redText(fileName) <<
	             " at line "<< redText(linNumber) << '\n';
	return errorStream;
}

pFlow::iOstream& notImplementedErrorMessage(const char*fnName, const char* fileName, int lineNumber)
{

	errorStream<<"\n>>> Fatal error in phasicFlow\n";
	errorStream<<" Function "<< redText(fnName) << " has not implmented yet!\n" <<
				 " Function definition is in source file "<< redText(fileName) << 
	             " at line "<< redText(lineNumber) <<'\n';
	return errorStream;
}


pFlow::iOstream& ioErrorMessage(const char* fileName, int fileLineNumber, const char* fnName, const char* fName, int lNumber)
{
	
	errorStream<<"\n>>> Fatal IO file error\n"<<
				 " IO error at number "<<redText(fileLineNumber)<<
				 " of file " << redText(fileName)<<'\n';
	errorStream<<" IO operation is peformed from function "<<redText(fnName) <<
	             " in file "<< redText(fName)<< " at line "<< redText(lNumber) <<'\n';
	return errorStream;
}

pFlow::iOstream& ioErrorMessage(const pFlow::word& fileName, int fileLineNumber, const char* fnName, const char* fName, int lNumber)
{
	return ioErrorMessage( fileName.c_str(), fileLineNumber, fnName, fName, lNumber);
}


pFlow::iOstream& warningMessage(const char* fnName, const char* fileName, int linNumber )
{
	
	errorStream<<"\n>>> Warning in phasicFlow\n"<<
				 " Warning is issued in function " << yellowText(fnName)<<
	             " in source file "<< yellowText(fileName) <<
	             " at line "<< yellowText(linNumber) <<'\n';
	return errorStream;
}

pFlow::iOstream& reportAndExit()
{
	errorStream<<"\n>>> phasicFlow is exiting . . ." << pFlow::endl;
	exit(EXIT_FAILURE);
	return errorStream;
}


