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

#ifndef __Set_hpp__
#define __Set_hpp__

#include <set>

#include "types.hpp"
#include "iOstream.hpp"

namespace pFlow
{


template<typename Key>
using Set = std::set<Key,std::less<Key>,std::allocator<Key>>;

using wordSet = Set<word>;

template<typename key>
iOstream& operator<<(iOstream& os, const Set<key>& s)
{
	os << beginListToken();
	for(auto elm = s.begin(); elm!=s.end(); )
	{
		os<< *elm++;
        if( elm!=s.end() )
			os<<spaceToken();
	}
	os<< endListToken();
    os.check(FUNCTION_NAME);
    return os;
}

}

#endif 
