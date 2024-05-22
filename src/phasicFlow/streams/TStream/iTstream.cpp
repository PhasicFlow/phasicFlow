
#include "iTstream.hpp"
#include "error.hpp"
#include "iOstream.hpp"

bool pFlow::iTstream::isLastToken()
{
    return currentToken_ == tokenList_.end();
}
 
void pFlow::iTstream::setFirstToken() 
{
    currentToken_ = tokenList_.begin();
}

void pFlow::iTstream::validate()
{
	for (auto it = tokenList_.begin(); it != tokenList_.end(); )
	{
        if (!validTokenForStream(*it))
        {
            it = tokenList_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

pFlow::iTstream::iTstream
(
    const word& streamName
)
:
    iIstream(),
    name_(streamName),
    tokenList_(),
    currentToken_(tokenList_.begin())
{
    setOpened();
    setGood();
    rewind();
}

pFlow::iTstream::iTstream
(
    const word& streamName,
    const tokenList& tList
)
:
    iIstream(),
    name_(streamName),
    tokenList_(tList),
    currentToken_(tokenList_.begin())
{
    setOpened();
    setGood();
    // check for invalid tokens in the tList
    validate();
    rewind();
}

pFlow::iTstream::iTstream
(
    const word& streamName,
    tokenList&& tList
)
:
    iIstream(),
    name_(streamName),
    tokenList_(std::move(tList)),
    currentToken_(tokenList_.begin())
{
    setOpened();
    setGood();

    // check for invalid tokens in the tList
    validate();
    rewind();
}

// copy assignment from tokenList
void pFlow::iTstream::operator=(const tokenList& tList)
{
    tokenList_ = tList;
    validate();
    rewind();
}
    
// move assignment from tokenList
void pFlow::iTstream::operator=(tokenList&& tList)
{
    tokenList_ = std::move(tList);
    validate();
    rewind();
}

const pFlow::word& pFlow::iTstream::name() const
{
    return name_;
}

pFlow::word& pFlow::iTstream::name()
{
    return name_;
}


pFlow::iIstream& pFlow::iTstream::read
(
    token& t
)
{
    // Return the put back token if it exists
    if (iIstream::getBack(t))
    {
        lineNumber_ = t.lineNumber();
        setGood();
        return *this;
    }

    if ( !isLastToken() )
    {
        //t = operator[](tokenIndex_++);
        lineNumber_ = t.lineNumber();
        t = *currentToken_;
        setGood();
        currentToken_++;

        if( isLastToken() )
        {
            setEof();
        }
    }
    else
    {
        if (eof())
        {
            fatalErrorInFunction <<
            "attempt to read beyond EOF \n";
            fatalExit;
            setBad();
        }
        else
        {
            setEof();
        }

        t.reset();

        if (tokenList_.size())
        {
            t.lineNumber() = tokenList_.back().lineNumber();
        }
        else
        {
            t.lineNumber() = lineNumber();
        }
    }

    return *this;
}



pFlow::iIstream& pFlow::iTstream::read
(
    char& c
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(c);
    return *this;
}

    
pFlow::iIstream& pFlow::iTstream::read
(
    word& str
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(str);
    return *this;
}

 
pFlow::iIstream& pFlow::iTstream::readString
(
    word& str
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(str);
    return *this;
}

 
pFlow::iIstream& pFlow::iTstream::read
(
    int64& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

pFlow::iIstream& pFlow::iTstream::read
(
    int32& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

 
pFlow::iIstream& pFlow::iTstream::read
(
    int8& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

pFlow::iIstream& pFlow::iTstream::read
(
    uint64& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

pFlow::iIstream& pFlow::iTstream::read
(
    uint32& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

pFlow::iIstream& pFlow::iTstream::read
(
    uint8& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

 
pFlow::iIstream& pFlow::iTstream::read
(
    float& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

 
pFlow::iIstream& pFlow::iTstream::read
(
    double& val
)
{

    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

pFlow::iIstream& pFlow::iTstream::read
(
    char* buffer, 
    std::streamsize count
)
{
    notImplementedFunction;
    return *this;
}

void pFlow::iTstream::rewind()
{
    iIstream::resetPutBack();
    setFirstToken();
    setGood();
}

void pFlow::iTstream::seek(size_t pos) 
{
    notImplementedFunction;
}

void pFlow::iTstream::reset()
{
    iIstream::resetPutBack();
    tokenList_.clear();
    setFirstToken();
    setGood();
}

size_t pFlow::iTstream::tell() 
{
    notImplementedFunction;
    return static_cast<size_t>(-1);
}

const pFlow::tokenList& pFlow::iTstream::tokens()const
{
    return tokenList_;
}

size_t pFlow::iTstream::size()const 
{
    return tokenList_.size();
}

size_t pFlow::iTstream::numTokens()const
{
    return tokenList_.size();
}

void pFlow::iTstream::appendTokens
(
    const tokenList & tList
)
{

    for(auto& t:tList)
    {
        if(validTokenForStream(t)) tokenList_.push_back(t);
    }
    rewind();
}

void pFlow::iTstream::appendToken
(
    const token& t
)
{
    if(validTokenForStream(t)) tokenList_.push_back(t);
    rewind();
}

// ************************************************************************* //
