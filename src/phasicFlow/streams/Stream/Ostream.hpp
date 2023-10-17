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

#ifndef __Ostream_hpp__
#define __Ostream_hpp__

#include "iOstream.hpp"
#include <iostream>


namespace pFlow
{

/**
 * Standard output stream for BINARY and ASCII formats
 * 
 * It is based on OpenFOAM stream, with some modifications/simplifications
 * to be tailored to our needs
 */
class Ostream
:
    public iOstream
{
private:

    // - private data members 

    /// Stream name 
    word name_;

    /// Output stream 
    std::ostream& os_;


public:

    ///- Constructors
    
        /// Construct from components 
        Ostream ( std::ostream& os, const word& streamName, writeFormat wf = ASCII);

        /// No copy construct 
        Ostream(const Ostream&) = delete;

        /// No copy assignment
        void operator=(const Ostream&) = delete;


    //// - Methods

        /// Return the name of the stream
        virtual const word& name() const
        {
            return name_;
        }

        /// Return non-const access to the name of the stream
        virtual word& name()
        {
            return name_;
        }

        /// Return flags of output stream
        virtual ios_base::fmtflags flags() const;


        /// Write token to stream or otherwise handle it.
        ///  return false if the token type was not handled by this method
        bool write(const token& tok)override;

        /// Write character
        iOstream& write(const char c)override;

        /// Write character string
        iOstream& write(const char* str)override;

        /// Write word
        iOstream& write(const word& str)override;
    
        /// Write std::string surrounded by quotes.
        ///  Optional write without quotes.
        iOstream& writeQuoted ( const word& str, const bool quoted=true ) override;

        /// Write int64
        iOstream& write(const int64 val) override;

        /// Write int32
        iOstream& write(const int32 val) override;

        /// Write int32
        iOstream& write(const int8 val) override;

        /// Write uint64
        iOstream& write(const uint64 val) override;

        /// Write uint32
        iOstream& write(const uint32 val) override;

        /// Write uint8
        iOstream& write(const uint8 val) override;

        /// Write float
        iOstream& write(const float val) override;

        /// Write double
        iOstream& write(const double val) override;

        /// Write size_t
        iOstream& write(const size_t val) override;

        /// Write a block of binray data 
        iOstream& write(const char* binaryData, std::streamsize count) override;

        iOstream& writeBinaryBlockFlag() override;

        /// Add indentation characters
        void indent() override;

        /// Set stream flags
        ios_base::fmtflags flags(const ios_base::fmtflags f) override;

        /// Flush stream
        void flush() override;

        /// Add newline and flush stream
        void endl() override;

        /// Get the current padding character
        char fill() const override;

        /// Set padding character for formatted field up to field width
        ///  \return previous padding character
        char fill(const char fillch) override;

        /// Get width of output field
        int width() const override;

        /// Set width of output field
        ///  \return previous width
        int width(const int w) override;

        /// Get precision of output field
        int precision() const override;

        /// Set precision of output field
        ///  return old precision
        int precision(const int p) override;

        /// Access to underlying std::ostream
        virtual std::ostream& stdStream() 
        {
            return os_;
        }

        /// Const access to underlying std::ostream
        virtual const std::ostream& stdStream() const
        {
            return os_;
        }

};


} // pFlow


#endif //__Ostream_hpp__


