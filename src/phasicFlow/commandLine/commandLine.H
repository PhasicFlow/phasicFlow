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


#ifndef __commandLine_H__
#define __commandLine_H__

#include <any>
#include "CLI.hpp"

#include "types.H"
#include "Lists.H"
#include "streams.H"

namespace pFlow
{


class commandLine
:
	public CLI::App
{
public:
	
	using Option = CLI::Option;

	using ParseError = CLI::ParseError;

	using any = std::any;

protected:
	
	word appName_;

	List<any> 	internalBinVars_;

	inline static  word versionStr = "v-"+
					int322Word(phasicFlow_VERSION_MAJOR)+
					"."+
					int322Word(phasicFlow_VERSION_MINOR);
	inline static  	word copyRight = "(copyright(C): www.cemf.ir)";

	inline static 	word versionName = "phasicFlow "+ versionStr;

	inline static   word versoinCopyright = versionName + " " +copyRight;

	inline static 	word description = "Command line help for "+
										versoinCopyright +"\n";
	
	

public:

	commandLine(word appName);

	commandLine(word appName, word disptn);
	

	bool parse(int argc, char** argv);

	template<typename Type>
	Option* addOption(
		word optionName,
		word description,
		word opText = "");

	template<typename Type>
	Option* addOption(
		word optionName,
		Type& bindingVar,
		word description,
		word opText = "");



};

}// pFlow


template<typename Type>
pFlow::commandLine::Option*
	pFlow::commandLine::addOption(
		word optionName,
		word description,
		word opText)
{
	try{

		Type& bindingVar = std::any_cast<Type&>(
			internalBinVars_.emplace_back( any(Type()))
				);
		return CLI::App::add_option(optionName, bindingVar, description)->option_text(opText);		
	} 
	catch (ParseError& err){
		fatalExit;
		return nullptr;
	}
}


template<typename Type>
pFlow::commandLine::Option*
	pFlow::commandLine::addOption(
		word optionName,
		Type& bindingVar,
		word description,
		word opText)
{
	try{

		return CLI::App::add_option(optionName, bindingVar, description)->option_text(opText);		
	} 
	catch (ParseError& err){
		fatalExit;
		return nullptr;
	}
}



#endif 

