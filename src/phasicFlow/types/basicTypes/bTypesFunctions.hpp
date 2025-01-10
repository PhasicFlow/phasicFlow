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
/*! \file bTypeFunctions.hpp

    \brief Helper functions for built-in types.
 */

#ifndef __bTypesFunctions_hpp__
#define __bTypesFunctions_hpp__

#include "builtinTypes.hpp"
//#include "math.hpp"
#include "numericConstants.hpp"
#include "pFlowMacros.hpp"

namespace pFlow
{

/// Zero real variable
inline const real  zero = 0.0;

/// one real variable
inline const real  one = 1.0;

/// zero int32 variable
inline const int32 zero32 = 0;

/// one int32 variable
inline const int32 one32 = 1;

/// null/empty word
inline const word  nullWord;

/// white space
inline const word  whiteSpace(" \t\n\v\f\r");

/// Count numer of chars c in a word
int32
countChar(const word& s, const char c);

/// Count numer of chars c in a char string
int32
countChar(const char* s, const char c);

/// convert a word to all caps
word
toUpper(const word& inStr);

word
firstCapital(const word& inStr);

/// Check if str equals "Yes", "Y", "True", "Ok", "ON", or "T"
bool
isYes(const word& str);

/// Check if str equals "No", "N", "False", or "Off"
bool
isNo(const word& str);

/// Convert floating point variable to string with fixed number of precisions
word
real2Fixed(const real& v, int32 numPrecision = 6);

/// Convert floating point variable to string with general format
word
real2Word(const real& v, int32 numPrecision = 6);

/// Convert int32 to word
word
int322Word(const int32& v);

/// Remove zeros from decimal part of a string number
word
removeDecimalZeros(const word& str);

/// Convert to fixed point variable and remove zeros
word
real2FixedStripZeros(const real& v, int32 numPrecision = 6);

/// Output <w1,w2>
word
angleBracketsNames(const word& w1, const word& w2);

/// Output base<w1,w2>
word
angleBracketsNames2(const word& base, const word& w1, const word& w2);

/// Output base<w1,sw2,w3>
word
angleBracketsNames3(
  const word& base,
  const word& w1,
  const word& w2,
  const word& w3
);

/// Group words and output bw.tw
word
groupNames(const word& bw, const word& tw, char sep = '.');

/// Find the base in a group separated by "." and return it.
word
baseName(const word& w, char sep = '.');

/// Find tail name in a group separated by "." and return it.
word
tailName(const word& w, char sep = '.');

/// Is the character valid for a word name?
bool
validWord(char c);

/// Is c a valid character including quote?
bool
validWordWithQuote(char c);

/// Is a valid word?
bool
validWord(const word& w);

/// Is a valid word with qoute?
bool
validWordWithQuote(const word& c);

/// Convert word to uint32
bool
readUint32(const word& w, uint32& val);

/// Convert char string to uint32
bool
readUint32(const char* buf, uint32& val);

/// Convert word to int64
bool
readInt64(const word& w, int64& val);

/// Convert char string to int64
bool
readInt64(const char* buf, int64& val);

/// Convert word to int32
bool
readInt32(const word& w, int32& val);

/// Convert char string to int32
bool
readInt32(const char* buf, int32& val);

/// Convert word to int8
bool
readInt8(const word& w, int8& val);

/// Convert char string to int8
bool
readInt8(const char* buf, int8& val);

/// Convert word to real
bool
readReal(const word& w, real& val);

/// Convert char string to real
bool
readReal(const char* buf, real& val);

/// Convert word to bool
bool
readBoolian_Str(const word& w, bool& val);

/// Convert char string to bool
bool
readBoolian_Str(const char* buf, bool& val);

inline bool
readValue(const word& w, real& val)
{
	return readReal(w, val);
}

inline bool
readValue(const word& w, uint32& val)
{
	return readUint32(w, val);
}

inline bool
readValue(const word& w, int64& val)
{
	return readInt64(w, val);
}

inline bool
readValue(const word& w, int32& val)
{
	return readInt32(w, val);
}

inline bool
readValue(const word& w, int8& val)
{
	return readInt8(w, val);
}

inline bool
readValue(const word& w, bool& val)
{
	return readBoolian_Str(w, val);
}

INLINE_FUNCTION_HD
bool
equal(const real& s1, const real& s2, real tol = smallValue)
{
	return abs(s1 - s2) <= tol;
}

INLINE_FUNCTION_HD
bool
equal(const int64& s1, const int64& s2)
{
	return s1 == s2;
}

INLINE_FUNCTION_HD
bool
equal(const int32& s1, const int32& s2)
{
	return s1 == s2;
}

INLINE_FUNCTION_HD
bool
equal(const int8& s1, const int8& s2)
{
	return s1 == s2;
}

INLINE_FUNCTION_HD
bool
equal(const uint32& s1, const uint32& s2)
{
	return s1 == s2;
}

/// Are two words equal (host only)?
INLINE_FUNCTION
bool
equal(const word& s1, const word& s2)
{
	return s1 == s2;
}

/// Convert degree to radians
INLINE_FUNCTION_HD
real
degree2Radian(const real& theta)
{
	return theta / 180.0 * Pi;
}

/// Convert radians to degree
INLINE_FUNCTION_HD
real
radian2Degree(const real& phi)
{
	return phi / Pi * 180.0;
}

} // end of pFlow

#endif //__bTypesFunctions_hpp__
