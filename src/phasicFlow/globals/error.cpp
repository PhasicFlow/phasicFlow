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


#include <Kokkos_Core.hpp>

#include "error.hpp"
#include "processors.hpp"
#include "streams.hpp"



//static pFlow::Ostream& errorStream   = pFlow::errReport;

static pFlow::Ostream& errorStream   = pFlow::pOutput;


pFlow::iOstream& fatalErrorMessage(const char* fileName, int linNumber )
{	
	
	errorStream<<"\n>>> Fatal error in phasicFlow\n" <<
				 "Error occured in source file "<< Red_Text(fileName) <<
				 " at line "<<Red_Text(linNumber)<<'\n';
	return errorStream;

}

pFlow::iOstream& fatalErrorInMessage(const char* fnName, const char* fileName, int linNumber )
{	

	errorStream<<"\n>>> Fatal error in phasicFlow\n" <<
	 			 " Error is issued in function " << Red_Text(fnName)<<
	 			 ", located in file "<< Red_Text(fileName) <<
	             " at line "<< Red_Text(linNumber) << '\n';
	return errorStream;
}

pFlow::iOstream& notImplementedErrorMessage(const char*fnName, const char* fileName, int lineNumber)
{

	errorStream<<"\n>>> Fatal error in phasicFlow\n";
	errorStream<<" Function "<< Red_Text(fnName) << " has not been implemented yet!\n" <<
				 " File "<< Yellow_Text(fileName) << 
	             " at line "<< Yellow_Text(lineNumber) <<'\n';
	return errorStream;
}


pFlow::iOstream& ioErrorMessage(const char* fileName, int fileLineNumber, const char* fnName, const char* fName, int lNumber)
{
	
	errorStream<<"\n>>> Fatal IO file error\n"<<
				 " IO error at number "<<Red_Text(fileLineNumber)<<
				 " of file " << Red_Text(fileName)<<'\n';
	errorStream<<" IO operation is peformed from function "<<Red_Text(fnName) <<
	             " in file "<< Red_Text(fName)<< " at line "<< Red_Text(lNumber) <<'\n';
	return errorStream;
}

pFlow::iOstream& ioErrorMessage(const pFlow::word& fileName, int fileLineNumber, const char* fnName, const char* fName, int lNumber)
{
	return ioErrorMessage( fileName.c_str(), fileLineNumber, fnName, fName, lNumber);
}


pFlow::iOstream& warningMessage(const char* fnName, const char* fileName, int linNumber )
{
	
	errorStream<<"\n>>> Warning in phasicFlow\n"<<
				 " Warning is issued in function " << Yellow_Text(fnName)<<
	             " in source file "<< Yellow_Text(fileName) <<
	             " at line "<< Yellow_Text(linNumber) <<'\n';
	return errorStream;
}

pFlow::iOstream& reportAndExit(int errorCode)
{
	errorStream<<"\n>>> phasicFlow is exiting . . ." << pFlow::endl;
	fatalExitPhasicFlow(errorCode);
	return errorStream;
}

int fatalExitPhasicFlow(int errorCode)
{
	// Kokkos should be finalized first
	Kokkos::finalize();

	pFlow::processors::abort(errorCode);
	return errorCode;
}
