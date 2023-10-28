
#include "oTstream.hpp"
#include "error.hpp"

pFlow::oTstream::oTstream
(
    const word& nm
)
:
    iOstream(),
    name_(nm),
    tokenList_()
{
    setOpened();
    setGood();
}

pFlow::oTstream::oTstream
(
    const oTstream& src
)
:
    iOstream(src),
    name_(src.name_),
    tokenList_(src.tokenList_)
{
    setOpened();
    setGood();
}


const pFlow::tokenList& pFlow::oTstream::tokens()const
{
    return tokenList_;
}


pFlow::tokenList& pFlow::oTstream::tokens()
{
    return tokenList_;
}


bool pFlow::oTstream::write(const token& tok)
{
    if (tok.good())
    {
        append(tok);
        return true;
    }

    return false;
}


pFlow::iOstream& pFlow::oTstream::write(const char c)
{
    if (!std::isspace(c) && std::isprint(c))
    {
        // Should generally work, but need to verify corner cases
        append(token(token::punctuationToken(c)));
    }

    return *this;
}


pFlow::iOstream& pFlow::oTstream::write(const char* str)
{
    
    append(token(word(str)));

    return *this;
}


pFlow::iOstream& pFlow::oTstream::write(const word& str)
{
    append(token(str)); // tokenType::WORD

    return *this;
}


pFlow::iOstream& pFlow::oTstream::writeQuoted
(
    const word& str,
    const bool quoted
)
{
    
    append(token(str, lineNumber(),quoted)); // tokenType::STRING/WORD

    return *this;
}

pFlow::iOstream& pFlow::oTstream::write(const int64 val)
{
    append(token(val)); // tokenType::INT64

    return *this;
}

pFlow::iOstream& pFlow::oTstream::write(const int32 val)
{
    append(token(val)); // tokenType::INT64

    return *this;
}

pFlow::iOstream& pFlow::oTstream::write(const int8 val)
{
    append(token(val)); // tokenType::INT64

    return *this;
}




pFlow::iOstream& pFlow::oTstream::write(const uint64 val)
{
    append(token(static_cast<int64>(val))); // tokenType::INT64

    return *this;
}

pFlow::iOstream& pFlow::oTstream::write(const uint32 val)
{
    append(token(static_cast<int64>(val))); // tokenType::INT64

    return *this;
}

pFlow::iOstream& pFlow::oTstream::write(const uint8 val)
{
    append(token(static_cast<int64>(val))); // tokenType::INT64

    return *this;
}


pFlow::iOstream& pFlow::oTstream::write(const float val)
{
    append(token(val)); // tokenType::FLOAT

    return *this;
}


pFlow::iOstream& pFlow::oTstream::write(const double val)
{
    append(token(val)); // tokenType::DOUBLE

    return *this;
}

pFlow::iOstream& pFlow::oTstream::write(const size_t val)
{
    append(token(static_cast<int64>(val))); // tokenType::INT64

    return *this;
}


pFlow::iOstream& pFlow::oTstream::write
(
    const char* binaryData, 
    std::streamsize count
)
{
    notImplementedFunction;
    return *this;
}

void pFlow::oTstream::seek(size_t pos) 
{
    notImplementedFunction;
}

void pFlow::oTstream::append(const token& tok)
{
    
    if( validTokenForStream(tok) )
    	tokenList_.push_back(tok);
}

void pFlow::oTstream::append(const tokenList& tLisk)
{
    for(auto& e:tLisk)
    {
        append(e);
    }
}

void pFlow::oTstream::reset()
{
    this->rewind();
}

//- Rewind the output stream
void pFlow::oTstream::rewind()
{
    tokenList_.clear();
    setGood();
}
// ************************************************************************* //
