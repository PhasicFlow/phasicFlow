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
// based on OpenFOAM stream, with some modifications/simplifications
// to be tailored to our needs

#include "error.hpp"
#include "token.hpp"
#include "Ostream.hpp"


pFlow::Ostream::Ostream
(
    std::ostream& os,
    const word& streamName,
    writeFormat wF
)
:
    iOstream(wF),
    name_(streamName),
    os_(os)
{
    if (os_.good())
    {
        setOpened();
        setGood();
        os_.precision(precision_);
    }
    else
    {
        setState(os_.rdstate());
    }
}



bool pFlow::Ostream::write(const token& tok)
{
    // Direct token handling only for some types

    switch (tok.type())
    {
        case token::tokenType::FLAG :
        {
            // silently consume the flag
            return true;
        }

        case token::tokenType::VARIABLE :
        {
            writeQuoted(tok.wordToken(), false);

            return true;
        }

        default:
            break;
    }

    return false;
}


pFlow::iOstream& pFlow::Ostream::write(const char c)
{
    os_ << c;
    if (c == token::NL)
    {
        ++lineNumber_;
    }
    setState(os_.rdstate());
    return *this;
}


pFlow::iOstream& pFlow::Ostream::write(const char* str)
{
    lineNumber_ += countChar(str, token::NL);
    os_ << str;
    setState(os_.rdstate());
    return *this;
}


pFlow::iOstream& pFlow::Ostream::write(const word& str)
{
    os_ << str;
    setState(os_.rdstate());
    return *this;
}


pFlow::iOstream& pFlow::Ostream::writeQuoted
(
    const word& str,
    const bool quoted
)
{
    if (!quoted)
    {
        // Output unquoted, only advance line number on newline
        lineNumber_ += countChar(str, token::NL);
        os_ << str;

        setState(os_.rdstate());
        return *this;
    }


    // Output with surrounding quotes and backslash escaping
    os_ << token::BEGIN_STRING;

    unsigned backslash = 0;
    for (auto iter = str.cbegin(); iter != str.cend(); ++iter)
    {
        const char c = *iter;

        if (c == '\\')
        {
            ++backslash;
            continue; // only output after escaped character is known
        }
        else if (c == token::NL)
        {
            ++lineNumber_;
            ++backslash;    // backslash escape for newline
        }
        else if (c == token::END_STRING)
        {
            ++backslash;    // backslash escape for quote
        }

        // output all pending backslashes
        while (backslash)
        {
            os_ << '\\';
            --backslash;
        }

        os_ << c;
    }

    // silently drop any trailing backslashes
    // they would otherwise appear like an escaped end-quote
    os_ << token::END_STRING;

    setState(os_.rdstate());
    return *this;
}



pFlow::iOstream& pFlow::Ostream::write(const int64 val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}


pFlow::iOstream& pFlow::Ostream::write(const int32 val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}

/*pFlow::iOstream& pFlow::Ostream::write(const int16 val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}

pFlow::iOstream& pFlow::Ostream::write(const int8 val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}*/

pFlow::iOstream& pFlow::Ostream::write(const label val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}

pFlow::iOstream& pFlow::Ostream::write(const uint32 val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}

pFlow::iOstream& pFlow::Ostream::write(const uint16 val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}

pFlow::iOstream& pFlow::Ostream::write(const float val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}


pFlow::iOstream& pFlow::Ostream::write(const double val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}

pFlow::iOstream& pFlow::Ostream::write
(
    const char* binaryData, 
    std::streamsize count
)
{
    if ( !isBinary() )
    {
        fatalErrorInFunction<<"stream format is not binray. Stream name is "<<
        name()<<'\n';
        fatalExit;
    }

    os_ << token::BEGIN_LIST;
    os_.write(binaryData, count);
    os_ << token::END_LIST;

    setState(os_.rdstate());

    return *this;
}




void pFlow::Ostream::indent()
{
    for (unsigned short i = 0; i < indentLevel_*indentSize_; ++i)
    {
        os_ << ' ';
    }
}


void pFlow::Ostream::flush()
{
    os_.flush();
}


void pFlow::Ostream::endl()
{
    write('\n');
    os_.flush();
}


std::ios_base::fmtflags pFlow::Ostream::flags() const
{
    return os_.flags();
}


std::ios_base::fmtflags pFlow::Ostream::flags(const ios_base::fmtflags f)
{
    return os_.flags(f);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

char pFlow::Ostream::fill() const
{
    return os_.fill();
}


char pFlow::Ostream::fill(const char fillch)
{
    return os_.fill(fillch);
}


int pFlow::Ostream::width() const
{
    return os_.width();
}


int pFlow::Ostream::width(const int w)
{
    return os_.width(w);
}


int pFlow::Ostream::precision() const
{
    return os_.precision();
}


int pFlow::Ostream::precision(const int p)
{
    return os_.precision(p);
}


// ************************************************************************* //
