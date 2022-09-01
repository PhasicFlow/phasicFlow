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


#include "iOstream.H"
#include "token.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void pFlow::iOstream::decrIndent()
{
    if (!indentLevel_)
    {
        std::cerr
            << "iOstream::decrIndent() : attempt to decrement 0 indent level\n";
    }
    else
    {
        --indentLevel_;
    }
}


pFlow::iOstream& pFlow::iOstream::writeWordKeyword(const word& kw)
{
    
    indent();
    writeQuoted(kw, false);
    
    if (indentSize_ <= 1)
    {
        write(char(token::SPACE));
        return *this;
    }

    int32 nSpaces = entryIndentation_ - int32(kw.size());   

    // Could also increment by indentSize_ ...
    if (nSpaces < 1)
    {
        nSpaces = 1;
    }

    while (nSpaces--)
    {
        write(char(token::SPACE));
    }

    return *this;
}


pFlow::iOstream& pFlow::iOstream::beginBlock(const word& kw)
{
    indent(); write(kw); newLine();
    beginBlock();

    return *this;
}


pFlow::iOstream& pFlow::iOstream::beginBlock()
{
    indent(); write(char(token::BEGIN_BLOCK)); newLine();
    incrIndent();

    return *this;
}


pFlow::iOstream& pFlow::iOstream::endBlock()
{
    decrIndent();
    indent(); write(char(token::END_BLOCK)); newLine();

    return *this;
}


pFlow::iOstream& pFlow::iOstream::endEntry()
{
    write(char(token::END_STATEMENT)); newLine();

    return *this;
}

//- Write a newLine to stream
pFlow::iOstream& pFlow::iOstream::newLine()
{
    write(char(token::NL));
    return *this;
}

pFlow::iOstream& pFlow::iOstream::space
(
    int32 n
)
{
    for(int32 i=0; i<n; i++)
    {
        write(char(token::SPACE));        
    }
    return *this;
}


pFlow::iOstream& pFlow::iOstream::beginList
(
)
{
    write(char(token::BEGIN_LIST));
    return *this;
}


pFlow::iOstream& pFlow::iOstream::beginList
(
    const word& kw
)
{
    writeWordKeyword(kw); beginList();
    return *this;
}


pFlow::iOstream& pFlow::iOstream::endList
(
)
{
    write(char(token::END_LIST));
    return *this;
}


pFlow::iOstream& pFlow::iOstream::beginSquare
(
)
{
    write(char(token::BEGIN_SQR));
    return *this;
}


pFlow::iOstream& pFlow::iOstream::beginSquare
(
    const word& kw
)
{
    writeWordKeyword(kw); beginSquare();
    return *this;
}


pFlow::iOstream& pFlow::iOstream::endSquare
(
)
{
    write(char(token::END_SQR));
    return *this;
}


// ************************************************************************* //
