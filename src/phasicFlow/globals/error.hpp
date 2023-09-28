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

#ifndef __error_hpp__
#define __error_hpp__


#include "builtinTypes.hpp"


//- Forward decleartions 
namespace pFlow
{
	class iOstream;	
}


//- Decleartions 

/// Take actions to fatal exit phasicFlow
int fatalExitPhasicFlow(int errorCode = EXIT_FAILURE);

pFlow::iOstream& fatalErrorMessage(const char* fileName, int linNumber );
pFlow::iOstream& fatalErrorInMessage(const char* fnName, const char* fileName, int linNumber );
pFlow::iOstream& notImplementedErrorMessage(const char*fnName, const char* fileName, int lineNumber);
pFlow::iOstream& ioErrorMessage(const pFlow::word& fileName, int fileLineNumber, const char* fnName, const char* fName, int lNumber);
pFlow::iOstream& ioErrorMessage(const char* fileName, int fileLineNumber, const char* fnName, const char* fName, int lNumber);
pFlow::iOstream& warningMessage(const char* fnName, const char* fileName, int linNumber );
pFlow::iOstream& reportAndExit(int errorCode = EXIT_FAILURE);


/// Report a fatal error and exit the applicaiton 
#define fatalError \
	fatalErrorMessage(__FILE__, __LINE__)

/// Report a fatal error and supplied function name and exit the application 
#define fatalErrorIn( functionName ) \
	fatalErrorInMessage((functionName), __FILE__, __LINE__ )

/// Report a fatal error and function name and exit the application 
#define fatalErrorInFunction fatalErrorIn(FUNCTION_NAME)

/// Report that a function is yet not implemented with supplied function name. 
#define Not_Implemented(functionName) \
    notImplementedErrorMessage ((functionName), __FILE__, __LINE__ )

/// Report that a function is yet not implemented. 
#define notImplementedFunction Not_Implemented(FUNCTION_NAME);

/// Report an error in file operation with supplied fileName and lineNumber. 
#define ioErrorInFile( fileName, lineNumber) \
	ioErrorMessage( fileName, lineNumber, FUNCTION_NAME, __FILE__, __LINE__ )

/// Report a warning with supplied function name
#define warningIn( functionName ) \
	warningMessage((functionName), __FILE__, __LINE__ )

/// Report a warning
#define warningInFunction warningIn(FUNCTION_NAME)

/// Fatal exit 
#define fatalExit \
	reportAndExit()

#endif
