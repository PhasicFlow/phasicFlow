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

#include "dictionary.hpp"


namespace pFlow
{

class IOPattern;

class fileDictionary
:
    public dictionary
{
public:

    TypeInfo("fileDictionary");

    /// construct an empty dictionary with keyword and make it global/fileDictionary
	fileDictionary(const word& keyword);

		/// construct a dictionary with name and read it from file 
	fileDictionary(const word& keyword, const fileSystem& file);



    /// read from stream
	virtual bool read(iIstream& is, const IOPattern& iop);

	/// write to stream
	virtual bool write(iOstream& os, const IOPattern& iop) const;

};

}
