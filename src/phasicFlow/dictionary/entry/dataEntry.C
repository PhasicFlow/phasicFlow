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
// based on OpenFOAM dictionary, with some modifications/simplifications
// to be tailored to our needs


#include "dataEntry.H"
#include "dictionary.H"
#include "error.H"
#include "iIstream.H"
#include "iOstream.H"
#include "iTstream.H"
#include "oTstream.H"

pFlow::dataEntry pFlow::dataEntry::nullDataEntry;

bool pFlow::dataEntry::readDataEntry
(
	iIstream& is
)
{	
	// reset the stream (only tokens)
	tokenStream_.reset();

	int32 level = 0;
	token nextTok;

	while
	(
		!is.read(nextTok).bad() && nextTok.good() && 
		!(level == 0 && nextTok == token::END_STATEMENT)
	)
	{

		if( nextTok.isPunctuation() )
		{
			auto t = nextTok.pToken();
			if( t == token::BEGIN_LIST ||
				t == token::BEGIN_BLOCK ||
				t == token::BEGIN_SQR
				)
			{
				level ++;
			}
			else if (
				t == token::END_LIST ||
				t == token::END_BLOCK ||
				t == token::END_SQR )
			{
				level--;
			}
		}

		// checks if there are imbalance (, { or [
		if( level < 0 )
		{
			ioErrorInFile(is.name(), is.lineNumber()) <<
			"number of opening and closing ( or, [ or { does not match, closing is greater than opening \n";
			fatalExit;
		}

		// add token to tokenStream
		// iTstream will take care of invalid tokens (space, tab, new line and null)
		tokenStream_.appendToken(nextTok);
	}


	if( level )
	{
		ioErrorInFile(is.name(), is.lineNumber()) <<
		"imbalance number of ( or { or [ \n";
		fatalExit; 
	}

	is.fatalCheck(FUNCTION_NAME);
    return nextTok.good();
}


bool pFlow::dataEntry::writeDataEntry
(
	iOstream& os
)const
{
	writeKeyword(os);
	auto lastPuncBegin = false;
	auto lastPuncEnd = false;
	auto applySpace = false;

	const tokenList& tokens = tokenStream_.tokens();

	
	for(auto tok = tokens.cbegin(); tok != tokens.cend(); tok++)
	{
		if(*tok == endStatementToken()) continue;
		

		if(isBeginToken(*tok))
		{
			if(lastPuncEnd) 
			{
				os<<spaceToken();
				os << *tok;
			}
			else
			{
				os << *tok;	
			}
			lastPuncBegin = true;
			lastPuncEnd   = false;

		}
		else if(isEndToken(*tok))
		{
			if(lastPuncBegin)
			{
				 os<<spaceToken();
				 os << *tok;
			}
			else
				os << *tok;
			lastPuncEnd = true;
			lastPuncBegin = false;
		}
		else
		{
			if(!lastPuncBegin&&applySpace) os<<spaceToken();
			os << *tok;
			lastPuncEnd = false;
			lastPuncBegin = false;
			applySpace = true;			
		}
		
		
	}

	os.endEntry();

	return true;
}

pFlow::dataEntry::dataEntry()
:
	iEntry("NULL_DATAENTRY"),
	parDict_(dictionary::nullDict),
	tokenStream_(parDict_.globalName())
{}


pFlow::dataEntry::dataEntry
(
	const word& keyword,
	const dictionary& parDict
)
:
	iEntry(keyword),
	parDict_(parDict),
	tokenStream_
	(
		groupNames(parDict.globalName(), keyword)
	)
{

}


pFlow::dataEntry::dataEntry
(
	const word& keyWord,
	const dictionary& parDict,
	const iTstream& is
)
:
	iEntry(keyWord),
	parDict_(parDict),
	tokenStream_
	(
		groupNames(parDict.globalName(), keyWord),
		is.tokens()
	)
{
	tokenStream_.rewind();
}

pFlow::dataEntry::dataEntry
(
	const word& keyWord,
	const dictionary& parDict,
	iIstream& is
)
:
	iEntry(keyWord),
	parDict_(parDict),
	tokenStream_
	(
		groupNames(parDict.globalName(),keyWord)
	)
{
	// reads the entry
	if( !readDataEntry(is) )
	{
		ioErrorInFile(is.name(), is.lineNumber() ) <<
		"error in reading data entry from file \n";
		fatalExit;
	}
	tokenStream_.rewind();
}

pFlow::dataEntry::dataEntry
(
	const word& keyword,
	const dictionary& parDict,
	const token& tok
)
:
	iEntry(keyword),
	parDict_(parDict),
	tokenStream_
	(
		groupNames(parDict.globalName(),keyword)
	)
{
	
	tokenStream_.appendToken(tok);
	tokenStream_.rewind();
}	


pFlow::dataEntry::dataEntry
(
	const word& keyword,
	const dictionary& parDict,
	const dataEntry& entry
)
:
	dataEntry(keyword, parDict)
{
	tokenStream_ = entry.tokenStream_.tokens();
	tokenStream_.rewind();
}


pFlow::iTstream& pFlow::dataEntry::stream
(
)
{
	tokenStream_.rewind();
	return tokenStream_;
}


pFlow::dictionary* pFlow::dataEntry::dictPtr()
{
	fatalErrorInFunction<<
	"Request for an entry that is not a dictionray. Entry name is " << globalName() << endl;
	fatalExit;
	return nullptr;
}


const pFlow::dictionary* pFlow::dataEntry::dictPtr() const
{
	fatalErrorInFunction<<
	"Request for an entry that is not a dictionray. Entry name is " << globalName() << endl;
	fatalExit;
	return nullptr;
}

bool pFlow::dataEntry::isDictionary()const
{
	return false;
}

pFlow::word pFlow::dataEntry::globalName()const
{
	return tokenStream_.name();
}


const pFlow::dictionary& pFlow::dataEntry::parrentDict() const
{
	return parDict_;
}


pFlow::dictionary& pFlow::dataEntry::dict()
{
	fatalErrorInFunction<<
	"Request for an entry that is not a dictionray. Entry name is " << globalName() << endl;
	fatalExit;
	return const_cast<dictionary&>(parDict_);
}


const pFlow::dictionary& pFlow::dataEntry::dict() const
{
	fatalErrorInFunction<<
	"Request for an entry that is not a dictionray. Entry name is " << globalName() << endl;
	fatalExit;
	return parDict_;
}


pFlow::uniquePtr<pFlow::iEntry> pFlow::dataEntry::clone() const 
{
	return makeUnique<dataEntry>(*this);
}


pFlow::iEntry* pFlow::dataEntry::clonePtr() const 
{
	auto ptr = makeUnique<dataEntry>(*this);
	return ptr.release();
}

pFlow::uniquePtr<pFlow::iEntry> pFlow::dataEntry::clone(const dictionary& parDict) const 
{
	return makeUnique<dataEntry>(this->keyword(), parDict, *this);
}

pFlow::iEntry* pFlow::dataEntry::clonePtr(const dictionary& parDict) const 
{
	auto ptr = makeUnique<dataEntry>(this->keyword(), parDict, *this);	
	return ptr.release();
}

///
bool pFlow::dataEntry::read(iIstream& is)
{
	token tok;
	if(!readKeyword(is, keyword_, tok))
	{
		ioErrorInFile(is.name(), is.lineNumber()) <<
		"expected a valid keyword for dataEntry but found " << tok <<endl;
		fatalExit;
	}

	if(!readDataEntry(is) )
	{
		fatalErrorInFunction<<
		"unsucceful in reading dataEntry from " << is.name() <<endl;
		fatalExit;
	}
	return true;
}



bool pFlow::dataEntry::write(iOstream& os) const
{
	if( !writeDataEntry(os) )
	{
		ioErrorInFile( os.name(), os.lineNumber() );
		fatalExit;
	}
	
	return true;
}