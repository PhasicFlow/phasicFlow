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


#ifndef __Istream_H__
#define __Istream_H__

#include <limits>
#include <iostream>

#include "iIstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace pFlow
{


class Istream
:
    public iIstream
{
    // Private Data

    word    name_;

    std::istream& is_;


    //- Get the next valid character
    char nextValid();

    //- Read a word token
    void readWordToken(token& t);

    
    //- Read a variable name starting with '$'.
    //  Handles "$var" and "${var}" forms, permits '/' scoping character.
    Istream& readVariable(word& str);

    //- No copy assignment
    void operator=(const Istream&) = delete;


public:


    //- Construct wrapper around std::istream, set stream status
    Istream( std::istream& is, const word& streamName);


    //- Destructor
    virtual ~Istream() = default;


    //// - Methods

    //- Return the name of the stream
    virtual const word& name() const
    {
        return name_;
    }

    //- Return non-const access to the name of the stream
    virtual word& name()
    {
        return name_;
    }

    //- Return flags of output stream
    virtual ios_base::fmtflags flags() const;


    //// Read Functions

    //- Raw, low-level get character function.
    Istream& get(char& c);

    //- Raw, low-level peek function.
    //  Does not remove the character from the stream.
    //  Returns the next character in the stream or EOF if the
    //  end of file is read.
    int peek();

    //- Raw, low-level getline (until delimiter) into a string.
    Istream& getLine(word& str, char delim = '\n');

    //- Low-level discard until delimiter
    //  return the number of characters extracted
    std::streamsize getLine(std::nullptr_t, char delim = '\n');

    //- Raw, low-level putback character function.
    Istream& putback(const char c);

    //- Return next token from stream
    virtual iIstream& read(token& t) override;

    //- Read a character
    virtual iIstream& read(char& c) override;

    //- Read a word
    virtual iIstream& read(word& str) override;

    //- Read a string
    virtual iIstream& readString(word& str) override;

    //- Read a int64
    virtual iIstream& read(int64&) override;

    //- Read a int32
    virtual iIstream& read(int32&) override;

    //- Read a int16
    virtual iIstream& read(int16&) override;

    //- Read a int8
    virtual iIstream& read(int8&) override;

    //- Read a label
    virtual iIstream& read(label&) override;

    //- Read a uint32
    virtual iIstream& read(uint32&) override;

    //- Read a uint16
    virtual iIstream& read(uint16&) override;

    //- Read a float
    virtual iIstream& read(float& val) override;

    //- Read a double
    virtual iIstream& read(double& val) override;

    
    //- Rewind the stream so that it may be read again
    virtual void rewind();


    //- Set stream flags
    virtual ios_base::fmtflags flags(const ios_base::fmtflags flags);



    //- Access to underlying std::istream
    virtual std::istream& stdStream()
    {
        return is_;
    }

    //- Const access to underlying std::istream
    virtual const std::istream& stdStream() const
    {
        return is_;
    }


};


} 


#endif

