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

#ifndef __fileSystem_hpp__
#define __fileSystem_hpp__

#include <filesystem>
#include "bTypes.hpp"
#include "List.hpp"


namespace pFlow
{

// - Forward 
class iOstream;
class fileSystem;

iOstream& operator <<
(
	iOstream& os,
	const fileSystem& fs
);

std::ostream& operator <<
(
	std::ostream& os,
	const fileSystem& fs
);


fileSystem operator /
(
	const fileSystem& fs1,
    const fileSystem& fs2
);

fileSystem operator /
(
	const fileSystem& fs1,
    const word& dir2
);

fileSystem operator +
(
	const fileSystem& fs1,
	const word fName
);

/**
 * Manages file pathes, manupulate and combines them. 
 * 
 */
class fileSystem
{
public:
	
	using 	pathType = std::filesystem::path;

protected:
	
	/// File path 
	pathType 	path_;
	
	/// Is this a directory path?
	bool 		isDir_;


	/// Not premitted chars in file name 	
	inline static word notPermittedCharsFile = word(" ") + word("&\t\n;:?*/<>\"?\'");
	
	/// Is name is valid for a file? 
	bool static validFileName(const word& name)
	{
		return name.find_first_of(notPermittedCharsFile)==word::npos;
	}

	/// Is a valid file name?
	bool static checkFileName(const word& name);

public:

	/// return current working directory 
	inline static fileSystem CWD() 
	{
		return fileSystem(std::filesystem::current_path().c_str());
	}
	
public:

	//// - Constructors 

		/// Default
		fileSystem():
			path_(),
			isDir_(true)
		{}

		/// From full path 
		explicit
		fileSystem(const word & wPath);

		/// From dir and file name 
		fileSystem( const word& dir, const word& file);
		
		/// From dir and file name 
		fileSystem( const char* dir, const char* file="");

		/// Copy
		explicit
		fileSystem( const pathType& path );

		/// Copy 
		fileSystem(const fileSystem&) = default;
		
		/// Move 
		fileSystem(fileSystem&&) = default;

		/// Copy assignment 
		fileSystem& operator = (const fileSystem&) = default;

		/// Move assignment 
		fileSystem& operator = (fileSystem&&) = default;

		/// Destructor 
		~fileSystem() = default;
	
	//// - Methods
		
		/// Is directory?
		bool isDir() const
		{
			return isDir_;
		}
		
		/// Const access to path
		const pathType& path()const
		{
			return path_;
		}

		/// Path in word type 
		word wordPath()const
		{
			return word(path_.c_str());
		}

		/// Dir part of the path 
		fileSystem dirPath() const;

		/// File name part of the path (if any)
		word fileName() const;

		/// Absolute path of this
		fileSystem absolute()const;


		/// Canonical path of this (it should exist)
		fileSystem canonical()const;

		/// relative path of this this with respect to base
		fileSystem relative(const fileSystem& base)const;

		/// Only operate on dir path
		/// Check if the dir path exists
		bool dirExist()const;	

		/// Check if the path exists
		bool exist()const;

		/// Operate on dir path only
		/// Create dir based on the path and returns the canonical path
		fileSystem createDirs()const;


		/// If this is dir path, add the filename to dir path
		/// if it is file path, replace the file name 
		void operator += (const word& fileName);

		/// It operates on dir path only
		/// Add the dir path of fs to this 
		void operator /=(const fileSystem& fs );

		/// Create a dir path from dir path of fs1 and fas2 in the 
		/// form of fs1/fs2 
		friend fileSystem operator /(const fileSystem& fs1, const fileSystem& fs2 );


		/// Return the dir path of this
		fileSystem operator()(bool retDir = true) const;
	
	//// - IO
	friend iOstream& operator << (iOstream& os, const fileSystem& fs);

	friend std::ostream& operator << (std::ostream& os, const fileSystem& fs);
	
};


using fileSystemList = List<fileSystem>;


/// Free function to reture current working directory 
inline fileSystem CWD()
{
	return fileSystem::CWD();
}

/// Free function to check if the path is dir path
bool isDirectory(const fileSystem& path);

/// free function to check if the path is regular file 
bool isRegularFile(const fileSystem& path);

/// A list of sub-directories that exist in path. 
fileSystemList subDirectories(const fileSystem& path);

/// A list of file paths that exist in the path.
fileSystemList containingFiles(const fileSystem& path);

} // pFlow

#endif
