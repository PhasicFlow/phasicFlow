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

#include "commandLine.hpp"


pFlow::commandLine::commandLine(word appName)
:
	CLI::App(description),
	appName_(appName)
{
	CLI::App::set_help_flag("-h,--help", "Help for using "+appName_+ " of "+versionName);
	CLI::App::set_version_flag("-v,--version", versionName, "Program version information");
}

pFlow::commandLine::commandLine(word appName, word disptn)
:
	commandLine(appName)
{
	
	CLI::App::add_flag_callback(
		"--description",
		[disptn, appName]() {
			mOutput<<"\n"<<Yellow_Text(versoinCopyright)<<endl;
			mOutput<<Yellow_Text(floatingPointDescription())<<endl<<endl;
			mOutput<<"Description for "<< Bold_Text(appName)<<":\n";
			mOutput<<"  "<<disptn<<endl;
		},
		"What does this app do?"
		)->configurable(false);
}

bool pFlow::commandLine::parse(
	int argc,
	char** argv)
{
	try {                       
        CLI::App::parse(argc, argv);
        if(CLI::App::count("--description")) return false;
        return true;
    } catch(const ParseError &e) {                                                                                
        CLI::App::exit(e);
        return false;
    }

    //if()
}
