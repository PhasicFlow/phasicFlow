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


#include <algorithm>

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

inline pFlow::token pFlow::token::boolean(bool on)
{
    token tok;
    tok.type_ = tokenType::BOOL;
    tok.data_.int64Val = on;

    return tok;
}


inline pFlow::token pFlow::token::flag(int bitmask)
{
    token tok;
    tok.type_ = tokenType::FLAG;
    tok.data_.flagVal = bitmask;

    return tok;
}


inline bool pFlow::token::isseparator(int c)
{
    // NOTE: keep synchronized with ISstream::read(token&)

    switch (c)
    {
        case token::END_STATEMENT :
        case token::BEGIN_LIST :
        case token::END_LIST :
        case token::BEGIN_SQR :
        case token::END_SQR :
        case token::BEGIN_BLOCK :
        case token::END_BLOCK :
        case token::COLON :
        case token::COMMA :
        // Excluded token::SUBTRACT since it could start a number
        case token::DIVIDE :
        {
            return true;
        }

        default:
            break;
    }

    return false;
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline void pFlow::token::setUndefined()
{
    type_ = tokenType::UNDEFINED;
    data_.int64Val = 0; // bit-wise zero for union content
    
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline constexpr pFlow::token::token() noexcept
:
    data_(), // bit-wise zero for union content
    type_(tokenType::UNDEFINED),
    lineNumber_(0)
{}


inline pFlow::token::token(const token& tok)
:
    data_(tok.data_), // bit-wise copy of union content
    type_(tok.type_),
    lineNumber_(tok.lineNumber_)
{
    // Fundamental: values already handled by bit-wise copy
    // Pointer: duplicate content or increase refCount

    switch (type_)
    {
        case tokenType::WORD:
        case tokenType::DIRECTIVE:
        {
            data_.wordPtr = new word(*tok.data_.wordPtr);
            break;
        }

        case tokenType::STRING:
        case tokenType::VARIABLE:
        {
            data_.stringPtr = new word(*tok.data_.stringPtr);
            break;
        }
        default:
            break;
    }
}


inline pFlow::token::token(token&& tok)
:
    data_(tok.data_), // bit-wise copy of union content
    type_(tok.type_),
    lineNumber_(tok.lineNumber_)
{
    tok.setUndefined(); // zero the union content without any checking
    tok.lineNumber_ = 0;
}


inline pFlow::token::token(punctuationToken p, int32 lineNumber)
:
    data_(),
    type_(tokenType::PUNCTUATION),
    lineNumber_(lineNumber)
{
    data_.punctuationVal = p;
}

inline pFlow::token::token(const label val, int32 lineNumber)
:
    data_(),
    type_(tokenType::INT64),
    lineNumber_(lineNumber)
{
    data_.int64Val = static_cast<int64>(val);
}

inline pFlow::token::token(const uint32 val, int32 lineNumber)
:
    data_(),
    type_(tokenType::INT64),
    lineNumber_(lineNumber)
{
    data_.int64Val = static_cast<int64>(val);
}

inline pFlow::token::token(const int64 val, int32 lineNumber)
:
    data_(),
    type_(tokenType::INT64),
    lineNumber_(lineNumber)
{
    data_.int64Val = val;
}

inline pFlow::token::token(const int32 val, int32 lineNumber)
:
    data_(),
    type_(tokenType::INT64),
    lineNumber_(lineNumber)
{
    data_.int64Val = static_cast<int64>(val);
}

inline pFlow::token::token(const float val, int32 lineNumber)
:
    data_(),
    type_(tokenType::FLOAT),
    lineNumber_(lineNumber)
{
    data_.floatVal = val;
}


inline pFlow::token::token(const double val, int32 lineNumber)
:
    data_(),
    type_(tokenType::DOUBLE),
    lineNumber_(lineNumber)
{
    data_.doubleVal = val;
}


inline pFlow::token::token(const word& w, int32 lineNumber, bool isString)
:
    data_(),
    type_(tokenType::WORD),
    lineNumber_(lineNumber)
{
    if(isString)
    {
        data_.stringPtr = new word(w);
        type_ = tokenType::STRING;
    } else
    {
        data_.wordPtr = new word(w);
    }
}


inline pFlow::token::token(word&& w, int32 lineNumber, bool isString)
:
    data_(),
    type_(tokenType::WORD),
    lineNumber_(lineNumber)
{
    if(isString)
    {
        data_.stringPtr = new word(std::move(w));
        type_ = tokenType::STRING;
    } else
    {
        data_.wordPtr = new word(std::move(w));
    }
}


inline pFlow::token::~token()
{
    reset();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline void pFlow::token::reset()
{
    switch (type_)
    {
        case tokenType::WORD:
        case tokenType::DIRECTIVE:
        {
            delete data_.wordPtr;
            break;
        }

        case tokenType::STRING:
        case tokenType::VARIABLE:
        {
            delete data_.stringPtr;
            break;
        }

        default:
            break;
    }

    setUndefined();
}


inline void pFlow::token::swap(token& tok)
{
    if (this == &tok)
    {
        return;  // Self-swap is a no-op
    }

    std::swap(data_, tok.data_);
    std::swap(type_, tok.type_);
    std::swap(lineNumber_, tok.lineNumber_);
}


inline pFlow::token::tokenType pFlow::token::type() const
{
    return type_;
}


inline bool pFlow::token::setType(token::tokenType tokType)
{
    if (type_ == tokType)
    {
        // No change required
        return true;
    }

    switch (tokType)
    {
        case tokenType::BOOL:
        case tokenType::INT64:
        {
            switch (type_)
            {
                case tokenType::BOOL:
                case tokenType::INT64:
                    type_ = tokType;
                    return true;
                    break;

                default:
                    break;
            }
        }
        break;

        case tokenType::WORD:
        case tokenType::DIRECTIVE:
        {
            switch (type_)
            {
                case tokenType::WORD:
                case tokenType::DIRECTIVE:
                    type_ = tokType;
                    return true;
                    break;

                default:
                    break;
            }
        }
        break;

        case tokenType::STRING:
        case tokenType::VARIABLE:
        {
            switch (type_)
            {
                // could also go from WORD to STRING etc - to be decided
                case tokenType::STRING:
                case tokenType::VARIABLE:
                    type_ = tokType;
                    return true;
                    break;

                default:
                    break;
            }
        }
        break;

        default:
            break;
    }

    return false;
}


inline pFlow::int32 pFlow::token::lineNumber() const
{
    return lineNumber_;
}


inline pFlow::int32& pFlow::token::lineNumber()
{
    return lineNumber_;
}


inline bool pFlow::token::good() const
{
    return (type_ != tokenType::UNDEFINED && type_ != tokenType::ERROR);
}


inline bool pFlow::token::undefined() const
{
    return (type_ == tokenType::UNDEFINED);
}


inline bool pFlow::token::error() const
{
    return (type_ == tokenType::ERROR);
}


inline bool pFlow::token::isBool() const
{
    return (type_ == tokenType::BOOL);
}


inline bool pFlow::token::boolToken() const
{
    if (type_ == tokenType::BOOL || type_ == tokenType::INT64)
    {
        return data_.int64Val;
    }

    parseError("bool");
    return false;
}


inline bool pFlow::token::isFlag() const
{
    return (type_ == tokenType::FLAG);
}


inline int pFlow::token::flagToken() const
{
    if (type_ == tokenType::FLAG)
    {
        return data_.flagVal;
    }

    parseError("flag bitmask");
    return NO_FLAG;
}


inline bool pFlow::token::isPunctuation() const
{
    return (type_ == tokenType::PUNCTUATION);
}

inline bool pFlow::token::isEndStatement() const
{
    if( type_ == tokenType::PUNCTUATION )
    {
        return pToken() == punctuationToken::END_STATEMENT;
    }

    return false;
}


inline bool pFlow::token::isEndBlock() const
{
    if( type_ == tokenType::PUNCTUATION )
    {
        return pToken() == punctuationToken::END_BLOCK;
    }

    return false;
}

inline pFlow::token::punctuationToken pFlow::token::pToken() const
{
    if (type_ == tokenType::PUNCTUATION)
    {
        return data_.punctuationVal;
    }

    parseError("punctuation character");
    return punctuationToken::NULL_TOKEN;
}


inline bool pFlow::token::isSeparator() const
{
    return
    (
        type_ == tokenType::PUNCTUATION
     && isseparator(data_.punctuationVal)
    );
}


inline bool pFlow::token::isInt64() const
{
    return (type_ == tokenType::INT64);
}

inline bool pFlow::token::isInt32() const
{
    return (type_ == tokenType::INT64);
}

inline pFlow::int64 pFlow::token::int64Token() const
{
    if (type_ == tokenType::INT64)
    {
        return data_.int64Val;
    }

    parseError("int64");
    return 0;
}

inline pFlow::int32 pFlow::token::int32Token() const
{
    return static_cast<int32>(int64Token());
}

inline bool pFlow::token::isFloat() const
{
    return (type_ == tokenType::FLOAT);
}


inline float pFlow::token::floatToken() const
{
    if (type_ == tokenType::FLOAT)
    {
        return data_.floatVal;
    }

    parseError("float");
    return 0;
}


inline bool pFlow::token::isDouble() const
{
    return (type_ == tokenType::DOUBLE);
}


inline double pFlow::token::doubleToken() const
{
    if (type_ == tokenType::DOUBLE)
    {
        return data_.doubleVal;
    }

    parseError("double");
    return 0;
}


inline bool pFlow::token::isReal() const
{
    return
    (
        type_ == tokenType::FLOAT
     || type_ == tokenType::DOUBLE
    );
}


inline pFlow::real pFlow::token::realToken() const
{
    if (type_ == tokenType::FLOAT)
    {
        return data_.floatVal;
    }
    else if (type_ == tokenType::DOUBLE)
    {
        return data_.doubleVal;
    }

    parseError("real");
    return 0;
}


inline bool pFlow::token::isNumber() const
{
    return (type_ == tokenType::INT64 || isReal());
}


inline pFlow::real pFlow::token::number() const
{
    if (isInt64())
    {
        return int64Token();
    }
    if (isReal())
    {
        return realToken();
    }

    parseError("number (int64 or real)");
    return 0;
}


inline bool pFlow::token::isWord() const
{
    return
    (
        type_ == tokenType::WORD
     || type_ == tokenType::DIRECTIVE
    );
}


inline bool pFlow::token::isDirective() const
{
    return (type_ == tokenType::DIRECTIVE);
}


inline const pFlow::word& pFlow::token::wordToken() const
{
    if
    (
        type_ == tokenType::WORD
     || type_ == tokenType::DIRECTIVE
    )
    {
        return *data_.wordPtr;
    }

    parseError("word");
    return nullWord;
}

inline bool pFlow::token::isString() const
{
    return
    (
        type_ == tokenType::STRING
     || type_ == tokenType::VARIABLE
    );
}

inline const pFlow::word& pFlow::token::stringToken() const
{
    if
    (
        type_ == tokenType::STRING
     || type_ == tokenType::VARIABLE
    )
    {
        return *data_.stringPtr;
    }
    else if
    (
        type_ == tokenType::WORD
     || type_ == tokenType::DIRECTIVE
    )
    {
        // pFlow::word derives from pFlow::string, no need to cast.
        return *data_.wordPtr;
    }

    parseError("string");
    return nullWord;
}

inline bool pFlow::token::isVariable() const
{
    return (type_ == tokenType::VARIABLE);
}

inline bool pFlow::token::isStringType() const
{
    return (isWord() || isString());
}

inline void pFlow::token::setBad()
{
    reset();
    type_ = tokenType::ERROR;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void pFlow::token::operator=(const token& tok)
{
    if (this == &tok)
    {
        return;  // Self-assignment is a no-op
    }

    reset();

    type_ = tok.type_;
    data_ = tok.data_;  // bit-wise copy of union content
    lineNumber_ = tok.lineNumber_;

    // Fundamental: values already handled by bit-wise copy
    // Pointer: duplicate content or increase refCount

    switch (type_)
    {
        case tokenType::WORD:
        case tokenType::DIRECTIVE:
        {
            data_.wordPtr = new word(*tok.data_.wordPtr);
        }
        break;

        case tokenType::STRING:
        case tokenType::VARIABLE:
        {
            data_.stringPtr = new word(*tok.data_.stringPtr);
        }
        break;

        default:
            break;
    }
}


inline void pFlow::token::operator=(token&& tok)
{
    if (this == &tok)
    {
        return;  // Self-assignment is a no-op
    }

    reset();
    lineNumber_ = 0;
    swap(tok);
}


inline void pFlow::token::operator=(const punctuationToken p)
{
    reset();
    type_ = tokenType::PUNCTUATION;
    data_.punctuationVal = p;
}


inline void pFlow::token::operator=(const int64 val)
{
    reset();
    type_ = tokenType::INT64;
    data_.int64Val = val;
}

inline void pFlow::token::operator=(const int32 val)
{
    reset();
    type_ = tokenType::INT64;
    data_.int64Val = static_cast<int64>(val);
}


inline void pFlow::token::operator=(const float val)
{
    reset();
    type_ = tokenType::FLOAT;
    data_.floatVal = val;
}


inline void pFlow::token::operator=(const double val)
{
    reset();
    type_ = tokenType::DOUBLE;
    data_.doubleVal = val;
}


inline void pFlow::token::operator=(const word& w)
{
    reset();
    type_ = tokenType::WORD;
    data_.wordPtr = new word(w);
}


inline void pFlow::token::operator=(word&& w)
{
    reset();
    type_ = tokenType::WORD;
    data_.wordPtr = new word(std::move(w));
}


inline bool pFlow::token::operator==(const token& tok) const
{
    if (type_ != tok.type_)
    {
        return false;
    }

    switch (type_)
    {
        case tokenType::UNDEFINED:
            return true;

        case tokenType::BOOL:
            return data_.int64Val == tok.data_.int64Val;

        case tokenType::FLAG:
            return data_.flagVal == tok.data_.flagVal;

        case tokenType::PUNCTUATION:
            return data_.punctuationVal == tok.data_.punctuationVal;

        case tokenType::INT64:
            return data_.int64Val == tok.data_.int64Val;

        case tokenType::FLOAT:
            return equal(data_.floatVal, tok.data_.floatVal);

        case tokenType::DOUBLE:
            return equal(static_cast<real>(data_.doubleVal), static_cast<real>(tok.data_.doubleVal));

        case tokenType::WORD:
        case tokenType::DIRECTIVE:
            return *data_.wordPtr == *tok.data_.wordPtr;

        case tokenType::STRING:
        case tokenType::VARIABLE:
            return *data_.stringPtr == *tok.data_.stringPtr;

        case tokenType::ERROR:
            return true;
    }

    return false;
}


inline bool pFlow::token::operator==(const punctuationToken p) const
{
    return (type_ == tokenType::PUNCTUATION && data_.punctuationVal == p);
}


inline bool pFlow::token::operator==(const int64 val) const
{
    return
    (
        type_ == tokenType::INT64
     && data_.int64Val == val
    );
}

inline bool pFlow::token::operator==(const int32 val) const
{
    return
    (
        type_ == tokenType::INT64
     && data_.int64Val == static_cast<int64>(val)
    );
}


inline bool pFlow::token::operator==(const float val) const
{
    return
    (
        type_ == tokenType::FLOAT
     && equal(data_.floatVal, val)
    );
}


inline bool pFlow::token::operator==(const double val) const
{
    return
    (
        type_ == tokenType::DOUBLE
     && equal( static_cast<real>(data_.doubleVal), static_cast<real>(val))
    );
}

inline bool pFlow::token::operator==(const word& w) const
{
    return 
    (
        type_== tokenType::WORD
    &&  *data_.wordPtr == w 
    );
}

inline bool pFlow::token::operator!=(const token& tok) const
{
    return !operator==(tok);
}


inline bool pFlow::token::operator!=(const punctuationToken p) const
{
    return !operator==(p);
}


inline bool pFlow::token::operator!=(const int64 val) const
{
    return !operator==(val);
}

inline bool pFlow::token::operator!=(const int32 val) const
{
    return !operator==(val);
}


inline bool pFlow::token::operator!=(const float val) const
{
    return !operator==(val);
}


inline bool pFlow::token::operator!=(const double val) const
{
    return !operator==(val);
}


inline bool pFlow::token::operator!=(const word& w) const
{
    return !operator==(w);
}


// ************************************************************************* //
