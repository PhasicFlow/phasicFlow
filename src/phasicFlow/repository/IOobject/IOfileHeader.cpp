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
 
  phasicFlow is distribute+d to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/
#include "processors.hpp"
#include "IOfileHeader.hpp"
#include "repository.hpp"

pFlow::uniquePtr<pFlow::iFstream> pFlow::IOfileHeader::inStream()const
{
	if( fileExist() )
		return makeUnique<iFstream>(path(), inFileBinary());
	else
		return nullptr;
}

pFlow::uniquePtr<pFlow::oFstream> pFlow::IOfileHeader::outStream()const
{

	
	auto osPtr = makeUnique<oFstream>(path(), outFileBinary());
	
	if(osPtr && owner_)
	{
	 	auto outPrecision = owner_->outFilePrecision();
	 	osPtr->precision(outPrecision);
	}

	return osPtr;
}

pFlow::IOfileHeader::IOfileHeader
(
	const objectFile& objf,
	const repository* owner
)
:
	objectFile(objf),
	owner_(owner)
{}

pFlow::fileSystem pFlow::IOfileHeader::path() const
{
	fileSystem f;

	if( owner_ )
	{
		f = owner_->path()/localPath();	

	}else
	{
		f = localPath();
	}
	f += name_;
	return f;
}

bool pFlow::IOfileHeader::outFileBinary()const
{
	if(owner_)
		return owner_->outFileBinary();
	else
		return false;
}

bool pFlow::IOfileHeader::inFileBinary()const
{
	return toUpper(fileFormat_) == "BINARY";
}

bool pFlow::IOfileHeader::headerOk(bool silent)
{
	if(!fileExist())
	{
		if(!silent)
			warningInFunction <<
			"  the file "<< path() << " does not exist. \n";
		return false;
	}

	if( implyRead() )
	{
		
		if( auto ptrIS = inStream(); ptrIS )
		{
			return readHeader( ptrIS(), silent );
		}
		else
		{
			if(!silent)
				warningInFunction<<
				"could not open file " << path() <<endl;
			return false;
		}
	}

	return true;
}

bool pFlow::IOfileHeader::implyRead() const
{
	if (isReadAlways()) return true;
	return readIfPresent();
}

bool pFlow::IOfileHeader::implyWrite() const
{
	return isWriteAlways();
}

bool pFlow::IOfileHeader::fileExist() const
{
	return path().exist();
}

bool pFlow::IOfileHeader::readIfPresent()const
{
	return fileExist() && isReadIfPresent();
}

bool pFlow::IOfileHeader::writeHeader()const
{
	if( !this->readWriteHeader() ) return false;
	if( !processors::isMaster() ) return false;
	if( !implyWrite() ) return false;

	return true;
}

bool pFlow::IOfileHeader::writeHeader
(
	iOstream& os, 
	const word& typeName, 
	bool forceWrite
) const
{

	if(!forceWrite && !writeHeader()) return true;

	writeBanner(os);

	os.writeWordEntry("objectType", typeName );
	os.fatalCheck("writing objectType");

	os.writeWordEntry("objectName", name() );
	os.fatalCheck("writing objectName");

	word fileFormat;
	if(outFileBinary())
		fileFormat = "Binary";
	else
		fileFormat = "ASCII";

	os.writeWordEntry("fileFormat", fileFormat);
	os.fatalCheck("writing fileFormat");

	writeSeparator(os);
	return true;
}

bool pFlow::IOfileHeader::writeHeader(iOstream& os, bool forceWrite) const
{
	return writeHeader(os, objectType_, forceWrite);
}

bool pFlow::IOfileHeader::readHeader()const
{
	if( !this->readWriteHeader() ) return false;
	return true;
}

bool pFlow::IOfileHeader::readHeader(iIstream& is, bool silent)
{
	
	if(!readHeader()) return true;

	if( !is.findTokenAndNextSilent("objectName", objectName_) )
	{
		if(!silent)
		{
			warningInFunction<<
			"cannot find/error in reading objectName in file " <<
			is.name()<<endl;
		}
		return false;
	}	

	if( !is.findTokenAndNextSilent("objectType", objectType_) )
	{
		if(!silent)
		{
			warningInFunction <<
			"cannot find/error in reading objectType in file "<<
			is.name()<<endl;
		}
		return false;
	}


	if( !is.findTokenAndNextSilent("fileFormat", fileFormat_) )
	{
		if(!silent)
		{
			warningInFunction <<
			"cannot find/error in reading fileFormat in file "<<
			is.name()<<endl;
		}
		return false;
	}

	return true;
} 

bool pFlow::IOfileHeader::writeData()const
{
	if( processors::isMaster() || this->differentDataOnProcessors())
		return true;
	else
		return false;
}

bool pFlow::IOfileHeader::readData()const
{
	return true;
}

bool pFlow::IOfileHeader::writeBanner(iOstream& os)const
{
	os<< 
"/* -------------------------------*- C++ -*---------------------------------- *\\ \n"<<
"|  phasicFlow File                                                            | \n"<<
"|  copyright: www.cemf.ir                                                     | \n"<<
"\\* ------------------------------------------------------------------------- */  \n \n";

	return true;
}

bool pFlow::IOfileHeader::writeSeparator(iOstream& os)const
{
	os<< "\n" <<
"// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - // \n \n";

	return true;
}
