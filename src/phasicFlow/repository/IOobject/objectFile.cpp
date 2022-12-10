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

#include "objectFile.hpp"

pFlow::objectFile::objectFile
(
	const word& name
)
:
	name_(name),
	rFlag_(READ_NEVER),
	wFlag_(WRITE_NEVER),
	localPath_("")
{}

pFlow::objectFile::objectFile
(
	const word& 			name,
	const fileSystem& 		localPath,
	const readFlag&   		rf, 
	const writeFlag&  		wf,
	bool  rwHeader
)
:
	name_(name),
	rFlag_(rf),
	wFlag_(wf),
	localPath_(localPath),
	readWriteHeader_(rwHeader)
{
}