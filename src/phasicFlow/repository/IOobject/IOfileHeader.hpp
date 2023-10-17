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


#ifndef __IOfileHeader_hpp__
#define __IOfileHeader_hpp__


#include "uniquePtr.hpp"
#include "objectFile.hpp"
#include "streams.hpp"

namespace pFlow
{

class repository;

class IOfileHeader
:
	public objectFile
{
protected:
	

	//// - data members 
		// owner repository
		const repository* 	owner_ = nullptr;
		
		// object name read from file 
		word objectName_;

		// object type read from file 
		word objectType_;

		/// file format read from file 
		word fileFormat_ = "ASCII";

	//// - methods 

		// - input file stream
		uniquePtr<iFstream> inStream()const;

		// - ouput file stream
		uniquePtr<oFstream> outStream()const;

public:

	// with owner 
	IOfileHeader(const objectFile& objf, const repository* owner = nullptr);

	// - object name
	const word& objectName()const
	{
		return objectName_;
	}

	// - object type
	const word& objectType()const
	{
		return objectType_;
	}

	// - pointer to owner repository 
	const repository* owner()const 
	{
		return owner_;
	}

	// - path to file name 
	fileSystem path() const;

	bool outFileBinary()const;

	bool inFileBinary()const;

	// - should be used for read operations 
	//   check if the file exist,
	//   read the header of the file to check if it is ok
	bool headerOk(bool silent = false);
	
	/// Imply read 
	bool implyRead() const;

	/// Imply write 
	bool implyWrite() const;
	
	/// Check if file exists
	bool fileExist() const;
	
	/// Check read if present 
	bool readIfPresent()const;

	/// Check if the header should be written to file
	/// True: on master + implyWrite + readWriteHeader = true
	/// False: slave or NOT implyWrite
	bool writeHeader()const;

	/// Write the header to the file , typeName comes from caller
	bool writeHeader(iOstream& os, const word& typeName, bool forceWrite = false ) const;

	/// Write the header to the file, typeName comes from the one read from file 
	bool writeHeader(iOstream& os, bool forceWrite = false) const;

	/// Check if the data should be written to file 
	/// True: on master or differentDataOnProcessor is true
	/// False: otherwise 
	bool writeData()const;

	/// Check if header should be read from file
	/// True: All processors, read the file header 
	/// False: readWriteHeader = false
	bool readHeader()const;

	/// Read the header in the file
	bool readHeader(iIstream& is, bool silent=false);

	/// Check if data should be read from file 
	/// Always return true
	bool readData()const;
	
	// - write the banner 
	bool writeBanner(iOstream& os)const;

	// - wirte a separator line 
	bool writeSeparator(iOstream& os)const;

};


}

#endif //__objectFile_hpp__
