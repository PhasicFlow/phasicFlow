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
#include "Istream.hpp"
#include "token.hpp"
#include "error.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Truncate error message for readability
static constexpr const unsigned errLen = 80;


namespace
{

// Convert a single character to a word with length 1
inline static pFlow::word charToWord(char c)
{
    return pFlow::word(std::string(1, c), false);
}

// Permit slash-scoping of entries
static inline bool validVariableChar(char c)
{
    return (pFlow::validWord(c) || c == '/');
}

} // End anonymous namespace


char pFlow::Istream::nextValid()
{
    char c = 0;

    while (true)
    {
        // Get next non-whitespace character
        while (get(c) && isspace(c))
        {}

        // Return if stream is bad - ie, previous get() failed
        if (bad() || isspace(c))
        {
            return 0;
        }

        // Is this the start of a C/C++ comment?
        if (c == '/')
        {
            if (!get(c))
            {
                // Cannot get another character - return this one
                return '/';
            }

            if (c == '/')
            {
                // C++ style single-line comment - skip through past end-of-line
                while (get(c) && c != '\n')
                {}
            }
            else if (c == '*')
            {
                // Within a C-style comment
                while (true)
                {
                    // Search for end of C-style comment - '*/'
                    if (get(c) && c == '*')
                    {
                        if (get(c))
                        {
                            if (c == '/')
                            {
                                // matched '*/'
                                break;
                            }
                            else if (c == '*')
                            {
                                // check again
                                putback(c);
                            }
                        }
                    }

                    if (!good())
                    {
                        return 0;
                    }
                }
            }
            else
            {
                // The '/' did not start a C/C++ comment - return it
                putback(c);
                return '/';
            }
        }
        else
        {
            // A valid character - return it
            return c;
        }
    }

    return 0;
}


void pFlow::Istream::readWordToken(token& t)
{
    word val;
    if (read(val).bad())
    {
        t.setBad();
    }
    else
    {
        t = std::move(val); // Move contents to token
    }
}


pFlow::Istream& pFlow::Istream::readVariable(word& str)
{
    constexpr const unsigned maxLen = 1024;
    static char buf[maxLen];

    unsigned nChar = 0;
    unsigned depth = 0; // Track depth of (..) or {..} nesting
    char c;

    // First character must be '$'
    if (!get(c) || c != token::DOLLAR)
    {
        ioErrorInFile( name(), lineNumber())
            << "Invalid first character found : " << c << nl;
            fatalExit;
    }
    buf[nChar++] = c;

    // Next character should also exist.
    // This should never fail, since it was checked before calling.
    if (!get(c))
    {
        str.assign(buf, nChar);
        warningInFunction
            << "Truncated variable name : " << str << nl;

        return *this;
    }
    buf[nChar++] = c;

    str.clear();
    if (c == token::BEGIN_BLOCK)
    {
        // Processing ${...} style.
        ++depth;

        // Could check that the next char is good and not one of '{}'
        // since this would indicate "${}", "${{..." or truncated "${"

        while (get(c))
        {
            buf[nChar++] = c;
            if (nChar == maxLen)
            {
                str.append(buf, nChar);
                nChar = 0;
            }
            if (c == token::BEGIN_BLOCK)
            {
                ++depth;
            }
            else if (c == token::END_BLOCK)
            {
                --depth;
                if (!depth)
                {
                    // Found closing '}' character
                    str.append(buf, nChar);
                    return *this;
                }
            }
        }

        // Should never reach here on normal input

        str.append(buf, nChar);  // Finalize pending buffer input

        nChar = str.length();
        if (str.length() > errLen)
        {
            str.erase(errLen);
        }

        ioErrorInFile(name(), lineNumber())
            << "stream terminated while reading variable '"
            << str.c_str() << "...' [" << static_cast<int32>(nChar) << "]\n";
            fatalExit;

        return *this;
    }
    else if (validVariableChar(c))
    {
        // Processing $var style

        while
        (
            (nChar < maxLen) && get(c)
         && (validVariableChar(c))
        )
        {
            if (c == token::BEGIN_LIST)
            {
                ++depth;
            }
            else if (c == token::END_LIST)
            {
                if (!depth)
                {
                    break;  // Closed ')' without an opening '(' ? ... stop
                }
                --depth;
            }

            buf[nChar++] = c;
        }
    }
    else
    {
        // Invalid character. Terminate string (for message) without
        // including the invalid character in the count.

        buf[nChar--] = '\0';

        warningInFunction
            << "Bad variable name: " << buf << nl << endl;
    }

    if (nChar >= maxLen)
    {
        buf[errLen] = '\0';

        ioErrorInFile(name(), lineNumber())
            << "variable '" << buf << "...'\n"
            << "    is too long (max. " << static_cast<int32>(maxLen) << " characters)";
            fatalExit;

        return *this;
    }

    buf[nChar] = '\0';  // Terminate string

    if (bad())
    {
        // Could probably skip this check
        buf[errLen] = '\0';

        ioErrorInFile(name(), lineNumber())
            << "Problem while reading variable '" << buf << "...' after "
            << static_cast<int32>(nChar) << " characters\n";
            fatalExit;

        ioErrorInFile(name(), lineNumber());

        return *this;
    }

    if (depth)
    {
        warningInFunction
            << "Missing " << static_cast<int32>(depth)
            << " closing ')' while parsing" << nl << nl
            << buf << nl << endl;
    }

    // Finalize
    str.assign(buf, nChar);
    putback(c);

    return *this;
}


pFlow::Istream::Istream
(
    std::istream& is,
    const word& streamName,
    writeFormat wf
)
:
    iIstream(wf),
    name_(streamName),
    is_(is)
{
    if (is_.good())
    {
        setOpened();
        setGood();
    }
    else
    {
        setState(is_.rdstate());
    }
}

pFlow::Istream& pFlow::Istream::get(char& c)
{
    is_.get(c);
    setState(is_.rdstate());

    if (good() && c == '\n')
    {
        ++lineNumber_;
    }

    return *this;
}


int pFlow::Istream::peek()
{
    return is_.peek();
}


pFlow::Istream& pFlow::Istream::getLine(std::string& str, char delim)
{
    std::getline(is_, str, delim);
    setState(is_.rdstate());

    if (delim == '\n')
    {
        ++lineNumber_;
    }

    return *this;
}


std::streamsize pFlow::Istream::getLine(std::nullptr_t, char delim)
{
    is_.ignore(std::numeric_limits<std::streamsize>::max(), delim);
    setState(is_.rdstate());

    std::streamsize count = is_.gcount();

    if (count && delim == '\n')
    {
        ++lineNumber_;
    }

    return count;
}


pFlow::Istream& pFlow::Istream::putback(const char c)
{
    if (c == '\n')
    {
        --lineNumber_;
    }

    if (!is_.putback(c))
    {
        setBad();
    }

    setState(is_.rdstate());

    return *this;
}

pFlow::iIstream& pFlow::Istream::read(token& t)
{
    constexpr const unsigned maxLen = 128; // Max length for units/scalars
    static char buf[maxLen];

    // Return the put back token if it exists
    if (Istream::getBack(t))
    {
        return *this;
    }

    // Assume that the streams supplied are in working order.
    // Lines are counted by '\n'

    // Get next 'valid character': i.e. proceed through any whitespace
    // and/or comments until a semantically valid character is found

    char c = nextValid();

    // Set the line number of this token to the current stream line number
    t.lineNumber() = lineNumber();

    // Return on error
    if (!c)
    {
        t.setBad();
        return *this;
    }

    // Analyse input starting with this character.
    switch (c)
    {
        // Check for punctuation first - same as token::isseparator()

        case token::END_STATEMENT :
        case token::BEGIN_LIST :
        case token::END_LIST :
        case token::BEGIN_SQR :
        case token::END_SQR :
        case token::BEGIN_BLOCK :
        case token::END_BLOCK :
        case token::COLON :
        case token::COMMA :
        case token::DIVIDE :
        {
            t = token::punctuationToken(c);
            return *this;
        }

        // String: enclosed by double quotes.
        case token::BEGIN_STRING :
        {
            putback(c);

            word val;
            if (readString(val).bad())
            {
                t.setBad();
            }
            else
            {
                t = std::move(val); // Move contents to token
            }

            return *this;
        }
        // Dictionary variable (as rvalue)
        case token::DOLLAR :
        {
            char nextC;
            if (read(nextC).bad())
            {
                // Return lone '$' as word
                t = charToWord(c);
            }
            else
            {
                // Put back both so that '$...' is included in the variable
                putback(nextC);
                putback(c);

                word val;
                if (readVariable(val).bad())
                {
                    t.setBad();
                }
                else
                {
                    t = std::move(val); // Move contents to token
                    t.setType(token::tokenType::VARIABLE);
                }
            }

            return *this;
        }

        // Number: integer or floating point
        //
        // ideally match the equivalent of this regular expression
        //
        //    /[-+]?([0-9]+\.?[0-9]*|\.[0-9]+)([Ee][-+]?[0-9]+)?/
        //
        case '-' :
        case '.' :
        case '0' : case '1' : case '2' : case '3' : case '4' :
        case '5' : case '6' : case '7' : case '8' : case '9' :
        {
            int64 int64Val = (c != '.'); // used as bool here

            unsigned nChar = 0;
            buf[nChar++] = c;

            // get everything that could resemble a number and let
            // readScalar determine the validity
            while
            (
                is_.get(c)
             && (
                    isdigit(c)
                 || c == '+'
                 || c == '-'
                 || c == '.'
                 || c == 'E'
                 || c == 'e'
                )
            )
            {
                if (int64Val)
                {
                    int64Val = isdigit(c);
                }

                buf[nChar++] = c;
                if (nChar == maxLen)
                {
                    // Runaway argument - avoid buffer overflow
                    buf[maxLen-1] = '\0';

                    ioErrorInFile( name(), lineNumber())
                        << "number '" << buf << "...'\n"
                        << "    is too long (max. " << 
                        static_cast<int32>(maxLen) << " characters)";
                        fatalExit;

                    t.setBad();
                    return *this;
                }
            }
            buf[nChar] = '\0';  // Terminate string

            setState(is_.rdstate());
            if (is_.bad())
            {
                t.setBad();
            }
            else
            {
                is_.putback(c);

                if (nChar == 1 && buf[0] == '-')
                {
                    // A single '-' is punctuation
                    t = token::punctuationToken(token::SUBTRACT);
                }
                else if (int64Val && readInt64(buf, int64Val))
                {
                    t = int64Val;
                }
                else
                {
                    real realVal;
                    
                    if (readReal(buf, realVal))
                    {
                        // A scalar or too big to fit as a unit
                        t = realVal;
                    }
                    else
                    {
                        t.setBad();
                    }
                }
            }

            return *this;
        }

        // Should be a word (which can also be a single character)
        default:
        {
            putback(c);
            readWordToken(t);

            return *this;
        }
    }
}


pFlow::iIstream& pFlow::Istream::read(char& c)
{
    c = nextValid();
    return *this;
}


pFlow::iIstream& pFlow::Istream::read(word& str)
{
    
    constexpr const unsigned maxLen = 1024;
    static char buf[maxLen];

    unsigned nChar = 0;
    unsigned depth = 0;  // Track depth of (..) nesting
    char c;

    while
    (
        (nChar < maxLen)
     && get(c)
     && validWord(c)
    )
    {
        if (c == token::BEGIN_LIST)
        {
            ++depth;
        }
        else if (c == token::END_LIST)
        {
            if (!depth)
            {
                break;  // Closed ')' without an opening '(' ? ... stop
            }
            --depth;
        }

        buf[nChar++] = c;
    }

    if (nChar >= maxLen)
    {
        buf[errLen] = '\0';
        ioErrorInFile(name(), lineNumber())
            << "word '" << buf << "...'\n"
            << "    is too long (max. " << 
            static_cast<int32>(maxLen) << " characters)";
            fatalExit;

        return *this;
    }

    buf[nChar] = '\0';  // Terminate string

    if (bad())
    {
        // Could probably skip this check
        buf[errLen] = '\0';

        ioErrorInFile(name(), lineNumber())
            << "Problem while reading word '" << buf << "...' after "
            << static_cast<int32>(nChar) << " characters\n";
            fatalExit;

        return *this;
    }

    if (nChar == 0)
    {
        ioErrorInFile(name(), lineNumber())
            << "Invalid first character found : " << c;
            fatalExit;
    }
    else if (depth)
    {
        warningInFunction
            << "Missing " << static_cast<int32>(depth)
            << " closing ')' while parsing" << nl << nl
            << buf << nl << endl;
    }

    // Finalize: content already validated, assign without additional checks.
    str.assign(buf, nChar);
    putback(c);

    return *this;
}


pFlow::iIstream& pFlow::Istream::readString(word& str)
{
    constexpr const unsigned maxLen = 1024;
    static char buf[maxLen];

    char c;

    if (!get(c))
    {
        ioErrorInFile(name(), lineNumber())
            << "cannot read start of string";
            fatalExit;

        return *this;
    }

    // Note, we could also handle single-quoted strings here (if desired)
    if (c != token::BEGIN_STRING)
    {
        ioErrorInFile(name(), lineNumber())
            << "Incorrect start of string character found : " << c;
            fatalExit;

        return *this;
    }

    unsigned nChar = 0;
    bool escaped = false;

    while
    (
        (nChar < maxLen)
     && get(c)
    )
    {
        if (c == token::END_STRING)
        {
            if (escaped)
            {
                escaped = false;
                --nChar;    // Overwrite backslash
            }
            else
            {
                // Done reading
                str.assign(buf, nChar);
                return *this;
            }
        }
        else if (c == token::NL)
        {
            if (escaped)
            {
                escaped = false;
                --nChar;    // Overwrite backslash
            }
            else
            {
                buf[errLen] = buf[nChar] = '\0';

                ioErrorInFile(name(), lineNumber())
                    << "found '\\n' while reading string \""
                    << buf << "...\"";
                    fatalExit;

                return *this;
            }
        }
        else if (c == '\\')
        {
            escaped = !escaped;    // toggle state (retains backslashes)
        }
        else
        {
            escaped = false;
        }

        buf[nChar++] = c;
    }

    if (nChar >= maxLen)
    {
        buf[errLen] = '\0';

        ioErrorInFile(name(), lineNumber())
            << "string \"" << buf << "...\"\n"
            << "    is too long (max. " << static_cast<int32>(maxLen) << " characters)";
            fatalExit;

        return *this;
    }

    // Don't worry about a dangling backslash if string terminated prematurely
    buf[errLen] = buf[nChar] = '\0';

    ioErrorInFile(name(), lineNumber())
        << "Problem while reading string \"" << buf << "...\"";
        fatalExit;

    return *this;
}

pFlow::iIstream& pFlow::Istream::read(int64& val)
{
    is_ >> val;
    setState(is_.rdstate());
    return *this;
}

pFlow::iIstream& pFlow::Istream::read(int32& val)
{
    is_ >> val;
    setState(is_.rdstate());
    return *this;
}


pFlow::iIstream& pFlow::Istream::read(int8& val)
{
    is_ >> val;
    setState(is_.rdstate());
    return *this;
}

pFlow::iIstream& pFlow::Istream::read(uint64& val)
{
    is_ >> val;
    setState(is_.rdstate());
    return *this;
}

pFlow::iIstream& pFlow::Istream::read(uint32& val)
{
    is_ >> val;
    setState(is_.rdstate());
    return *this;
}

pFlow::iIstream& pFlow::Istream::read(uint8& val)
{
    is_ >> val;
    setState(is_.rdstate());
    return *this;
}

pFlow::iIstream& pFlow::Istream::read(float& val)
{
    is_ >> val;
    setState(is_.rdstate());
    return *this;
}


pFlow::iIstream& pFlow::Istream::read(double& val)
{
    is_ >> val;
    setState(is_.rdstate());
    return *this;
}

pFlow::iIstream& pFlow::Istream::read
(
    char* buffer, 
    std::streamsize count
)
{
    if ( !isBinary() )
    {
        fatalErrorInFunction<<"stream format is not binray. Stream name is "<<
        name()<<'\n';
        fatalExit;
    }

    readBegin("binaryBlock");
    is_.read(buffer, count);
    readEnd("binaryBlock");

    setState(is_.rdstate());
    return *this;
}

size_t pFlow::Istream::findBinaryBlockStart()
{
    size_t pos = 0;
    char getChar = 'a';
    unsigned char bFlag = 255;
    int numFound = 0;

    while( is_.good() && !is_.eof() )
    {
        getChar = is_.get();
        pos++;

        if( numFound <3 &&
         static_cast<unsigned char>(getChar) == bFlag ) 
        {
            numFound++;
        }
        else if(numFound == 3 && static_cast<unsigned char>(getChar) == 0 )
        {
            return pos;
        }
        else
        {
            numFound = 0;
        }
    }

    return static_cast<size_t>(-1);
}

void pFlow::Istream::rewind()
{
    lineNumber_ = 1;      // Reset line number

    stdStream().clear();  // Clear the iostate error state flags
    setGood();            // Sync local copy of iostate

    // pubseekpos() rather than seekg() so that it works with gzstream
    stdStream().rdbuf()->pubseekpos(0, std::ios_base::in);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

std::ios_base::fmtflags pFlow::Istream::flags() const
{
    return is_.flags();
}


std::ios_base::fmtflags pFlow::Istream::flags(const ios_base::fmtflags f)
{
    return is_.flags(f);
}


// ************************************************************************* //
