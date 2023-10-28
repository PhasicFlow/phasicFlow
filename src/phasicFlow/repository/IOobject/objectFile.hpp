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
#include "IOPattern.hpp"


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


protected:

	/// Name of the entity
	word 	name_;

	/// Read flag
	readFlag  rFlag_ = READ_NEVER;

	/// Write flag
	writeFlag wFlag_ = WRITE_NEVER;

	/// Local path of entity
	fileSystem localPath_ = "";

	/// Number of bytes used for writing/reading real variable (used for binray)
	int 		numBytesForReal_ = numBytesForReal__;
	
	IOPattern 	ioPattern_ = {IOPattern::MasterProcessor};

	/// Does the objectFile write the header or not
	bool readWriteHeader_ = true;

public:


	
	// constructors
	objectFile
	(
		const word& name
	);


	objectFile
	(
		const word& name,
		const fileSystem& 	localPath,
		const readFlag&   	rf = READ_NEVER,
		const writeFlag&  	wf = WRITE_NEVER,
		IOPattern::IOType	ioType = IOPattern::MasterProcessor,
		bool  rwHdr = true
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
		return rFlag_ == READ_ALWAYS;
	}

	inline
	bool isReadNever()const
	{
		return rFlag_ == READ_NEVER;
	}

	inline
	bool isReadIfPresent()const
	{
		return rFlag_ == READ_IF_PRESENT;
	}

	inline
	bool isWriteAlways()const
	{
		return wFlag_ == WRITE_ALWAYS;
	}

	inline
	bool isWriteNever()const
	{
		return wFlag_ == WRITE_NEVER;
	}

	const IOPattern& ioPattern()const
	{
		return ioPattern_;
	}

	inline 
	bool readWriteHeader()const
	{
		return readWriteHeader_;
	}

};



}

#endif //__objectFile_hpp__
