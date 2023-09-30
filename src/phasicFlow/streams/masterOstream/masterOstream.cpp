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


#include "masterOstream.hpp"
#include "token.hpp"


pFlow::masterOstream::masterOstream
(
    std::ostream& os,
    const word& streamName
)
:
    Ostream(os, streamName, writeFormat::ASCII)
{}


pFlow::iOstream& pFlow::masterOstream::write(const char c)
{
    if(showOutput())
    {
        Ostream::write(c);
    }
    return *this;
}


pFlow::iOstream& pFlow::masterOstream::write(const char* str)
{
    if(showOutput())
    {
        Ostream::write(str);
    }
    return *this;
}


pFlow::iOstream& pFlow::masterOstream::write(const word& str)
{
    if(showOutput())
    {
        Ostream::write(str);
    }
    return *this;
}


pFlow::iOstream& pFlow::masterOstream::writeQuoted
(
    const word& str,
    const bool quoted
)
{
    if(showOutput())
    {
        Ostream::writeQuoted(str, quoted);
    }
    return *this;
}



pFlow::iOstream& pFlow::masterOstream::write(const int64 val)
{
    if(showOutput())
    {
        Ostream::write(val);
    }
    return *this;   
}


pFlow::iOstream& pFlow::masterOstream::write(const int32 val)
{
    if(showOutput())
    {
        Ostream::write(val);
    }
    return *this; 
}


pFlow::iOstream& pFlow::masterOstream::write(const int8 val)
{
    if(showOutput())
    {
        Ostream::write(val);
    }
    return *this; 
}

pFlow::iOstream& pFlow::masterOstream::write(const uint64 val)
{
    if(showOutput())
    {
        Ostream::write(val);
    }
    return *this; 
}

pFlow::iOstream& pFlow::masterOstream::write(const uint32 val)
{
    if(showOutput())
    {
        Ostream::write(val);
    }
    return *this; 
}

pFlow::iOstream& pFlow::masterOstream::write(const uint8 val)
{
    if(showOutput())
    {
        Ostream::write(val);
    }
    return *this; 
}

pFlow::iOstream& pFlow::masterOstream::write(const float val)
{
    if(showOutput())
    {
        Ostream::write(val);
    }
    return *this; 
}


pFlow::iOstream& pFlow::masterOstream::write(const double val)
{
    if(showOutput())
    {
        Ostream::write(val);
    }
    return *this; 
}


pFlow::iOstream& pFlow::masterOstream::write
(
    const char* binaryData, 
    std::streamsize count
)
{
    if(showOutput())
    {
        Ostream::write(binaryData, count);
    }
    return *this; 
}


void pFlow::masterOstream::indent()
{
    
    if(showOutput())
    {
        Ostream::indent();
    }    
}