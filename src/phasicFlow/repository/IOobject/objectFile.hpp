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

#ifndef __objectFile_hpp__
#define __objectFile_hpp__

#include "types.hpp"
#include "fileSystem.hpp"

namespace pFlow
{

class objectFile
{
public:

	enum readFlag
	{
		READ_ALWAYS,
		READ_NEVER,
		READ_IF_PRESENT
	};

	enum writeFlag
	{
		WRITE_ALWAYS,
		WRITE_NEVER
	};


private:

	/// Name of the entity
	word 	name_;

	/// Read flag
	readFlag  rFlag_ = readFlag::READ_NEVER;

	/// Write flag
	writeFlag wFlag_ = writeFlag::WRITE_NEVER;

	/// Local path of entity
	fileSystem localPath_ = "";

	/// Number of bytes used for writing/reading real variable (used for binray)
	int 		numBytesForReal_ = numBytesForReal__;
	
	/// Does the objectFile read & write the header?
	bool        readWriteHeader_ = true;

public:


	
	// constructors
	explicit objectFile
	(
		const word& name
	);


	objectFile
	(
		const word& name,
		const fileSystem& 	localPath,
		const readFlag&   	rf = readFlag::READ_NEVER,
		const writeFlag&  	wf = writeFlag::WRITE_NEVER,
		bool  rwHeader = true
	);

	// copy construct 
	objectFile( const objectFile & src) = default;

	objectFile( objectFile&& src) = default;
	
	objectFile& operator = (const objectFile & rhs) = default;

	objectFile& operator = (objectFile && rhs) = default;


	virtual ~objectFile()=default;

	virtual word name() const
	{
		return name_;
	}

	virtual fileSystem localPath()const
	{
		return localPath_;
	}

	inline
	readFlag rFlag()const
	{
		return rFlag_;
	}

	inline
	writeFlag wFlag()const
	{
		return wFlag_;
	}

	inline
	bool isReadAlways()const
	{
		return rFlag_ == readFlag::READ_ALWAYS;
	}

	inline
	bool isReadNever()const
	{
		return rFlag_ == readFlag::READ_NEVER;
	}

	inline
	bool isReadIfPresent()const
	{
		return rFlag_ == readFlag::READ_IF_PRESENT;
	}

	inline
	bool isWriteAlways()const
	{
		return wFlag_ == writeFlag::WRITE_ALWAYS;
	}

	inline
	bool isWriteNever()const
	{
		return wFlag_ == writeFlag::WRITE_NEVER;
	}

	inline 
	bool readWriteHeader()const
	{
		return readWriteHeader_;
	}

};



}

#endif //__objectFile_hpp__
