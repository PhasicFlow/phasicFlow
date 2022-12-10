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


#ifndef __iOstream_hpp__
#define __iOstream_hpp__

// based on OpenFOAM stream, with some modifications/simplifications
// to be tailored to our needs


#include "IOstream.hpp"

const inline char* defaultColor = "\033[0m";
const inline char* blackColor   = "\033[30m";
const inline char* redColor     = "\033[31m";
const inline char* greenColor   = "\033[32m";
const inline char* yellowColor  = "\033[33m";
const inline char* blueColor    = "\033[34m";
const inline char* magentaColor = "\033[35m";
const inline char* cyanColor    = "\033[36m";
const inline char* whiteColor   = "\033[37m"; 

const inline char* boldChar     = "\033[1m";



namespace pFlow
{

// Forward Declarations
class token;



class iOstream
:
    public IOstream
{
protected:

    // Protected Data

        //- Indentation of the entry from the start of the keyword
        static constexpr const unsigned short entryIndentation_ = 16;

        //- Number of spaces per indent level
        unsigned short indentSize_ = 4;

        //- Current indent level
        unsigned short indentLevel_ = 0;


public:
    

    // Constructor
    explicit iOstream()
    {}

    //- Copy construct
    iOstream(const iOstream&) = default;

    //- Destructor
    virtual ~iOstream() = default;


    // Write Functions

    //- Write token to stream or otherwise handle it.
    //  \return false if the token type was not handled by this method
    virtual bool write(const token& tok) = 0;

    //- Write character
    virtual iOstream& write(const char c) = 0;

    //- Write character string
    virtual iOstream& write(const char* str) = 0;

    //- Write word
    virtual iOstream& write(const word& str) = 0;


    //- Write std::string surrounded by quotes.
    //  Optional write without quotes.
    virtual iOstream& writeQuoted
    (
        const word& str,
        const bool quoted=true
    ) = 0;

    
    //- Write int64
    virtual iOstream& write(const int64 val) = 0;

    //- Write int32
    virtual iOstream& write(const int32 val) = 0;

    //- Write label
    virtual iOstream& write(const label val) = 0;

    //- Write uint32
    virtual iOstream& write(const uint32 val) = 0;

    //- Write uint16
    virtual iOstream& write(const uint16 val) = 0;

    //- Write float
    virtual iOstream& write(const float val) = 0;

    //- Write double
    virtual iOstream& write(const double val) = 0;
   

    
    //- Add indentation characters
    virtual void indent() = 0;

    //- Return indent level
    unsigned short indentSize() const
    {
        return indentSize_;
    }

    //- Access to indent size
    unsigned short& indentSize()
    {
        return indentSize_;
    }

    //- Return indent level
    unsigned short indentLevel() const
    {
        return indentLevel_;
    }

    //- Access to indent level
    unsigned short& indentLevel()
    {
        return indentLevel_;
    }

    //- Increment the indent level
    void incrIndent()
    {
        ++indentLevel_;
    }

    //- Decrement the indent level
    void decrIndent();

   

    
    //- Write begin block group with a name
    //  Increments indentation, adds newline.
    virtual iOstream& beginBlock(const word& kw);

    //- Write begin block group without a name
    //  Increments indentation, adds newline.
    virtual iOstream& beginBlock();

    //- Write end block group
    //  Decrements indentation, adds newline.
    virtual iOstream& endBlock();

    //- Write begin list "("
    virtual iOstream& beginList();

    //- Write begin list with keyword "kw ("
    virtual iOstream& beginList(const word& kw);

    //- Write end list ")"
    virtual iOstream& endList();

    //- Write begin list "["
    virtual iOstream& beginSquare();

    //- Write begin list with keyword "kw ["
    virtual iOstream& beginSquare(const word& kw);

    //- Write end list "]"
    virtual iOstream& endSquare();

    //- Write end entry (';') followed by newline.
    virtual iOstream& endEntry();

    //- Write a newLine to stream
    virtual iOstream& newLine();

    //- Write space to stream
    virtual iOstream& space(int32 n=1);

    
     //- Write the keyword followed by an appropriate indentation
    virtual iOstream& writeWordKeyword(const word& kw);

    //- Write a keyword/value entry.
    template<class T>
    iOstream& writeWordEntry(const word& key, const T& value)
    {
        writeWordKeyword(key) << value;
        return endEntry();
    }

    //// Stream state functions

    //- Flush stream
    virtual void flush() = 0;

    //- Add newline and flush stream
    virtual void endl() = 0;

    //- Get padding character
    virtual char fill() const = 0;

    //- Set padding character for formatted field up to field width
    virtual char fill(const char fillch) = 0;

    //- Get width of output field
    virtual int width() const = 0;

    //- Set width of output field (and return old width)
    virtual int width(const int w) = 0;

    //- Get precision of output field
    virtual int precision() const = 0;

    //- Set precision of output field (and return old precision)
    virtual int precision(const int p) = 0;


    // Member Operators

    //- Return a non-const reference to const iOstream
    //  Needed for write functions where the stream argument is temporary:
    //  e.g. thing thisThing(OFstream("thingFileName")());
    iOstream& operator()() const
    {
        return const_cast<iOstream&>(*this);
    }
};



//- An iOstream manipulator
typedef iOstream& (*iOstreamManip)(iOstream&);


//- operator<< handling for manipulators without arguments
inline iOstream& operator<<(iOstream& os, iOstreamManip f)
{
    return f(os);
}

//- operator<< handling for manipulators without arguments
inline iOstream& operator<<(iOstream& os, IOstreamManip f)
{
    f(os);
    return os;
}


//- Indent stream
inline iOstream& indent(iOstream& os)
{
    os.indent();
    return os;
}

//- Increment the indent level
inline iOstream& incrIndent(iOstream& os)
{
    os.incrIndent();
    return os;
}

//- Decrement the indent level
inline iOstream& decrIndent(iOstream& os)
{
    os.decrIndent();
    return os;
}


//- Flush stream
inline iOstream& flush(iOstream& os)
{
    os.flush();
    return os;
}


//- Add newline and flush stream
inline iOstream& endl(iOstream& os)
{
    os.endl();
    return os;
}


//- Write begin block group without a name
//  Increments indentation, adds newline.
inline iOstream& beginBlock(iOstream& os)
{
    os.beginBlock();
    return os;
}


//- Write end block group
//  Decrements indentation, adds newline.
inline iOstream& endBlock(iOstream& os)
{
    os.endBlock();
    return os;
}


//- Write end entry (';') followed by newline.
inline iOstream& endEntry(iOstream& os)
{
    os.endEntry();
    return os;
}


// overloading for basic types
inline iOstream& operator<<( iOstream& os, const char c)
{
    return os.write(c);
}

inline iOstream& operator<<( iOstream& os, const char * buf)
{
    return os.write(buf);
}

inline iOstream& operator<<( iOstream& os, const word& w)
{
    return os.write(w);
}


inline iOstream& operator<<( iOstream& os, const int64& val)
{
    return os.write(val);
}

inline iOstream& operator<<( iOstream& os, const int32& val)
{
    return os.write(val);
}

inline iOstream& operator<<( iOstream& os, const int16& val)
{
    return os.write(val);
}

inline iOstream& operator<<( iOstream& os, const int8& val)
{
    return os.write(val);
}

inline iOstream& operator<<( iOstream& os, const label& val)
{
    return os.write(val);
}

inline iOstream& operator<<( iOstream& os, const uint32& val)
{
    return os.write(val);
}

inline iOstream& operator<<( iOstream& os, const uint16& val)
{
    return os.write(val);
}

inline iOstream& operator<<( iOstream& os, const float& val)
{
    return os.write(val);
}


inline iOstream& operator<<( iOstream& os, const double& val)
{
    return os.write(val);
}
// Useful aliases for tab and newline characters
constexpr char tab = '\t';
constexpr char nl = '\n';




} //  pFlow


#endif

// ************************************************************************* //
