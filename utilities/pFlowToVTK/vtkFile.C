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


#include "vtkFile.H"

bool pFlow::vtkFile::openStream()
{
	oStream_ = makeUnique<oFstream>( fileName() );
	if( !oStream_ )return false;
	return writeHeader();
}

bool pFlow::vtkFile::vtkFile::writeHeader()
{

	if(!oStream_) return false;

	oStream_() << "# vtk DataFile Version 2.0" << endl;
	
	oStream_() << "vtk file for time : " << time_ << endl;

	oStream_() << "ASCII" << endl;

	if( oStream_().fail() ) return false;

	return true;

}

pFlow::vtkFile::vtkFile
(
	const fileSystem dir,
	const word& bName,
	real time
)
:
	dirPath_(dir),
	baseName_(bName),
	time_(time)
{

	if(!openStream())
	{
		fatalErrorInFunction <<
		"  error in creating vtkFile "<<fileName()<<endl;
		fatalExit;
	}

}


pFlow::fileSystem pFlow::vtkFile::fileName()const
{
	word fName = baseName_ +"-" + int322Word(10000*time_) + ".vtk";
	return dirPath_ +fName;
}

