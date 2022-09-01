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

#include "error.H"
#include "token.H"
#include "iIstream.H"
#include "iOstream.H"

namespace pFlow
{

template<class OS>
static OS& printTokenInfo(OS& os, const token& tok)
{
    os  << "on line " << tok.lineNumber() << ": ";

    switch (tok.type())
    {
        case token::tokenType::UNDEFINED:
            os << "UNDEFINED";
            warningInFunction
                << "Undefined token" << endl;
        break;

        case token::tokenType::FLAG:
            // Swallow the flag
        break;

        case token::tokenType::PUNCTUATION:
            os << tok.pToken();
        break;

        case token::tokenType::BOOL:
        case token::tokenType::INT64:
            os << tok.int64Token();
        break;

        case token::tokenType::FLOAT:
            os << tok.floatToken();
        break;

        case token::tokenType::DOUBLE:
            os <<tok.doubleToken();
        break;

        // Different behaviour for (serial/parallel) streams: preserve types
        case token::tokenType::DIRECTIVE:
        case token::tokenType::VARIABLE:
        //case token::tokenType::VERBATIMSTRING:
            os << tok.stringToken();
        break;

        case token::tokenType::WORD:
            os << tok.wordToken();
        break;

        case token::tokenType::STRING:
            os << tok.stringToken();
        break;

        case token::tokenType::ERROR:
            os << "ERROR";
            warningInFunction
                << "Error token" << endl;
        break;

        default:
            os << "UNKNOWN";
            warningInFunction
                << "Unknown token" << endl;
    }

    return os;
}

} //  pFlow


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

pFlow::token::token(iIstream& is)
:
    token()
{
    is.read(*this);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

pFlow::word pFlow::token::name() const
{
    switch (type_)
    {
        case token::tokenType::UNDEFINED: return "undefined";
        case token::tokenType::BOOL: return "bool";
        case token::tokenType::FLAG: return "flag";
        case token::tokenType::PUNCTUATION: return "punctuation";
        case token::tokenType::INT64: return "int64/int32";
        case token::tokenType::FLOAT: return "float";
        case token::tokenType::DOUBLE: return "double";
        case token::tokenType::WORD: return "word";
        case token::tokenType::DIRECTIVE: return "directive";
        case token::tokenType::STRING: return "string";
        //case token::tokenType::VERBATIM: return "verbatim";
        case token::tokenType::VARIABLE: return "variable";
        //case token::tokenType::COMPOUND: return "compound";
        case token::tokenType::ERROR: return "error";

        default:
            break;
    }

    return "unknown(" + std::to_string(int(type_)) + ")";
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

pFlow::iIstream& pFlow::operator>>(iIstream& is, token& tok)
{
    tok.reset();
    return is.read(tok);
}


pFlow::iOstream& pFlow::operator<<(iOstream& os, const token& tok)
{
    switch (tok.type_)
    {
        case token::tokenType::UNDEFINED:
            os << "UNDEFINED";
            warningInFunction
                << "Undefined token" << endl;
        break;

        case token::tokenType::FLAG:
            // Swallow the flag
        break;

        case token::tokenType::PUNCTUATION:
            os << tok.pToken();
        break;

        case token::tokenType::BOOL:
        case token::tokenType::INT64:
            os << tok.int64Token();
        break;

        case token::tokenType::FLOAT:
            os << tok.floatToken();
        break;

        case token::tokenType::DOUBLE:
            os <<tok.doubleToken();
        break;

        // Different behaviour for (serial/parallel) streams: preserve types
        case token::tokenType::DIRECTIVE:
        case token::tokenType::VARIABLE:
        //case token::tokenType::VERBATIMSTRING:
            os.write(tok);
        break;

        case token::tokenType::WORD:
            os << tok.wordToken();
        break;

        case token::tokenType::STRING:
            os << tok.stringToken();
        break;

        case token::tokenType::ERROR:
            os << "ERROR";
            warningInFunction
                << "Error token" << endl;
        break;

        default:
            os << "UNKNOWN";
            warningInFunction
                << "Unknown token" << endl;
    }

    os.check(FUNCTION_NAME);
    return os;
}

pFlow::iOstream& pFlow::operator<<(iOstream& os, const token::punctuationToken& pt)
{
    return os << char(pt);
}

std::ostream& pFlow::operator<<(std::ostream& os, const token::punctuationToken& pt)
{
    return os << char(pt);
}

std::ostream& pFlow::operator<<(std::ostream& os, const token& tok)
{
	return printTokenInfo(os, tok);
}


pFlow::iOstream& pFlow::token::printInfo(iOstream& os)const
{
    return printTokenInfo(os, *this);
}

std::ostream& pFlow::token::printInfo(std::ostream& os)const
{
    return printTokenInfo(os, *this);
}


