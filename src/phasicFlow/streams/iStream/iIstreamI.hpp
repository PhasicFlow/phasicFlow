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


// * * * * * * * * * * * * * * templates * * * * * * * * * * * * * * * * *//
template<typename T>
bool pFlow::iIstream::findKeywordAndVal(const word& keyword, T& val, bool checkEndStatement)
{
    if( findToken(keyword) )
    {
        iIstream& is = *this;
        is >> val;

        if( checkEndStatement )
        {
            token next(is);

            if( !next.good() || !next.isEndStatement() )
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}


template<typename T>
T pFlow::iIstream::lookupData
(
    const word& keyword
)
{   
    T val;
    if( !findKeywordAndVal(keyword, val) )
    {
        ioErrorInFile( name(), lineNumber() )<<
        " error in finding keyword "<< keyword <<" and value next to it.";
        fatalExit;
    }

    return val;
}

template<typename T>
T pFlow::iIstream::lookupDataOrSet(const word& keyword, const T& setVal)
{
    T val;

    if(!findKeywordAndVal(keyword, val))
    {
        val = setVal;
    }

    return val;
}

template<typename T>
bool pFlow::iIstream::nextData(const word& keyword, T& data)
{

    token next;

    if( !eof() && good() )
    {
        read(next);
        if(next.error()) 
        {
            ioErrorInFile(name(), lineNumber());
            return false;
        }

        if( !next.isWord() )
        {   
            ioErrorInFile(name(), lineNumber())<<
            "  expected word token.";
            return false;
        }
        if(next.wordToken() != keyword) 
        {
            ioErrorInFile(name(), lineNumber())<<
            "  expected keyword "<< keyword << " but found "<<next.wordToken()<<endl;
            return false;   
        }

        iIstream& is = *this;
        is >> data;

        read(next);

        if( !next.good() || !next.isEndStatement() )
        {
            ioErrorInFile(name(),lineNumber())<<
            "  expected ; but found "<< next<<endl;
            return false;
        }

        return true;
        
    }

    return false;

}

// * * * * * * * * * * * * * * IO operations * * * * * * * * * * * * * * * * *// 
inline pFlow::iIstream& pFlow::operator>>(iIstream& is, word & w)
{
    token t(is);
    if (!t.good())
    {
        ioErrorInFile(is.name(), is.lineNumber())
            << "Bad token - could not get word/string value";
            fatalExit;
        is.setBad();
        return is;
    }

    if (t.isString())
    {
        w = t.stringToken(); 
    }
    else if(t.isWord() )
    {
        w = t.wordToken();
    }
    else
    {
        ioErrorInFile(is.name(), is.lineNumber())
            << "Wrong token type - expected word/string value, found "
            << t;
            fatalExit;
        is.setBad();
        return is;
    }

    is.check(FUNCTION_NAME);
    return is;
}

inline pFlow::iIstream& pFlow::operator>>( iIstream& is, int64& val)
{
    token t(is);

    if (!t.good())
    {
        ioErrorInFile(is.name(), is.lineNumber())
            << "Bad token - could not get int64 value";
            fatalExit;
        is.setBad();
        return is;
    }

    if (t.isNumber())
    {
        val = t.number();
    }
    else
    {
        ioErrorInFile(is.name(), is.lineNumber())
            << "Wrong token type - expected int64 value, found "
            << t;
            fatalExit;
        is.setBad();
        return is;
        
    }

    is.check(FUNCTION_NAME);
    return is;
}

inline pFlow::iIstream& pFlow::operator>>( iIstream& is, int32& val)
{
    int64 lval(0);
    is>>lval;
    val = static_cast<int32>(lval);
    return is;
}


inline pFlow::iIstream& pFlow::operator>>( iIstream& is, int8& val)
{
    int64 lval(0);
    is>>lval;
    val = static_cast<int8>(lval);
    return is;
}

inline pFlow::iIstream& pFlow::operator>>( iIstream& is, uint64& val)
{
    int64 lval(0);
    is>>lval;
    val = static_cast<uint64>(lval);
    return is;
}

inline pFlow::iIstream& pFlow::operator>>( iIstream& is, uint32& val)
{
    int64 lval(0);
    is>>lval;
    val = static_cast<uint32>(lval);
    return is;
}

inline pFlow::iIstream& pFlow::operator>>( iIstream& is, uint8& val)
{
    int64 lval(0);
    is>>lval;
    val = static_cast<uint8>(lval);
    return is;
}


inline pFlow::iIstream& pFlow::operator>>( iIstream& is, float& val)
{
    token t(is);

    if (!t.good())
    {
        ioErrorInFile(is.name(), is.lineNumber())
            << "Bad token - could not get float value";
            fatalExit;
        is.setBad();
        return is;
    }

    if (t.isNumber())
    {
        val = t.number();
    }
    else
    {
        ioErrorInFile(is.name(), is.lineNumber())
            << "Wrong token type - expected float value, found "
            << t;
            fatalExit;
        is.setBad();
        return is;
    }

    is.check(FUNCTION_NAME);
    return is;
}

inline pFlow::iIstream& pFlow::operator>>( iIstream& is, double& val)
{
    token t(is);
    if (!t.good())
    {
        ioErrorInFile(is.name(), is.lineNumber())
            << "Bad token - could not get double value";
            fatalExit;
        is.setBad();
        return is;
    }

    if (t.isNumber())
    {
        val = t.number();
    }
    else
    {
        ioErrorInFile(is.name(), is.lineNumber())
            << "Wrong token type - expected double value, found "
            << t;
            fatalExit;
        is.setBad();
        return is;
    }

    is.check(FUNCTION_NAME);
    return is;
}

inline pFlow::iIstream& pFlow::operator>>( iIstream& is, size_t& val)
{
    token t(is);
    if (!t.good())
    {
        ioErrorInFile(is.name(), is.lineNumber())
            << "Bad token - could not get double value";
            fatalExit;
        is.setBad();
        return is;
    }

    if (t.isNumber())
    {
        val = static_cast<size_t>(t.number());
    }
    else
    {
        ioErrorInFile(is.name(), is.lineNumber())
            << "Wrong token type - expected double value, found "
            << t;
            fatalExit;
        is.setBad();
        return is;
    }

    is.check(FUNCTION_NAME);
    return is;
}



