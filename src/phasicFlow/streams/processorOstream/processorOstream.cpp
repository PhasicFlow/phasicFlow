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

#include <cstring>

#include "processorOstream.hpp"
#include "token.hpp"


pFlow::processorOstream::processorOstream
(
    std::ostream& os,
    const word& streamName
)
:
    Ostream(os, streamName, writeFormat::ASCII)
{}


pFlow::iOstream& pFlow::processorOstream::write(const char c)
{
    checkForPrefix();
    Ostream::write(c);

    if (c == token::NL)
    {
        printPrefix_ = true;
    }

    return *this;

}


pFlow::iOstream& pFlow::processorOstream::write(const char* str)
{
    checkForPrefix();
    Ostream::write(str);

    size_t len = std::strlen(str);
    if (len && str[len-1] == token::NL)
    {
        printPrefix_ = true;
    }

    return *this;
}


pFlow::iOstream& pFlow::processorOstream::write(const word& str)
{
    checkForPrefix();
    return Ostream::write(str);
}


pFlow::iOstream& pFlow::processorOstream::writeQuoted
(
    const word& str,
    const bool quoted
)
{
    checkForPrefix();
    return Ostream::writeQuoted(str, quoted);
}



pFlow::iOstream& pFlow::processorOstream::write(const int64 val)
{
    checkForPrefix();
    return Ostream::write(val);
}


pFlow::iOstream& pFlow::processorOstream::write(const int32 val)
{
    checkForPrefix();
    return Ostream::write(val);
}


pFlow::iOstream& pFlow::processorOstream::write(const int8 val)
{
    checkForPrefix();
    return Ostream::write(val);
}

pFlow::iOstream& pFlow::processorOstream::write(const uint64 val)
{
    checkForPrefix();
    return Ostream::write(val);
}

pFlow::iOstream& pFlow::processorOstream::write(const uint32 val)
{
    checkForPrefix();
    return Ostream::write(val);
}

pFlow::iOstream& pFlow::processorOstream::write(const uint8 val)
{
    checkForPrefix();
    return Ostream::write(val);
}

pFlow::iOstream& pFlow::processorOstream::write(const float val)
{
    checkForPrefix();
    return Ostream::write(val);
}


pFlow::iOstream& pFlow::processorOstream::write(const double val)
{
    checkForPrefix();
    return Ostream::write(val);
}

pFlow::iOstream& pFlow::processorOstream::write(const size_t val)
{
    checkForPrefix();
    return Ostream::write(val);
}

pFlow::iOstream& pFlow::processorOstream::write
(
    const char* binaryData, 
    std::streamsize count
)
{
    checkForPrefix();
    return Ostream::write(binaryData, count);
}


void pFlow::processorOstream::indent()
{
    checkForPrefix();
    Ostream::indent();
}
