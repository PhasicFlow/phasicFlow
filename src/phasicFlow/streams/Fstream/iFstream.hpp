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
#ifndef __iFstream_hpp__
#define __iFstream_hpp__


#include "fileSystem.hpp"
#include "fileStream.hpp"
#include "Istream.hpp"

namespace pFlow
{

/**
 * Input file stream for reading binary or ascii data from a file. 
 * 
 * It is based on OpenFOAM stream, with some modifications/simplifications
 * to be tailored to our needs.
 */
class iFstream
:
	public fileStream,
	public Istream
{
public:

	//// - Constructors

		/// From file path and format 
		iFstream (const fileSystem& path, bool binary = false);

		/// No copy constructor
		iFstream( const iFstream& src) = delete;

		/// No assignment
		iFstream& operator = (const iFstream& rhs) = delete;

		/// Destructor
		virtual ~iFstream() = default;

};

}


#endif
