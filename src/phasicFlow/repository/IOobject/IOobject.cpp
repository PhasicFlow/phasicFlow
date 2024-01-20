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

#include "IOobject.hpp"
#include "repository.hpp"


pFlow::IOobject::IOobject
(
    const objectFile&   objf, 
    const IOPattern&    iop, 
    repository*   owner
)
:
	IOfileHeader(objf),
	ioPattern_(iop),
    owner_(owner)
{
    
    if(owner_&& !owner->addToRepository(this))
    {
        fatalExit;
    }
}

pFlow::IOobject::~IOobject()
{
    if(owner_)
    {
        owner_->removeFromRepository(this);
    }
}

pFlow::repository* pFlow::IOobject::releaseOwner
(
    bool fromOwner
)
{   
    auto* old = owner_;
    if(old && !fromOwner)
    {
        old->removeFromRepository(this);
    }
    owner_ = nullptr;
    return old;
}

bool pFlow::IOobject::readObject(bool rdHdr)
{
    
	if( implyRead() )
	{
		if( rdHdr & ioPattern().thisCallRead())
		{	
			if( auto ptrIS = inStream(); ptrIS )
			{
				if(!readHeader(ptrIS()))return false;
			}
			else
			{
				warningInFunction<<
				"could not open file " << path() <<endl;
				return false;
			}
		}

        if(ioPattern().thisCallRead())
        {
            if( auto ptrIS = inStream(); ptrIS )
            {
                if(!readObject(ptrIS(), rdHdr))return false;              
            }
            else
            {
                warningInFunction<<
                "could not open file " << path() <<endl;
                return false;
            }    
        }
		
	}	
	
	return true;
}


bool pFlow::IOobject::writeObject() const
{
	if(implyWrite())
	{
		if(ioPattern().thisProcWriteData())
        {
            if(auto ptrOS = outStream(); ptrOS )
            {
                return writeObject(ptrOS());
            }
            else
            {
                warningInFunction<< 
                "error in opening file "<< path() <<endl;
                return false;
            }
        }
	}

	return true;
}


bool pFlow::IOobject::readObject(iIstream& is, bool rdHdr)
{
	if(rdHdr && ioPattern().thisCallRead() )
	{
		if(!readHeader(is))return false;
	}

    if(ioPattern().thisCallRead())
    {
        return read(is, ioPattern());
    }
    else
    {
        return true;
    }
	
}


bool pFlow::IOobject::writeObject(iOstream& os) const
{
	if(this->writeHeader() && ioPattern().thisProcWriteHeader())
		writeHeader(os, typeName());
    
    if(ioPattern().thisProcWriteData())
	 {   //return (object_->write_object_t(os, ioPattern_) && writeSeparator(os));
        notImplementedFunction;
        //return object_->read_object_t(is, ioPattern_);
        return false;
     }
    else
        return true;
}