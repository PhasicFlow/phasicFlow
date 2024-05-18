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


#ifndef __stlFile_hpp__
#define __stlFile_hpp__

#include "types.hpp"
#include "Vectors.hpp"
#include "Lists.hpp"
#include "fileSystem.hpp"



namespace pFlow
{


class iIstream;

class stlFile
{
protected:
	
	//// - data memebrs

		// - list of verticies of all solids
		ListPtr<realx3x3Vector> solids_;

		// - list of names of all solids
		wordList 			  solidNames_;

		// - file name of stl file (used for reading and writing)
		mutable fileSystem file_;

	// - protected members

		bool readSolid(iIstream& is, realx3x3Vector & vertecies, word & name);

		bool readFacet(iIstream& is, realx3x3& tri);

		bool writeSolid(iOstream& os, const realx3x3Vector& vertecies, const word& name)const;

		bool writeFacet(iOstream& os, const realx3x3& tri)const;


public:

	//// - Constructors
		// - construct with file name
		//   an empty stlFile
		stlFile( fileSystem file );

		// - construct with file name and one solid (copy)
		stlFile( fileSystem file, const word& name, const realx3x3Vector& vertecies);

		// - construct with file name and one solid (copy)
		stlFile( fileSystem file, const word& name, realx3x3Vector&& vertecies);

		// - copy construct
		stlFile(const stlFile&) = default;

		// - move construct
		stlFile(stlFile&&) = default;

		~stlFile() = default;

	//// - Methods

		// - add a solid at the end of list, with copy operation 
		void addSolid(const word& name, const realx3x3Vector& vertecies);

		// - add a solid at the end of list, with move operation 
		void addSolid(const word& name, realx3x3Vector&& vertecies);

		// - clear current content and read from file
		bool read();

		// - write the current contnet to file
		bool write()const;

		// - set stl file path
		void setFile(fileSystem file) const;

		// - name of solids 
		const wordList& names()const;

		// - number of solids
		size_t size()const;
		
		// - vertecies of ith solid
		const realx3x3Vector& solid(size_t i)const;

		// - name of ith solid 
		const word& name(size_t i)const;

};


} // pFlow

#endif //__stlFile_hpp__
