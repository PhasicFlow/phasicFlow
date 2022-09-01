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

#ifndef __objectFile_H__
#define __objectFile_H__

#include "types.H"
#include "fileSystem.H"


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

	// name of the entity
	word 	name_;

	// read flag
	readFlag  rFlag_;

	// write flag
	writeFlag wFlag_;

	// local path of entity
	fileSystem localPath_;

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
		const fileSystem& localPath,
		const readFlag&   rf = READ_NEVER,
		const writeFlag&  wf = WRITE_NEVER,
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

	readFlag rFlag()const
	{
		return rFlag_;
	}

	writeFlag wFlag()const
	{
		return wFlag_;
	}

	bool isReadAlways()const
	{
		return rFlag_ == READ_ALWAYS;
	}

	bool isReadNever()const
	{
		return rFlag_ == READ_NEVER;
	}

	bool isReadIfPresent()const
	{
		return rFlag_ == READ_IF_PRESENT;
	}

	bool isWriteAlways()const
	{
		return wFlag_ == WRITE_ALWAYS;
	}

	bool isWriteNever()const
	{
		return wFlag_ == WRITE_NEVER;
	}

	bool readWriteHeader()const
	{
		return readWriteHeader_;
	}

};



}

#endif //__objectFile_H__
