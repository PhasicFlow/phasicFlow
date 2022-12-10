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

#ifndef __bTypesFunctions_hpp__
#define __bTypesFunctions_hpp__

#include "pFlowMacros.hpp"
#include "numericConstants.hpp"
#include "builtinTypes.hpp"
#include "math.hpp"


// helper functions and constants for basic types 
namespace pFlow
{


inline const real  zero = 0.0;
inline const real  one  = 1.0;

inline const int32  zero32 = 0;
inline const int32  one32  = 1;

inline const word nullWord;

inline const word whiteSpace(" \t\n\v\f\r");


int32 countChar (const word& s, const char c);

int32 countChar(const char* s, const char c);

word toUpper(const word & inStr);

bool isYes(const word & str);

bool isNo(const word & str);

word real2Fixed(const real & v, int32 numPrecision = 6);

word real2Word(const real & v, int32 numPrecision = 6);

word removeDecimalZeros(const word& str);

word real2FixedStripZeros(const real & v, int32 numPrecision = 6);

word int322Word(const int32 & v);

 
word angleBracketsNames(const word& w1, const word& w2);

 
word angleBracketsNames2(const word& base, const word& w1, const word& w2);

word angleBracketsNames3(const word& base, const word& w1, const word& w2, const word& w3);

word groupNames(const word& bw, const word& tw, char sep = '.');

word baseName(const word& w, char sep = '.');

word tailName(const word& w, char sep = '.');


// is the character valid for a word name  
bool validWord(char c);

bool validWordWithQuote(char c);


bool validWord(const word& w);

bool validWordWithQuote(const word& c);


bool readLabel( const word& w, label & val);


bool readLabel( const char* buf, label & val);


bool readUint32( const word& w, uint32 & val);


bool readUint32( const char* buf, uint32 & val);


bool readInt64( const word& w, int64 & val);


bool readInt64( const char* buf, int64 & val);


bool readInt32( const word& w, int32 & val);


bool readInt32( const char* buf, int32 & val);


bool readInt16( const word& w, int16 & val);


bool readInt16( const char* buf, int16 & val);


bool readInt8( const word& w, int8 & val);


bool readInt8( const char* buf, int8 & val);


bool readReal( const word& w, real & val);


bool readReal( const char* buf, real & val );


bool readBoolian_Str( const word& w, bool & val);


bool readBoolian_Str( const char* buf, bool & val);

inline
bool readValue(const word& w, real& val)
{
    return readReal(w,val);
}

inline
bool readValue(const word& w, label& val)
{
    return readLabel(w,val);
}

inline
bool readValue(const word& w, uint32& val)
{
    return readUint32(w,val);
}

inline
bool readValue(const word& w, int64& val)
{
    return readInt64(w,val);
}

inline
bool readValue(const word& w, int32& val)
{
    return readInt32(w,val);
}

inline
bool readValue(const word& w, int16& val)
{
    return readInt16(w,val);
}

inline
bool readValue(const word& w, int8& val)
{
    return readInt8(w,val);
}

inline
bool readValue(const word& w, bool& val)
{
    return readBoolian_Str(w,val);
}


INLINE_FUNCTION_HD bool equal(const real& s1, const real& s2)
{
    return abs(s1 - s2) <= smallValue;
}

INLINE_FUNCTION_HD bool equal(const int64& s1, const int64& s2)
{
    return s1 == s2;
}

INLINE_FUNCTION_HD bool equal(const int32& s1, const int32& s2)
{
	return s1 == s2;
}

INLINE_FUNCTION_HD bool equal(const int16& s1, const int16& s2)
{
    return s1 == s2;
}

INLINE_FUNCTION_HD bool equal(const int8& s1, const int8& s2)
{
    return s1 == s2;
}

INLINE_FUNCTION_HD bool equal(const uint32& s1, const uint32& s2)
{
    return s1 == s2;
}

INLINE_FUNCTION_HD bool equal(const label& s1, const label& s2)
{
    return s1 == s2;
}

// host only
INLINE_FUNCTION bool equal(const word& s1, const word& s2)
{
	return s1==s2;
}

INLINE_FUNCTION_HD real degree2Radian(const real &theta)
{
    return theta / 180.0 * Pi;
}

INLINE_FUNCTION_HD real radian2Degree(const real &phi)
{
    return phi / Pi * 180.0;
}


} // end of pFlow


#endif //__bTypesFunctions_hpp__
