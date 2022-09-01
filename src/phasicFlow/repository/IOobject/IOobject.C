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

#include "IOobject.H"
#include "repository.H"


pFlow::IOobject::IOobject
(
	const objectFile& objf,
	const repository* owner,
	uniquePtr<iObject>&& obj
)
:
	IOfileHeader(objf, owner),
	object_(obj.release())
{

	if(!read(this->readWriteHeader()))
	{
		fatalErrorInFunction<<
		"error in reading " << name() << " from path " << path()<<endl;
		fatalExit;
	}
}


pFlow::IOobject::IOobject
(
	const objectFile& objf,
	const repository* owner,
	uniquePtr<IOobject>&& obj
)
:
	IOfileHeader(objf, owner),
	object_( obj->object_.release())
{
}


bool pFlow::IOobject::isObjectValid()const
{
	return object_.get() != nullptr;
}

bool pFlow::IOobject::read(bool rdHdr)
{

	if( implyRead() )
	{
		if( auto ptrIS = inStream(); ptrIS )
		{	
			return read( ptrIS(), rdHdr);
		}
		else
		{
			warningInFunction<<
			"could not open file " << path() <<endl;
			return false;
		}
	}
	return true;
}


bool pFlow::IOobject::write() const
{
	if(implyWrite())
	{
		if(auto ptrOS = outStream(); ptrOS )
		{
			return write(ptrOS());
		}
		else
		{
			warningInFunction<< 
			"error in opening file "<< path() <<endl;
			return false;
		}
	}

	return true;
}


bool pFlow::IOobject::read(iIstream& is, bool rdHdr)
{
	if(rdHdr)
	{
		if(!readHeader(is))return false;
	}
	return object_->read_object_t(is);
}


bool pFlow::IOobject::write(iOstream& os) const
{
	if(this->readWriteHeader())
		writeHeader(os, typeName());
	
	return (object_->write_object_t(os) && writeSeparator(os));
}