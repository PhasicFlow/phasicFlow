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


#ifndef __IOobject_hpp__
#define __IOobject_hpp__



#include "IOfileHeader.hpp"
#include "IOPattern.hpp"

namespace pFlow
{

class repository;

class IOobject
:
	public	IOfileHeader
{
protected:

    IOPattern       ioPattern_;

    mutable repository* owner_;	

public:

	// - typeinfo
	virtual 
    word typeName() const = 0;

	//// - Constructors

		// - construct from components, transfer the ownership of iObject (object_t) to the 
		//   onwner and read the object from file 
		IOobject(
            const objectFile&   objf, 
            const IOPattern&    iop, 
            repository*   owner);
        
        ~IOobject();

		// - copy construct 
		IOobject(const IOobject& src)=delete;	

		// - move construct
		IOobject(IOobject&& src) = delete;
		

        inline
        const IOPattern& ioPattern()const
        {
            return ioPattern_;
        }

        // - pointer to owner repository 
        const repository* owner()const override
        {
            return owner_;
        }

        repository* releaseOwner(bool fromOwner = false);

	//// - IO operations 

		// - read from file 
		bool read(bool rdHdr = true);

		// - write to file 
		bool write() const;

		// - read from istream 
		bool read(iIstream& is, bool rdHdr = true);

		// - write to istream 
		bool write(iOstream& os) const;

        virtual 
        bool write(iOstream& is, const IOPattern& iop)const = 0;

        virtual 
        bool read(iIstream& is, const IOPattern& iop) = 0;

};

}

#include "IOobjectTemplates.cpp"

#endif //__IOobject_hpp__
