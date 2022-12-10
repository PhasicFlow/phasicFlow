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
// based on OpenFOAM stream, with some modifications/simplifications
// to be tailored to our needs


#include "fileStream.hpp"
#include "error.hpp"



void pFlow::fileStream::openInFile
(
	const fileSystem& path
)
{

	if( !path.exist() ) 
	{
		fatalErrorInFunction <<
		"File " << path << " does not exist for opening. \n";
		fatalExit;
	}

	inStream_ = makeUnique<std::ifstream>( path.wordPath(), std::ios_base::in);

	if( !inStream_->is_open())
	{

		fatalErrorInFunction <<
		"File " << path << " cannot be opened. \n";
		fatalExit;

	}
}


void pFlow::fileStream::openOutFile
(
	const fileSystem& path
)
{

	// - check if the Dir exists
	auto dir = path.dirPath();
	
	if(!dir.exist())
	{
		dir.createDirs();
	}
	
	outStream_ = makeUnique< std::ofstream>(path.wordPath(), std::ios_base::out);

	if(!outStream_->is_open())
	{
		fatalErrorInFunction <<
		"File " << path << " cannot be opened. \n";
		fatalExit;
	}
}

void pFlow::fileStream::close()
{
	if(inStream_)
	{
		inStream_.reset(nullptr);
	}

	if(outStream_)
	{
		outStream_.reset(nullptr);
	}
}

pFlow::fileStream::fileStream
(
	const fileSystem& path,
	bool outStream
)
:
	inStream_(nullptr),
	outStream_(nullptr)
{

	if(outStream)
	{
		openOutFile(path);
	}else
	{
		openInFile(path);
	}

}

std::ifstream& pFlow::fileStream::inStream()
{
	return inStream_();
}

std::ofstream& pFlow::fileStream::outStream()
{
	return outStream_();
}



