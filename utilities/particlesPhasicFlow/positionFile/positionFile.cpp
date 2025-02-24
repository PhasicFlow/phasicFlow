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

#include "error.hpp"
#include "dictionary.hpp"
#include "positionFile.hpp"


#include "streams.hpp"
// #include "token.hpp"
#include "fileSystem.hpp"
#include "iFstream.hpp"
#include "oFstream.hpp"

bool pFlow::positionFile::positionPointsFile()
{
	REPORT(0) << "Reading user defined position file....";

	position_.clear();

	// Read position data from file.
	iFstream is(fileName_);

	realx3 tempPoint;

	token tok;

	while (!is.bad())
	{	
		// read position x

		is >> tok;
		if(tok.good()&& tok.isNumber()&& !is.eof())
		{
			tempPoint.x() = tok.realToken();
		}
		else
		{
			ioErrorInFile(is.name(), is.lineNumber())<< "Bad char or end of file in reading position x!\n";
			return false;
		}

		if(commaSeparated_)
		{
			is >> tok;
			if( !tok.isComma() || is.eof())
			{
				ioErrorInFile(is.name(), is.lineNumber())<< "Bad char or end of file in reading comma!\n";
				return false;
			}
		}

		// read position y
		is >> tok;
		if(tok.good()&& tok.isNumber()&& !is.eof())
		{
			tempPoint.y() = tok.realToken();
		}
		else
		{
			ioErrorInFile(is.name(), is.lineNumber())<< "Bad char or end of file in reading position y!\n";
			return false;
		}

		if(commaSeparated_)
		{
			is >> tok;
			if(!tok.isComma() || is.eof())
			{
				ioErrorInFile(is.name(), is.lineNumber())<< "Bad char or end of file in reading comma!\n";
				return false;
			}
		}
		
		// read position z
		is >> tok;
		if(tok.good()&& tok.isNumber()&& !is.eof())
		{
			tempPoint.z() = tok.realToken();
		}
		else
		{
			ioErrorInFile(is.name(), is.lineNumber())<< "Bad char or end of file in reading position z!\n";
			return false;
		}

		// insert position data to vector
		position_.push_back(tempPoint);

		is>>tok;
		if(is.eof()) break;

		is.putBack(tok);
	}

	REPORT(0) << "Done!" << END_REPORT;

	return true;
}

pFlow::positionFile::positionFile
(
	systemControl& control,
	const dictionary& dict
)
:
	positionParticles(control, dict),
	poDict_
	(  
		dict.subDict("fileInfo")
	),
	fileName_
	(
		poDict_.getVal<word>("name")
	),
	commaSeparated_
	(
		poDict_.getValOrSet("commaSeparated", Logical("Yes"))
	),
	position_
	(
		"position", 
		1, 
		0,
		RESERVE()
	)
{
	if(!positionPointsFile())
	{
		fatalExit;
	}
}