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
#ifndef __fileDictionary_hpp__
#define __fileDictionary_hpp__

#include "dictionary.hpp"
#include "IOobject.hpp"

namespace pFlow
{

class fileDictionary
:
    public IOobject,
    public dictionary
{
public:

    TypeInfo("fileDictionary");

    /// construct an empty dictionary with keyword and make it global/fileDictionary
	fileDictionary(const objectFile & of, repository* owner = nullptr);

	/// construct a dictionary with name and read it from file 
	fileDictionary(const word& keyword, const fileSystem& file);

  fileDictionary(
    const objectFile& objf, 
    const dictionary& dict, 
    repository* owner=nullptr);

    using dictionary::read;
    using dictionary::write;

    /// read from stream 
    bool read(iIstream& is, const IOPattern& iop) override;

	/// write to stream
	
    bool write(iOstream& os, const IOPattern& iop) const override;

};

}

#endif //__fileDictionary_hpp__
