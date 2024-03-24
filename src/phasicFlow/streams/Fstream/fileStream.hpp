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


#ifndef __fileStream_hpp__
#define __fileStream_hpp__


#include <fstream>

#include "fileSystem.hpp"
#include "uniquePtr.hpp"

namespace pFlow
{

/**
 * Creates and manages an input/output file stream 
 * with specified format. 
 * 
 * It is based on OpenFOAM stream, with some modifications/simplifications
 * to be tailored to our needs.
 */
class fileStream
{
protected:

	/// in file stream 
	uniquePtr<std::ifstream> inStream_;
	
	/// out file stream
	uniquePtr<std::ofstream> outStream_;

	bool binary_ = false;
	
	/// open input file
	void openInFile(const fileSystem& path);
	
	/// open output file
	void openOutFile(const fileSystem& path);
	
	/// close streams 
	void close();

public:

	//// - Constructors

		/// From file path and input type and format. 
		fileStream( const fileSystem& path, bool outStream = false, bool binary = false);
		
		/// No copy
		fileStream(const fileStream&)= delete;

		/// No assignment
		fileStream& operator=(const fileStream&)=delete;

		/// Destructor
		virtual ~fileStream() 
		{
			close();
		}
	
	
	//// - Access 
	
		/// Access input file stream 
		std::ifstream& inStream();
		
		/// Access output file stream 
		std::ofstream& outStream();
};

}

#endif
