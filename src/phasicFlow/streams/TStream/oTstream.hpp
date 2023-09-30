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
#ifndef __oTstream_hpp__
#define __oTstream_hpp__

#include "tokenList.hpp"
#include "iOstream.hpp"


namespace pFlow
{


//- Helper functions declearation

/// Is tok a valid token for this stream?
inline bool validTokenForStream(const token tok);

/// Is tok a begin token?
inline bool isBeginToken(const token& tok);

/// Is tok an end token?
inline bool isEndToken(const token& tok);

/**
 * Output token stream
 * 
 * It is based on OpenFOAM stream, with some modifications/simplifications
 * to be tailored to our needs.
 */
class oTstream
:
    public iOstream
{

protected:

    /// name of stream
    word    name_;

    /// tokenList
    tokenList  tokenList_;

public:
    
    //// - Constructors

        /// emtpy stream with a name
        oTstream(const word& nm);
        
        /// copy construcotr 
        oTstream(const oTstream& src);

        /// move construct 
        oTstream(oTstream&&) = default;

        /// destructor 
        virtual ~oTstream() = default;

    
    //// -  Methods
    
        // Give const access 
        const tokenList& tokens()const;

        // Give access 
        tokenList& tokens();
    

    //// - Write

        /// Write token to stream or otherwise handle it.
        ///  return false if the token type was not handled by this method
        virtual bool write(const token& tok);

        /// Write single character. Whitespace is suppressed.
        virtual iOstream& write(const char c);

        /// Write the word-characters of a character string.
        ///  Sends as a single char, or as word.
        virtual iOstream& write(const char* str);

        /// Write word
        virtual iOstream& write(const word& str);

        
        /// Write std::string surrounded by quotes.
        ///  Optional write without quotes.
        virtual iOstream& writeQuoted(const std::string& str, const bool quoted=true );

        /// Write int64
        virtual iOstream& write(const int64 val) override;

        /// Write int32
        virtual iOstream& write(const int32 val) override;

        /// Write int8
        virtual iOstream& write(const int8 val) override;

        /// Write uint64
        virtual iOstream& write(const uint64 val) override;

        /// Write uint32
        virtual iOstream& write(const uint32 val) override;

        /// Write uint8
        virtual iOstream& write(const uint8 val) override;

        /// Write float
        virtual iOstream& write(const float val) override;

        /// Write double
        virtual iOstream& write(const double val) override;

        /// Write a block of binray data
        iOstream& write(
            const char* binaryData, 
            std::streamsize count) override;

        /// append token to the stream
        virtual void append(const token& tok);
        
        /// append a list of tokens to the stream
        virtual void append(const tokenList& tLisk);
        
    

    //// - Stream state functions

        /// Reset the output buffer and rewind the stream
        void reset();

        /// Rewind the output stream
        virtual void rewind();

        /// Add indentation characters
        virtual void indent()
        {}

        /// Flush stream
        virtual void flush()
        {}

        /// Add newline and flush stream
        virtual void endl()
        {}

        /// Get the current padding character
        ///  \return previous padding character
        virtual char fill() const
        {
            return 0;
        }

        /// Set padding character for formatted field up to field width
        virtual char fill(const char)
        {
            return 0;
        }

        /// Get width of output field
        virtual int width() const
        {
            return 0;
        }

        /// Set width of output field
        ///  \return previous width
        virtual int width(const int)
        {
             return 0;
        }

        /// Get precision of output field
        virtual int precision() const
        {
             return 0;
        }

        /// Set precision of output field
        ///  \return old precision
        virtual int precision(const int)
        {
             return 0;
        }

        /// Return flags of output stream
        virtual ios_base::fmtflags flags() const
        {
            return ios_base::fmtflags(0);
        }

        /// Set flags of stream
        ios_base::fmtflags flags(const ios_base::fmtflags)
        {
            return ios_base::fmtflags(0);
        }

};

#include "helperTstream.hpp"


} // End namespace pFlow


#endif

