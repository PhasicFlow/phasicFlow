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

#ifndef __iIstream_H__
#define __iIstream_H__

#include "IOstream.H"
#include "token.H"
#include "error.H"
#include "iOstream.H"

namespace pFlow
{


class iIstream // interface class for input streams
:
	public IOstream
{

	// Private Data

    //- Has a token been put back on the stream?
    bool putBack_;

    //- The last token put back on the stream
    token putBackToken_;

public:

    //// - Constructors    

    	// - default
	    iIstream():
			IOstream(),
	        putBack_(false)	
	    {}

	    // - Copy construct
	    iIstream(const iIstream&) = default;

	    // - Destructor
	    virtual ~iIstream() = default;



    //// member methods

	    //- Put back token
	    //  Only a single put back is permitted
	    void putBack(const token& tok);

	    //- Get the put back token if there is one and return true.
	    //  Return false if no put back token is available.
	    bool getBack(token& tok);

	    //- Peek at the put back token without removing it.
	    //  Returns false if no put back token is available and set the
	    //  token to undefined.
	    bool peekBack(token& tok);

	    //- reset the put back token;
	    void resetPutBack()
	    {
	    	putBackToken_.reset();
	    	putBack_ = false;
	    }

	    //- Return next token from stream
	    virtual iIstream& read(token&) = 0;

	    //- Read a character
	    virtual iIstream& read(char&) = 0;

	    //- Read a string (including enclosing double-quotes)
	    virtual iIstream& read(word&) = 0;

	    //- Read a string 
	    virtual iIstream& readString(word&) = 0;    

	    //- Read a int64
	    virtual iIstream& read(int64&) = 0;

	    //- Read a int32
	    virtual iIstream& read(int32&) = 0;

	    //- Read a int16
	    virtual iIstream& read(int16&) = 0;

	    //- Read a int8
	    virtual iIstream& read(int8&) = 0;

	    //- Read a label
	    virtual iIstream& read(label&) = 0;

	    //- Read a uin32
	    virtual iIstream& read(uint32&) = 0;

	    //- Read a uin16
	    virtual iIstream& read(uint16&) = 0;

	    //- Read a floatScalar
	    virtual iIstream& read(float&) = 0;

	    //- Read a doubleScalar
	    virtual iIstream& read(double&) = 0;

	    
	    //- Rewind the stream so that it may be read again
	    virtual void rewind() = 0;


    //// - find and lookups

        // - search for all tokesn and find the first word token tbat matchs w
        virtual bool findToken( const word & w );


        // - search for all tokesn and find the first word token tbat matchs 
        virtual bool findTokenSilent( const word & w, int32 limitLine = 100 );

        // - search for all tokens and find the first word token and also next word token
        //   chekck if it i seneded with end statement ;
        virtual bool findTokenAndNext( const word& w, word& nextW, bool checkEndStatement = true);


        virtual bool findTokenAndNextSilent( const word& w, word& nextW, int32 limitLine = 100);

        // - find a pair of keyword and data terminated by ;
        //   keyword data;
        //   return false if keyword does not exist or reading fails. 
        template<typename T>
        bool findKeywordAndVal(const word& keyword, T& val, bool checkEndStatement = true);

        // - lookup for keyword and data;
        //   fatalExit if fails
        template<typename T>
        T lookupData(const word& keyword);

        // - lookup for keyword and data;
        //   set to setVal if lookup fails. 
        template<typename T>
        T lookupDataOrSet(const word& keyword, const T& setVal);
        
        // - read the data next to keword 
        //   keyword    data; 
        //   check the keyword is correct or not 
        template<typename T>
        bool nextData(const word& keyword, T& data);

	//// Read List punctuation tokens

	    //- Begin read of data chunk, starts with '('.
	    //  return true or FatalIOError
	    bool readBegin(const char* funcName);

	    //- End read of data chunk, ends with ')'
	    //  return true or FatalIOError
	    bool readEnd(const char* funcName);

	    //- Begin read of data chunk, starts with '('.
	    //  return true or FatalIOError
	    bool readBeginSquare(const char* funcName);

	    //- Begin read of data chunk, starts with '('.
	    //  return true or FatalIOError
	    bool readEndSquare(const char* funcName);

	    //- Begin read of list data, starts with '(' or '{'
	    //  return starting delimiter or FatalIOError
	    char readBeginList(const char* funcName);

	    //- End read of list data, ends with ')' or '}'
	    //  return closing delimiter or FatalIOError
	    char readEndList(const char* funcName);

	    // End statement character ;
	    char readEndStatement(const char* funcName);

    	
    	iIstream& operator()() const;

};


typedef iIstream& (*iIstreamManip)(iIstream&);

//- operator>> handling for manipulators without arguments
inline iIstream& operator>>(iIstream& is, iIstreamManip f)
{
    return f(is);
}

//- operator>> handling for manipulators without arguments
inline iIstream& operator>>(iIstream& is, IOstreamManip f)
{
    f(is);
    return is;
}


// read operation for basic types it gets from the 
// token stream

inline iIstream& operator>>( iIstream& is, word & w);

inline iIstream& operator>>( iIstream& is, int64& val);

inline iIstream& operator>>( iIstream& is, int32& val);

inline iIstream& operator>>( iIstream& is, int16& val);

inline iIstream& operator>>( iIstream& is, int8& val);

inline iIstream& operator>>( iIstream& is, uint32& val);

inline iIstream& operator>>( iIstream& is, uint16& val);

inline iIstream& operator>>( iIstream& is, label& val);

inline iIstream& operator>>( iIstream& is, float& val);

inline iIstream& operator>>( iIstream& is, double& val);



} // pFlow


#include "iIstreamI.H"


#endif
