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

#ifndef __fileSystem_H__
#define __fileSystem_H__

#include <filesystem>
#include "bTypes.H"
#include "List.H"


namespace pFlow
{


class iOstream;
class ostream;
class fileSystem;

iOstream& operator <<
(
	iOstream& os,
	fileSystem fs
);

std::ostream& operator <<
(
	std::ostream& os,
	fileSystem fs
);


fileSystem operator /
(
	const fileSystem& fs1,
    const fileSystem& fs2
 );

fileSystem operator +
(
	const fileSystem& fs1,
	const word fName
);

// a class to manage file/directory names 
class fileSystem
{
protected:
	
	std::filesystem::path path_;
	bool isDir_;


	// protected static variables
	inline static word notPermittedCharsFile = word(" ") + word("\t\n\0;:?*/<>\"?\'");
	
	// protected methods 

	bool static validFileName(const word& name)
	{
		return name.find_first_of(notPermittedCharsFile);
	}

	bool static checkFileName(const word& name);

public:

	inline static fileSystem CWD() 
	{
		return fileSystem(std::filesystem::current_path().c_str());
	}
	
public:


	typedef std::filesystem::path 	pathType;


	fileSystem():
		path_(),
		isDir_(true)
	{}
	// Constructors
	fileSystem( const word& dir, const word& file = "");
	fileSystem( const char* dir, const char* file = "");
	fileSystem( const pathType& path );

	fileSystem(const fileSystem&) = default;
	
	fileSystem(fileSystem&&) = default;

	fileSystem& operator = (const fileSystem&) = default;

	fileSystem& operator = (fileSystem&&) = default;

	~fileSystem() = default;
	
	// Methods
	bool isDir() const
	{
		return isDir_;
	}
	
	const pathType& path()const
	{
		return path_;
	}

	word wordPath()const
	{
		return word(path_.c_str());
	}

	// dir path of this
	fileSystem dirPath() const;

	// file name of this (if any)
	word fileName() const;

	// absolute path of this
	fileSystem absolute()const;


	//fileSystem relative()const;

	// canonical path of this (it should exist)
	fileSystem canonical()const;

	// only operate on dir path
	// check if the dir path exists
	bool dirExist()const;	

	// check if the path exists
	bool exist()const;

	// operate on dir path only
	// create dir based on the path and returns the canonical path
	fileSystem createDirs()const;


	// if this is dir path, add the filename to dir path
	// if it is file path, replace the file name 
	void operator += (const word& fileName);

	// it operates on dir path only
	// it adds the dir path of fs to this 
	void operator /=(const fileSystem& fs );

	// Create a dir path from dir path of fs1 and fas2 in the 
	// form of fs1/fs2 
	friend fileSystem operator /(const fileSystem& fs1, const fileSystem& fs2 );


	// return the dir path of this
	fileSystem operator()(bool retDir = true) const;
	

	friend iOstream& operator << (iOstream& os, fileSystem fs);

	friend std::ostream& operator << (std::ostream& os, fileSystem fs);
	
};


using fileSystemList = List<fileSystem>;



inline fileSystem CWD()
{
	return fileSystem::CWD();
}

bool isDirectory(const fileSystem& path);

bool isRegularFile(const fileSystem& path);

fileSystemList subDirectories(const fileSystem& path);

fileSystemList containingFiles(const fileSystem& path);

} // pFlow

#endif
