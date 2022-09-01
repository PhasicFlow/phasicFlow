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

#ifndef __Ostream_H__
#define __Ostream_H__

#include "iOstream.H"
#include <iostream>


namespace pFlow
{



class Ostream
:
    public iOstream
{

    word name_;

    std::ostream& os_;


public:

    // Constructors
    
    Ostream ( std::ostream& os, const word& streamName);

    //- no copy construct 
    Ostream(const Ostream&) = delete;

    //- No copy assignment
    void operator=(const Ostream&) = delete;


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


        //- Write token to stream or otherwise handle it.
        //  return false if the token type was not handled by this method
        virtual bool write(const token& tok)override;

        //- Write character
        virtual iOstream& write(const char c)override;

        //- Write character string
        virtual iOstream& write(const char* str)override;

        //- Write word
        virtual iOstream& write(const word& str)override;
    
        //- Write std::string surrounded by quotes.
        //  Optional write without quotes.
        virtual iOstream& writeQuoted ( const word& str, const bool quoted=true )override;

        //- Write int64
        virtual iOstream& write(const int64 val) override;

        //- Write int32
        virtual iOstream& write(const int32 val) override;


        //- Write label
        virtual iOstream& write(const label val) override;

        //- Write uint32
        virtual iOstream& write(const uint32 val) override;

        //- Write uint16
        virtual iOstream& write(const uint16 val) override;

        //- Write float
        virtual iOstream& write(const float val) override;

        //- Write double
        virtual iOstream& write(const double val) override;

        //- Add indentation characters
        virtual void indent();


        //- Set stream flags
        virtual ios_base::fmtflags flags(const ios_base::fmtflags f);

        //- Flush stream
        virtual void flush();

        //- Add newline and flush stream
        virtual void endl();

        //- Get the current padding character
        virtual char fill() const;

        //- Set padding character for formatted field up to field width
        //  \return previous padding character
        virtual char fill(const char fillch);

        //- Get width of output field
        virtual int width() const;

        //- Set width of output field
        //  \return previous width
        virtual int width(const int w);

        //- Get precision of output field
        virtual int precision() const;

        //- Set precision of output field
        //  return old precision
        virtual int precision(const int p);

        //- Access to underlying std::ostream
        virtual std::ostream& stdStream()
        {
            return os_;
        }

        //- Const access to underlying std::ostream
        virtual const std::ostream& stdStream() const
        {
            return os_;
        }

};



} // pFlow


#endif


