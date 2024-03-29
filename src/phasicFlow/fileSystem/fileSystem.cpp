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


#include "fileSystem.hpp"
#include "error.hpp"
#include "streams.hpp"


bool pFlow::fileSystem::checkFileName(const word& name)
{

	if(!validFileName(name))
	{
		fatalErrorInFunction <<
		"Invalid file name supplied " << name <<
		"the following characters are not allowd: " <<
		notPermittedCharsFile << endl;
		fatalExit;
		return false;
	}

	return true;
}


/// From full path 
pFlow::fileSystem::fileSystem(const word & wPath)
:
	path_(wPath),
	isDir_(std::filesystem::is_directory(path_))
{
	if( !isDir_ && !checkFileName(fileName()))
	{
		fatalExit;
	}
}

pFlow::fileSystem::fileSystem( const word& dir, const word& file)
{
	isDir_ = file.empty();

	if( !isDir_) 
	{
		checkFileName(file);
	}

	try
	{
		path_ = pathType(dir)/file;	
	}
	catch (std::filesystem::filesystem_error & ec)
	{	
		fatalErrorInFunction<<
		"Invalid fileSystem input:" <<
		ec.what()<<endl;
		fatalExit;
	}	

}

pFlow::fileSystem::fileSystem( const char* dir, const char* file)
:
	fileSystem(word(dir),word(file))
{

}

pFlow::fileSystem::fileSystem( const pathType& path )
:
	path_(path),
	isDir_(std::filesystem::is_directory(path_))
{}

pFlow::fileSystem pFlow::fileSystem::dirPath() const
{

	if( isDir())
	{
		return *this;
	}
	else
	{
		return fileSystem(path_.parent_path().c_str());
	}

}

pFlow::word pFlow::fileSystem::fileName() const
{
	if( isDir())
		return word("");
	
	try
	{
		return word( path_.filename());
	}
	catch (std::filesystem::filesystem_error & ec)
	{
		fatalErrorInFunction<<
		ec.what()<<endl;
		fatalExit;
	}

	return word("");
}

pFlow::fileSystem pFlow::fileSystem::absolute
(
) const
{

	std::error_code ec;
	pathType abPath;	
	if( abPath = std::filesystem::absolute( path_, ec); ec )
	{
		fatalErrorInFunction << 
		"The absolute path of "<<  path_.c_str() <<" is invalid: "<<
		ec.message()<<endl;
		fatalExit;
	}

	fileSystem res;

	res.path_  = abPath;
	res.isDir_ = isDir_;

	return res;
	
}

pFlow::fileSystem pFlow::fileSystem::canonical
(
)const
{
	std::error_code ec;
	pathType cnPath;	
	if( cnPath = std::filesystem::canonical( path_, ec); ec )
	{
		fatalErrorInFunction << 
		"The canonical path of "<<  path_.c_str() <<" cannot be obtained: "<<
		ec.message()<<endl;
		fatalExit;
	}

	fileSystem res;

	res.path_  = cnPath;
	res.isDir_ = isDir_;

	return res;
}

bool pFlow::fileSystem::dirExist
(
) const
{
	if(!isDir())
	{
		fatalErrorInFunction<<
		"This function only operates on dir path, the path is "<<
		path_.c_str()<<endl;
		fatalExit;
		return false;
	}

	return exist();
	
}

bool pFlow::fileSystem::exist()const
{
	try
	{
		return std::filesystem::exists(path_);
	}
	catch (std::filesystem::filesystem_error & ec)
	{
		fatalErrorInFunction<<
		ec.what()<<endl;
		fatalExit;
		return false;
	}
}

pFlow::fileSystem pFlow::fileSystem::createDirs
(
) const
{
	if(! isDir())
	{
		fatalErrorInFunction<<
		"This function only operates on dir path, the path is "<<
		path_.c_str()<<endl;
		fatalExit;
	}	

	try
	{
		std::filesystem::create_directories(path_);
		return canonical();
	}
	catch (std::filesystem::filesystem_error & ec)
	{
		fatalErrorInFunction<<
		ec.what()<<endl;
		fatalExit;
		return *this;
	}
}


pFlow::fileSystem pFlow::fileSystem::operator()
(
	bool retDir 
) const
{
	if(retDir)
		return dirPath();
	else
		return *this;
}

void pFlow::fileSystem::operator += (const word& fileName)
{
	checkFileName(fileName);

	if( isDir())
	{
		path_ /= fileName;
		isDir_ = false;
	}
	else
	{
		path_ = path_.parent_path()/fileName;
	}
}


void pFlow::fileSystem::operator /=
(
	const fileSystem& fs
)
{
	if(!isDir())
	{
		fatalErrorInFunction<<
		"This operator should be used on dir path only"<<endl;
		fatalExit;
	}

	path_/= fs.dirPath().path_;
}

pFlow::fileSystem pFlow::operator /
(
	const fileSystem& fs1,
	const fileSystem& fs2 
)
{
	fileSystem::pathType cmbPath(fs1.dirPath().path_ / fs2.path_);

	return fileSystem( cmbPath.c_str() );

}

pFlow::fileSystem pFlow::operator /
(
	const fileSystem& fs1,
	const word& dir2 
)
{
	return fs1/fileSystem(dir2, "");
}

pFlow::fileSystem pFlow::operator +
(
	const fileSystem& fs1,
	const word fName
)
{
	fileSystem path = fs1;
	path+= fName;
	return path;
}

pFlow::iOstream& pFlow::operator << (iOstream& os, const fileSystem& fs)
{
	os << fs.path_.c_str();
	return os;
}

std::ostream& pFlow::operator << (std::ostream& os, const fileSystem& fs)
{
	os << fs.path_.c_str();
	return os;
}

bool pFlow::isDirectory
(
	const fileSystem& path
)
{
	return std::filesystem::is_directory(path.path());
}

bool pFlow::isRegularFile(const fileSystem& path)
{
	return std::filesystem::is_regular_file(path.path());
}

pFlow::fileSystemList pFlow::subDirectories
(
	const fileSystem& path
)
{
	fileSystemList dirs;

	if( isDirectory(path) )
	{
		auto dOps = std::filesystem::directory_options::skip_permission_denied;
		for( auto& subPath: std::filesystem::directory_iterator(path.path(), dOps) )
		{
			if(isDirectory( fileSystem(subPath.path()) ) )
			{
				dirs.emplace_back(subPath.path());
			}
		}
	}

	return dirs;
}

pFlow::fileSystemList pFlow::containingFiles
(
	const fileSystem& path
)
{
	fileSystemList files;

	if( isDirectory(path) )
	{
		auto dOps = std::filesystem::directory_options::skip_permission_denied;
		for( auto& subPath: std::filesystem::directory_iterator(path.path(), dOps) )
		{
			if( std::filesystem::is_regular_file(subPath.path()) )
			{
				files.emplace_back(subPath.path());
			}
		}

	}

	return files;
}