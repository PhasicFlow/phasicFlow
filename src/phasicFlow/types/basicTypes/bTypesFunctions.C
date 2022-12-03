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

#include <algorithm>
#include <sstream>
#include <iomanip>

#include "bTypesFunctions.H"


pFlow::int32 pFlow::countChar(const word& s, const char c)
{
    return std::count(s.cbegin(), s.cend(), c);
}


pFlow::int32 pFlow::countChar( const char* s, const char c)
{
    return
    (
      s == nullptr
      ? 0
      : std::count(s, (s + std::char_traits<char>::length(s)), c)
    );
}

pFlow::word pFlow::real2Fixed(const real & v, int32 numPrecision)
{
    std::stringstream ss;

    ss << std::fixed << std::setprecision(numPrecision) << v;
    return ss.str();
}

pFlow::word pFlow::real2Word(const real & v, int32 numPrecision)
{
    std::stringstream ss;
    if( abs(v) < verySmallValue )
    {
        ss <<"0";
    }
    else
    {
        ss << std::setprecision(numPrecision) << v;
    }
    
    return ss.str();
}

pFlow::word pFlow::int322Word(const int32 & v)
{
    std::stringstream ss;

    ss << v;
    return ss.str();
}

pFlow::word pFlow::removeDecimalZeros(const word& str)
{
    auto dec = str.find('.');
    if(dec == word::npos) return str;
    
    auto len = str.size();
    if(len == word::npos) return str;
    
    auto  firstZero = word::npos;
    for(auto n=len-1; n>dec;n--)
    {
        if( str[n] == '0' ) 
        {
            firstZero = n;
        }
        else
        {
            break;
        }
    }

    if(firstZero == dec+1) firstZero = dec;
    
    return str.substr(0,firstZero);
}

pFlow::word pFlow::real2FixedStripZeros(const real & v, int32 numPrecision)
{
    word strVal = real2Fixed(v, numPrecision);
    return removeDecimalZeros(strVal);
}

pFlow::word pFlow::toUpper(const word & inStr)
{
    word oStr(inStr);
    transform(inStr.begin(), inStr.end(), oStr.begin(), ::toupper);
    return oStr;
}

bool pFlow::isYes(const word & str)
{
    word s = toUpper(str);

    if( s == "YES" || s == "OK" || s == "TRUE" || s == "ON" || s=="T") return true;
    return false;
}

bool pFlow::isNo(const word & str)
{
    word s = toUpper(str);

    if( s == "NO" || s == "N" || "FALSE" || s == "OFF" || s == "F") return true;
    return false;
}


pFlow::word pFlow::angleBracketsNames(const word& w1, const word& w2)
{
    return w1+"<"+w2+">";
}

pFlow::word pFlow::angleBracketsNames2
(
    const word& base,
    const word& w1,
    const word& w2
)
{
    return base+"<"+w1+","+w2+">";
}

pFlow::word pFlow::angleBracketsNames3(const word& base, const word& w1, const word& w2, const word& w3)
{
    return base+"<"+w1+","+w2+","+w3+">";
}

pFlow::word pFlow::groupNames(const word& bw, const word& tw, char sep)
{
    return bw + sep + tw;
}

pFlow::word pFlow::baseName(const word& w, char sep)
{
    if( auto pos = w.find_last_of(sep); pos != word::npos)
    {
        return w.substr(0,pos);
    }
    else
    {
        return w;
    }
}

pFlow::word pFlow::tailName(const word& w, char sep)
{
    if( auto pos = w.find_last_of(sep); pos != word::npos)
    {
        return w.substr(pos+1);
    }
    else
    {
        return nullWord;
    }   
}

bool pFlow::validWord(char c)
{
    return
    (
        !isspace(c)
     && c != '"'   // string quote
     && c != '\''  // string quote
     //&& c != '/'   // path separator
     && c != ';'   // end statement
     && c != '{'   // beg subdict
     && c != '}'   // end subdict
    );
}

bool pFlow::validWordWithQuote(char c)
{
    return
    (
     !isspace(c)
     && c != ';'   // end statement
     && c != '{'   // beg subdict
     && c != '}'   // end subdict
    );
}

bool pFlow::validWord(const word& w)
{
    for(auto wi:w)
    {   
        char c = wi;
        if ( !validWord(c) ) return false;
    }
    return true;
}

bool pFlow::validWordWithQuote(const word& w)
{
    for(auto wi:w)
    {   
        char c = wi;
        if ( !validWordWithQuote(c) ) return false;
    }
    return true;
}


bool pFlow::readLabel( const word& w, label & val)
{
    try{
        val = std::stoull(w);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool pFlow::readLabel( const char* buf, label & val)
{
    word w(buf);
    return readLabel(w, val);
}

bool pFlow::readUint32( const word& w, uint32 & val)
{
    try{
        val = std::stoul(w);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool pFlow::readUint32( const char* buf, uint32 & val)
{
    word w(buf);
    return readUint32(w, val);
}

bool pFlow::readInt64( const word& w, int64 & val)
{
    try{
        val = std::stoll(w);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool pFlow::readInt64( const char* buf, int64 & val)
{
    word w(buf);
    return readInt64(w, val);
}

bool pFlow::readInt32( const word& w, int32 & val)
{
    try{
        val = std::stoi(w);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool pFlow::readInt32( const char* buf, int32 & val)
{
    word w(buf);
    return readInt32(w, val);
}


bool pFlow::readInt16( const word& w, int16 & val)
{
    try{
        val = static_cast<int16>(std::stoi(w));
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool pFlow::readInt16( const char* buf, int16 & val)
{
    word w(buf);
    return readInt16(w, val);
}

bool pFlow::readInt8( const word& w, int8 & val)
{
    try{
        val = static_cast<int8>(std::stoi(w));
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool pFlow::readInt8( const char* buf, int8 & val)
{
    word w(buf);
    return readInt8(w, val);
}
#include <iostream>
bool pFlow::readReal( const word& w, real & val)
{
    try{
        val = std::stod(w);
        
    }
    catch (std:: out_of_range& e)
    {
        val = static_cast<real>( std::stold(w) );
    }
    catch (...){
        return false;
    }
    return true;
}

bool pFlow::readReal( const char* buf, real & val )
{
    char* c;
    
    val = std::strtod(buf, &c);
    if(val == HUGE_VAL)
    {
        val = static_cast<real>( std::strtold(buf, &c) );  
        if(val == HUGE_VAL || c==buf)
            return false;
    }
    else if(c == buf)    
    {
        return false;
    }
    
    
    return true;
}


bool pFlow::readBoolian_Str( const word& w, bool & val)
{
    if( bool t = isYes(w); t )
    {
        val = true;
        return true;    
    } 
    if( bool f = isNo(w); f )
    {
        val = false;
        return true;
    }
    return false;
}

bool pFlow::readBoolian_Str( const char* buf, bool & val)
{
    word w(buf);
    return readBoolian_Str(w, val);
}
