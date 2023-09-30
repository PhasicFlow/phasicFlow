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
#ifndef __IOstream_hpp__
#define __IOstream_hpp__


#include <iostream>

#include "bTypesFunctions.hpp"

using std::ios_base;
using std::istream;
using std::ostream;

using std::cin;
using std::cout;
using std::cerr;



namespace pFlow
{

/**
 * A base calss for input/output streams
 * 
 * It is based on OpenFOAM stream, with some modifications/simplifications
 * to be tailored to our needs
 */
class IOstream
{

public:


    enum streamAccess : char
    {
        CLOSED = 0,         /// stream is not open
        OPENED              /// stream is open
    };

    enum writeFormat: char
    {
        ASCII = 0,          
        BINARY
    };

    /// Default precision, only works for ASCII
    static unsigned int precision_;

protected:

	/// Name for any generic stream - normally treat as readonly
    static word staticName_;

    /// Is stream open or closed
    streamAccess openClosed_;

    /// write format 
    writeFormat writeFormat_ = ASCII;

    /// state
    ios_base::iostate ioState_;

  
    /// The file line
    int32 lineNumber_;


    //- Protected Member Functions
   
        /// Set stream opened
        void setOpened()
        {
            openClosed_ = OPENED;
        }

        /// Set stream closed
        void setClosed()
        {
            openClosed_ = CLOSED;
        }

        /// Set stream state
        void setState(ios_base::iostate state)
        {
            ioState_ = state;
        }

        void setWriteFormat(writeFormat wF)
        {
            writeFormat_ = wF;
        }

        /// Set stream to be good
        void setGood()
        {
            ioState_ = ios_base::iostate(0);
        }

public:

    ////- Constructors

        /// Default
        explicit IOstream():
            openClosed_(CLOSED),
            writeFormat_(ASCII),
            ioState_(ios_base::iostate(0)),
            lineNumber_(0)
        {
            setBad();
        }

        /// Construct  and set write format 
        explicit IOstream(writeFormat wF):
            openClosed_(CLOSED),
            writeFormat_(wF),
            ioState_(ios_base::iostate(0)),
            lineNumber_(0)
        {
            setBad();
        }

        /// Copy
        IOstream(const IOstream&) = default;

        /// Destructor
        virtual ~IOstream() = default;
          
        
    ////- Member Functions

        /// Return the name of the stream
        virtual const word& name() const;

        /// Return non-const access to the name of the stream
        virtual word& name();

        /// Check IOstream status for given operation.
        ///  Print IOstream state or generate a FatalIOError
        ///  when an error has occurred.
        ///  The base implementation is a fatalCheck
        virtual bool check(const char* operation) const;

        /// Check IOstream status for given operation.
        ///  Generate a FatalIOError when an error has occurred.
        bool fatalCheck(const char* operation) const;

        /// Return true if stream has been opened
        bool opened() const
        {
            return openClosed_ == OPENED;
        }

        /// Return true if stream is closed
        bool closed() const
        {
            return openClosed_ == CLOSED;
        }

        /// Return true if stream format is binray 
        bool isBinary()const
        {
            return writeFormat_ == BINARY;
        }

        /// Return true if next operation might succeed
        bool good() const
        {
            return ioState_ == 0;
        }

        /// Return true if end of input seen
        bool eof() const
        {
            return ioState_ & ios_base::eofbit;
        }

        /// Return true if next operation will fail
        bool fail() const
        {
            return ioState_ & (ios_base::badbit | ios_base::failbit);
        }

        /// Return true if stream is corrupted
        bool bad() const
        {
            return ioState_ & ios_base::badbit;
        }

        /// Return true if the stream has not failed
        explicit operator bool() const
        {
            return !fail();
        }

        /// Return true if the stream has failed
        bool operator!() const
        {
            return fail();
        }

       
        /// Const access to the current stream line number
        int32 lineNumber() const
        {
            return lineNumber_;
        }

        /// Non-const access to the current stream line number
        int32& lineNumber()
        {
            return lineNumber_;
        }

        /// Set the stream line number
        /// return the previous value
        int32 lineNumber(const int32 num)
        {
            const int32 old(lineNumber_);
            lineNumber_ = num;
            return old;
        }

        /// Return flags of stream
        virtual ios_base::fmtflags flags() const = 0;

        /// Return the default precision
        static unsigned int defaultPrecision()
        {
            return precision_;
        }

        /// Reset the default precision
        /// return the previous value
        static unsigned int defaultPrecision(unsigned int prec)
        {
            unsigned int old(precision_);
            precision_ = prec;
            return old;
        }

        /// Set stream to have reached eof
        void setEof()
        {
            ioState_ |= ios_base::eofbit;
        }

        /// Set stream to have failed
        void setFail()
        {
            ioState_ |= ios_base::failbit;
        }

        /// Set stream to be bad
        void setBad()
        {
            ioState_ |= ios_base::badbit;
        }

        /// Set flags of stream
        virtual ios_base::fmtflags flags(const ios_base::fmtflags f) = 0;

        /// Set flags of stream
        ios_base::fmtflags setf(const ios_base::fmtflags f)
        {
            return flags(flags() | f);
        }

        /// Set flags of given field of stream
        ios_base::fmtflags setf
        (
            const ios_base::fmtflags f,
            const ios_base::fmtflags mask
        )
        {
            return flags((flags() & ~mask) | (f & mask));
        }

        /// Unset flags of stream
        void unsetf(const ios_base::fmtflags f)
        {
            flags(flags() & ~f);
        }

}; // end of IOstream


/// An IOstream manipulator
typedef IOstream& (*IOstreamManip)(IOstream&);


inline IOstream& dec(IOstream& io)
{
    io.setf(ios_base::dec, ios_base::dec|ios_base::hex|ios_base::oct);
    return io;
}

inline IOstream& hex(IOstream& io)
{
    io.setf(ios_base::hex, ios_base::dec|ios_base::hex|ios_base::oct);
    return io;
}

inline IOstream& oct(IOstream& io)
{
    io.setf(ios_base::oct, ios_base::dec|ios_base::hex|ios_base::oct);
    return io;
}

inline IOstream& fixed(IOstream& io)
{
    io.setf(ios_base::fixed, ios_base::floatfield);
    return io;
}

inline IOstream& scientific(IOstream& io)
{
    io.setf(ios_base::scientific, ios_base::floatfield);
    return io;
}


} // pFlow

#endif  //  __IOstream__hpp__
