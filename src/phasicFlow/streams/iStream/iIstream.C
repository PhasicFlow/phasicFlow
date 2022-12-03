
#include "iIstream.H"


void pFlow::iIstream::putBack(const token& tok)
{
    if (bad())
    {
        ioErrorInFile(name(), lineNumber()) <<
        "stream is bad.\n";
        fatalExit;
    }
    else if (putBack_)
    {
        fatalErrorInFunction <<
        "putBack is already full. \n";
        fatalExit;
    }
    else
    {
        putBackToken_ = tok;
        putBack_ = true;
    }
}


bool pFlow::iIstream::getBack(token& tok)
{
    
    if (bad())
    {
       fatalErrorInFunction;
       return false;
    }
    else if (putBack_)
    {
        tok = putBackToken_;
        putBack_ = false;
        return true;
    }

    return false;
}


bool pFlow::iIstream::peekBack(token& tok)
{
    if (putBack_)
    {
        tok = putBackToken_;
    }
    else
    {
        tok.reset();
    }

    return putBack_;
}

bool pFlow::iIstream::findToken( const word & w )
{
    rewind();
    token next;
    bool isFirstToken = true;

    while( !eof() && good() )
    {
        read(next);
        if(next.error()) 
        {
            fatalErrorInFunction <<
            "error in reading stream " << name() <<
            " at line number "<< lineNumber()<<endl;
            return false;
        }



        if( next.isWord() && isFirstToken)
        {   
            if(next.wordToken() == w ) return true;   
        }

        if(next.isEndStatement()|| next.isEndBlock())
            isFirstToken = true;
        else
            isFirstToken = false;
    }

    return false;
}

bool pFlow::iIstream::findTokenSilent( const word & w, int32 limitLine )
{
    rewind();
    token next;
    bool isFirstToken = true;

    while( !eof() && good() && lineNumber()<limitLine )
    {
        read(next);
        if(next.error()) 
        {
            return false;
        }

        if( next.isWord() && isFirstToken)
        {   
            if(next.wordToken() == w) return true;   
        }

        if(next.isEndStatement()|| next.isEndBlock())
            isFirstToken = true;
        else
            isFirstToken = false;
    }

    return false;   
}

bool pFlow::iIstream::findTokenAndNext
( 
    const word& w,
    word& nextW,
    bool checkEndStatement 
)
{
    if( findToken(w) )
    {
        // next token
        token next(*this);
        
        if(next.error()) 
        {
            ioErrorInFile( name(), lineNumber());
            return false;
        }
        if( next.isWord() )
        {
            nextW = next.wordToken();

            if( checkEndStatement )
            {
                read(next);

                if( !next.isEndStatement() )
                {
                    ioErrorInFile(name(), lineNumber()) <<
                    " expected ; but found  " << next;
                    return false;
                }

            }
            
            return true;
            
        }else
        {
            ioErrorInFile( name(), lineNumber() ) << 
            " token is not a word" << next <<endl;
            return false;
        }
    }

    return false;
}

bool pFlow::iIstream::findTokenAndNextSilent(const word& w, word& nextW, int32 limitLine)
{
    if( findTokenSilent(w, limitLine) )
    {
        // next token
        token next(*this);
        
        if(next.error()) 
        {
            return false;
        }

        if( next.isWord() )
        {
            nextW = next.wordToken();

            read(next);

            if( !next.isEndStatement() )
            {
                return false;
            }
                        
            return true;
            
        }else
        {
            return false;
        }
    }

    return false;   
}


bool pFlow::iIstream::readBegin(const char* funcName)
{   
    
    const token delimiter(*this);

    if (delimiter != token::BEGIN_LIST)
    {
        setBad();
        
        ioErrorInFile( name(), lineNumber() )
            << "Expected a '" << token::BEGIN_LIST
            << "' while reading " << funcName
            << ", found " << delimiter << endl;
            fatalExit;
    }

    return true;
}


bool pFlow::iIstream::readEnd(const char* funcName)
{
    const token delimiter(*this);

    if (delimiter != token::END_LIST)
    {
        setBad();
        
        ioErrorInFile( name(), lineNumber())
            << "Expected a '" << token::END_LIST
            << "' while reading " << funcName
            << ", found " 
            << delimiter <<endl;
            fatalExit;
    }

    return true;
}

bool pFlow::iIstream::readBeginSquare(const char* funcName)
{
	const token delimiter(*this);

    if (delimiter != token::BEGIN_SQR)
    {
        setBad();
        
        ioErrorInFile( name(), lineNumber())
            << "Expected a '" << token::BEGIN_SQR
            << "' while reading " << funcName
            << ", found "  
            << delimiter <<endl;
            fatalExit;
    }

    return true;
}
	    
bool pFlow::iIstream::readEndSquare(const char* funcName)
{
	const token delimiter(*this);

    if (delimiter != token::END_SQR)
    {
        setBad();
        
        ioErrorInFile( name(), lineNumber())
            << "Expected a '" << token::END_SQR
            << "' while reading " << funcName
            << ", found " 
            << delimiter <<endl;
            fatalExit;
    }

    return true;
}

char pFlow::iIstream::readBeginList(const char* funcName)
{
    const token delimiter(*this);

    if (delimiter != token::BEGIN_LIST && delimiter != token::BEGIN_BLOCK)
    {
        setBad();
        ioErrorInFile( name(), lineNumber() )
            << "Expected a '" << token::BEGIN_LIST
            << "' or a '" << token::BEGIN_BLOCK
            << "' while reading " << funcName
            << ", found " << delimiter<<endl;
            fatalExit;

        return '\0';
    }

    return delimiter.pToken();
}


char pFlow::iIstream::readEndList(const char* funcName)
{
    const token delimiter(*this);

    if (delimiter != token::END_LIST && delimiter != token::END_BLOCK)
    {
        setBad();
        
        ioErrorInFile( name(), lineNumber() )
            << "Expected a '" << token::END_LIST
            << "' or a '" << token::END_BLOCK
            << "' while reading " << funcName
            << ", found " << delimiter
            << " at stream position " << endl;
            fatalExit;

        return '\0';
    }

    return delimiter.pToken();
}


char pFlow::iIstream::readEndStatement(const char* funcName)
{
    CONSUME_PARAM(funcName);
    const token delimiter(*this);

    if (!delimiter.isEndStatement())
    {
        setBad();
        ioErrorInFile( name(), lineNumber() ) <<
        " expected ; but found " << delimiter << endl;
        fatalExit;

        return '\0';
    }

    return delimiter.pToken();
}

pFlow::iIstream& pFlow::iIstream::operator()() const
{
    if (!good())
    {
        check("iIstream::operator()");
        fatalExit;
    }

    return const_cast<iIstream&>(*this);
}