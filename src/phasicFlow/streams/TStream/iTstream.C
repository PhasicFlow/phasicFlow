
#include "iTstream.H"
#include "error.H"
#include "iOstream.H"

bool dFlow::iTstream::isLastToken()
{
    return currentToken_ == tokenList_.end();
}
 
void dFlow::iTstream::setFirstToken() 
{
    currentToken_ = tokenList_.begin();
}

void dFlow::iTstream::validate()
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

dFlow::iTstream::iTstream
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

dFlow::iTstream::iTstream
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

dFlow::iTstream::iTstream
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
void dFlow::iTstream::operator=(const tokenList& tList)
{
    tokenList_ = tList;
    validate();
    rewind();
}
    
// move assignment from tokenList
void dFlow::iTstream::operator=(tokenList&& tList)
{
    tokenList_ = std::move(tList);
    validate();
    rewind();
}

const dFlow::word& dFlow::iTstream::name() const
{
    return name_;
}

dFlow::word& dFlow::iTstream::name()
{
    return name_;
}


dFlow::iIstream& dFlow::iTstream::read
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



dFlow::iIstream& dFlow::iTstream::read
(
    char& c
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(c);
    return *this;
}

    
dFlow::iIstream& dFlow::iTstream::read
(
    word& str
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(str);
    return *this;
}

 
dFlow::iIstream& dFlow::iTstream::readString
(
    word& str
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(str);
    return *this;
}

 
dFlow::iIstream& dFlow::iTstream::read
(
    int64& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

dFlow::iIstream& dFlow::iTstream::read
(
    int32& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

dFlow::iIstream& dFlow::iTstream::read
(
    int16& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}
 
dFlow::iIstream& dFlow::iTstream::read
(
    int8& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

dFlow::iIstream& dFlow::iTstream::read
(
    label& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

dFlow::iIstream& dFlow::iTstream::read
(
    uint32& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

dFlow::iIstream& dFlow::iTstream::read
(
    uint16& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

 
dFlow::iIstream& dFlow::iTstream::read
(
    float& val
)
{
    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}

 
dFlow::iIstream& dFlow::iTstream::read
(
    double& val
)
{

    notImplementedFunction;
    fatalExit;
    CONSUME_PARAM(val);
    return *this;
}



void dFlow::iTstream::rewind()
{
    iIstream::resetPutBack();
    setFirstToken();
    setGood();
}


void dFlow::iTstream::reset()
{
    iIstream::resetPutBack();
    tokenList_.clear();
    setFirstToken();
    setGood();
}

const dFlow::tokenList& dFlow::iTstream::tokens()const
{
    return tokenList_;
}

size_t dFlow::iTstream::size()const 
{
    return tokenList_.size();
}

size_t dFlow::iTstream::numTokens()const
{
    return tokenList_.size();
}

void dFlow::iTstream::appendTokens
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

void dFlow::iTstream::appendToken
(
    const token& t
)
{
    if(validTokenForStream(t)) tokenList_.push_back(t);
    rewind();
}

// ************************************************************************* //
