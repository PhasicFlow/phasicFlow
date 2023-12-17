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


#include "dictionary.hpp"
#include "uniquePtr.hpp"
#include "error.hpp"
#include "streams.hpp"



pFlow::dictionary pFlow::dictionary::nullDict;


// better to move it to the abstract class
// with an static member function. 
bool pFlow::dictionary::readDictionary
(
	iIstream & is
)
{
	// ensure that the entry list of new the dicrionary is empty
	entries_.clear();


	// empty dictionary
    if (is.eof())
    {
        return true;
    }

    if (!is.good())
    {
        ioErrorInFile( is.name(), is.lineNumber() )<<
        "iIstream is not good for reading tokens .... \n";
        return false;
    }

    
    // reads tokens one-by-one
    token nextTok(is);
    if( nextTok == token::END_BLOCK)
    {
    	ioErrorInFile( is.name(), is.lineNumber() )<<
    	"Not expecting a } for the start of a dictionray \n";
    	return false;
    }
    
    bool hasBlockToken = true;
    
    // this is a sub-dict, so it should start with {
	if(nextTok != token::BEGIN_BLOCK)
	{
		hasBlockToken = false;
		is.putBack(nextTok);
	}
   
	
	//read all entries in ths dictionary
    while ( !is.eof() && iEntry::createEntry(*this, is, hasBlockToken) )
    {}


    return true;
}

bool pFlow::dictionary::writeDictionary
(
	iOstream& os,
	bool withBlock
)const
{
	if(withBlock) os.beginBlock(keyword());

	for( const auto& e:orderedEntries_)
	{
		if(e != nullptr )
			e->write(os);
	}
	if(withBlock) os.endBlock();

	return true;

}

pFlow::iEntry* pFlow::dictionary::findEntry
(
	const word& keyword
)
{
	if( auto [ptr, exist] = entries_.find(keyword); exist)
	{

		return ptr;
	
	}

	return nullptr;	
}

pFlow::iEntry* pFlow::dictionary::findEntry
(
	const word& keyword
)const
{
	if( auto [ptr, exist] = entries_.find(keyword); exist)
	{
		return const_cast<iEntry*> (ptr);
	}

	return nullptr;	
}


pFlow::dictionary::dictionary
()
:
	iEntry("NULL_DICT"),
	name_("NULL_DICT"),
	entries_(),
	orderedEntries_(),
	parDict_(dictionary::nullDict),
	isGlobal_(false)
{}

pFlow::dictionary::dictionary
(
	const word& keyword
)
:
	iEntry(keyword),
	name_(keyword),
	entries_(),
	orderedEntries_(),
	parDict_(dictionary::nullDict),
	isGlobal_(false)
{}


pFlow::dictionary::dictionary
(
	const word& keyword,
	bool global
)
:
	iEntry(keyword),
	name_(keyword),
	entries_(),
	orderedEntries_(),
	parDict_(dictionary::nullDict),
	isGlobal_(global)
{
}

pFlow::dictionary::dictionary
(
	const word& keyword,
	const fileSystem& file
)
:
	iEntry(keyword),
	name_(file.wordPath()),
	entries_(),
	orderedEntries_(),
	parDict_(dictionary::nullDict),
	isGlobal_(true)
{
	iFstream dictStream(file);

	if(!read(dictStream))
	{
		ioErrorInFile(dictStream.name(), dictStream.lineNumber())<<
		"error in reading dictionary from file "<< file <<endl;
		fatalExit;
	}

}

pFlow::dictionary::dictionary
(
	const word& keyword,
	const dictionary& parDict
)
:
	iEntry(keyword),
	name_(groupNames(parDict.globalName(), keyword)),
	entries_(),
	orderedEntries_(),
	parDict_(parDict),
	isGlobal_(false)
{

}

pFlow::dictionary::dictionary
(
	const word& keyword,
	const dictionary& parDict,
	iIstream& is
)
:
	iEntry(keyword),
	name_(groupNames(parDict.globalName(), keyword)),
	entries_(),
	orderedEntries_(),
	parDict_(parDict),
	isGlobal_(false)
{

	if( !readDictionary(is) )
	{
		ioErrorInFile(is.name(), is.lineNumber()) <<
		"error in reading dictionary " << name_ <<endl;
		fatalExit;
	}
}

pFlow::dictionary::dictionary
(
	const word& keyword,
	const dictionary& parDict,
	const dictionary& dict
)
:
	iEntry(keyword),
	name_(groupNames(parDict.globalName(),keyword)),
	entries_(),
	orderedEntries_(),
	parDict_(parDict),
	isGlobal_(false)
{

	for( auto& entry : dict.orderedEntries_ )
	{
		if(entry)
		{
			auto ptr = entry->clone(*this); 
			auto key = entry->name();

			if( !addPtr(key, ptr) )
			{
				fatalErrorInFunction <<
				" error in cloning dicrionary / dataEntry " << entry->globalName() <<endl;
				fatalExit;
			}
		}
	}

}

pFlow::dictionary::dictionary
(
	const dictionary& src
)
:
	iEntry(src.keyword()),
	name_(src.keyword()),
	entries_(),
	orderedEntries_(),
	parDict_(dictionary::nullDict),
	isGlobal_(src.isGlobal_)
{

	for( auto& entry: src.orderedEntries_)
	{
		
		if(entry)
		{
			auto ptr = entry->clone(*this); 
			auto key = entry->name();

			if( !addPtr(key, ptr) )
			{
				fatalErrorInFunction <<
				" error in cloning dicrionary / dataEntry " << entry->globalName() <<endl;
				fatalExit;
			}
		}
	}

}

pFlow::dictionary& pFlow::dictionary::operator=
(
	const dictionary& rhs
)
{
	
	if( &rhs == this)return *this;
	
	clear();

	for( auto& entry: rhs.orderedEntries_)
	{
		
		if(entry)
		{
			auto ptr = entry->clone(*this); 
			auto key = entry->name();

			if( !addPtr(key, ptr) )
			{
				fatalErrorInFunction <<
				" error in cloning dicrionary / dataEntry " << entry->globalName() <<endl;
				fatalExit;
			}
		}
	}

	return *this;
}

pFlow::dictionary* pFlow::dictionary::dictPtr()
{
	return this;
}


const pFlow::dictionary* pFlow::dictionary::dictPtr() const
{
	return this;
}


bool pFlow::dictionary::isDictionary() const
{
	return true;
}

pFlow::word pFlow::dictionary::globalName()const
{
	return name_;
}


const pFlow::dictionary& pFlow::dictionary::parrentDict() const
{
	return parDict_;
}



pFlow::dictionary& pFlow::dictionary::dict()
{
	return *this;
}


const pFlow::dictionary& pFlow::dictionary::dict() const
{
	return *this;
}

bool pFlow::dictionary::isFileDict()const
{
	return isGlobal_;
}

bool  pFlow::dictionary::addPtr
(
	const word& keyword,
	uniquePtr<iEntry>& entry,
	bool warning
)
{

	if(entry == nullptr) return false;

	iEntry* oldEntryPtr = nullptr;
	iEntry* newEntryPtr = entry.get();

	// search all entries for repeated keyword
	if(auto [ptr, exist] = entries_.find(keyword); exist )
	{
		if(warning)
		{
			warningInFunction <<
			"keyword " << keyword << " already exists in the dicrionary " <<
			this->globalName() << ". The old entry will be replaced by the new one. \n";
		}
		
		// store the old pointer to entry
		oldEntryPtr = ptr;
	}
	
	
	if( entries_.insertReplace(keyword, entry) )
	{
	
		if(oldEntryPtr)
		{
			// this should be replaced 
			auto oIter = orderedEntries_.find(oldEntryPtr);
			*oIter = newEntryPtr;
		}
		else
		{
			orderedEntries_.push_back(newEntryPtr);
		}
		return true;
	}else
	{
		return false;
	}

}

bool pFlow::dictionary::add
(
	const word& keyword,
	const float& v
)
{
	uniquePtr<iEntry> ptr = makeUnique<dataEntry>(keyword, *this, token(v));
	return addPtr(keyword, ptr);
}

bool pFlow::dictionary::add
(
	const word& keyword,
	const double& v
)
{
	uniquePtr<iEntry> ptr = makeUnique<dataEntry>(keyword, *this, token(v));
	return addPtr(keyword, ptr);
}	

bool pFlow::dictionary::add
(
	const word& keyword,
	const word& v
)
{
	uniquePtr<iEntry> ptr = makeUnique<dataEntry>(keyword, *this, token(v));
	return addPtr(keyword, ptr);
}

bool pFlow::dictionary::add
(
	const word& keyword,
	const int64& v
)
{
	uniquePtr<iEntry> ptr = makeUnique<dataEntry>(keyword, *this, token(v));
	return addPtr(keyword, ptr);
}

bool pFlow::dictionary::add
(
	const word& keyword,
	const int32& v
)
{
	uniquePtr<iEntry> ptr = makeUnique<dataEntry>(keyword, *this, token(v));
	return addPtr(keyword, ptr);
}


bool pFlow::dictionary::add
(
	const word& keyword,
	const int8& v
)
{
	uniquePtr<iEntry> ptr = makeUnique<dataEntry>(keyword, *this, token(v));
	return addPtr(keyword, ptr);
}

bool pFlow::dictionary::add
(
	const word& keyword,
	const uint64& v
)
{
	uniquePtr<iEntry> ptr = makeUnique<dataEntry>(keyword, *this, token(v));
	return addPtr(keyword, ptr);
}

bool pFlow::dictionary::add
(
	const word& keyword,
	const uint32& v
)
{
	uniquePtr<iEntry> ptr = makeUnique<dataEntry>(keyword, *this, token(v));
	return addPtr(keyword, ptr);
}

bool pFlow::dictionary::add
(
	const word& keyword,
	const uint8& v
)
{
	uniquePtr<iEntry> ptr = makeUnique<dataEntry>(keyword, *this, token(v));
	return addPtr(keyword, ptr);
}

bool pFlow::dictionary::addDict
(
	const word& keyword,
	const dictionary& dict
)
{
	uniquePtr<iEntry> ptr = makeUnique<dictionary>(keyword, *this, dict);
	return addPtr(keyword, ptr);
}

void pFlow::dictionary::clear()
{
	orderedEntries_.clear();
	entries_.clear();
}

pFlow::dictionary* pFlow::dictionary::subDictPtr
(
	const word& keyword
)
{
	if( auto entry = findEntry(keyword); entry!= nullptr && entry->isDictionary()  )
	{
		return dynamic_cast<dictionary*>(entry);
	}
	else
	{
		fatalErrorInFunction<<
		"keyword " << keyword<< " is not an sub-dictionary of " << this->globalName()<<endl;
		fatalExit;
		return this;	
	}
}

pFlow::dictionary& pFlow::dictionary::subDict
(
	const word& keyword
)
{
	
	if( auto entry = findEntry(keyword); entry!= nullptr && entry->isDictionary()  )
	{
		return dynamic_cast<dictionary&>(*entry);
	}
	else
	{
		fatalErrorInFunction<<
		"keyword " << keyword<< " is not an sub-dictionary of " << this->globalName()<<endl;
		fatalExit;
		return *this;	
	}

}

const pFlow::dictionary& pFlow::dictionary::subDict
(
	const word& keyword
) const
{
	
	if( auto entry = findEntry(keyword); entry!= nullptr && entry->isDictionary()  )
	{
		return dynamic_cast<dictionary&>(*entry);
	}
	else
	{
		fatalErrorInFunction<<
		"keyword " << keyword<< " is not an sub-dictionary of " << this->globalName()<<endl;
		fatalExit;
		return *this;	
	}
}

pFlow::dataEntry* pFlow::dictionary::dataEntryPtr(const word& keyword)
{
	if( auto entry = findEntry(keyword); entry && !entry->isDictionary()  )
	{
		return dynamic_cast<dataEntry*>(entry);
	}
	else
	{
		fatalErrorInFunction<<
		"keyword " << keyword<< " is not a dataEntry of " << this->globalName()<<endl;
		fatalExit;
		return nullptr;	
	}
}

pFlow::dataEntry& pFlow::dictionary::dataEntryRef(const word& keyword)
{
	if( auto entry = findEntry(keyword); entry && !entry->isDictionary()  )
	{
		return dynamic_cast<dataEntry&>(*entry);
	}
	else
	{
		fatalErrorInFunction<<
		"keyword " << keyword<< " is not a dataEntry of " << this->globalName()<<endl;
		fatalExit;
		return dataEntry::nullDataEntry;	
	}
}	

const pFlow::dataEntry& pFlow::dictionary::dataEntryRef
(
	const word& keyword
)const
{
	if( auto entry = findEntry(keyword); entry && !entry->isDictionary()  )
	{
		return dynamic_cast<dataEntry&>(*entry);
	}
	else
	{
		fatalErrorInFunction<<
		"keyword " << keyword<< " is not a dataEntry of " << this->globalName()<<endl;
		fatalExit;
		return dataEntry::nullDataEntry;	
	}
}

pFlow::dictionary& pFlow::dictionary::subDictOrCreate
(
	const word& keyword
)
{
	if( auto entry = findEntry(keyword); entry!= nullptr && entry->isDictionary()  )
	{
		return dynamic_cast<dictionary&>(*entry);
	}
	else 
	{
		uniquePtr<iEntry> ptr = makeUnique<dictionary>(keyword, *this);
		if( addPtr
			(
				keyword,
				ptr
			)
		  )
		{
			return subDictOrCreate(keyword);
		}
		else
		{
			fatalErrorInFunction<<
			"Unable to create sub-dictionary "<< keyword << " in dictionary " << globalName() <<endl;
			fatalExit;
		}
	}
	return *this;
}

size_t pFlow::dictionary::numEntries()const
{
	return entries_.size();
}

size_t pFlow::dictionary::numDataEntries()const
{
	size_t num = 0;
	for(auto& e:entries_)
	{
		if( e.second && !e.second->isDictionary())
		{
			num++;
		}
	}
	return num;
}

size_t pFlow::dictionary::numDictionaries()const
{
	size_t num = 0;
	for(auto& e:entries_)
	{
		if( e.second && e.second->isDictionary())
		{
			num++;
		}
	}
	return num;	
}

pFlow::wordList pFlow::dictionary::allKeywords()const
{
	wordList wl;

	for(auto oe:orderedEntries_)
	{
		if(oe) wl.push_back( oe->keyword() );
	}
	return wl;
}

pFlow::wordList pFlow::dictionary::dataEntryKeywords()const
{
	wordList wl;

	for(auto oe:orderedEntries_)
	{
		if( oe && !oe->isDictionary())
		{
			wl.push_back(oe->keyword());
		}
	}
	return wl;
}

// return a list of all dictionary keywords
pFlow::wordList pFlow::dictionary::dictionaryKeywords()const
{
	wordList wl;

	for(auto& oe:orderedEntries_)
	{
		if( oe && oe->isDictionary())
		{
			wl.push_back(oe->keyword());
		}
	}
	return wl;
}

bool pFlow::dictionary::containsDictionay
(
	const word& name
)const
{
	if( auto  ptr = findEntry(name); ptr)
	{
		return ptr->isDictionary();
	}
	return false;
}

bool pFlow::dictionary::containsDataEntry
(
	const word& name
)const
{
	if( auto  ptr = findEntry(name); ptr)
	{
		return !ptr->isDictionary();
	}
	return false;
}

bool pFlow::dictionary::read(iIstream& is)
{
	token tok;
	if(!isFileDict() && !readKeyword(is, keyword_, tok))
	{
		ioErrorInFile(is.name(), is.lineNumber()) <<
		"expected a valid keyword for dictionary but found " << tok <<endl;
		fatalExit;
	}

	if( !readDictionary(is) )
	{
		ioErrorInFile(is.name(), is.lineNumber()) <<
		"error in reading dictionary " << keyword_ <<endl;
		fatalExit;
	}

	return true;
}


bool pFlow::dictionary::write(iOstream& os) const
{
	if(! writeDictionary(os, !isFileDict()) )
	{
		ioErrorInFile( os.name(), os.lineNumber());
		fatalExit;
	}
	return true;
}


pFlow::uniquePtr<pFlow::iEntry> pFlow::dictionary::clone() const
{
	return makeUnique<dictionary>(*this);
}


pFlow::iEntry* pFlow::dictionary::clonePtr() const
{
	
	auto ptr = makeUnique<dictionary>(*this);
	return ptr.release();
}

pFlow::uniquePtr<pFlow::iEntry> pFlow::dictionary::clone
(
	const dictionary& parDict
) const
{
	return makeUnique<dictionary>(this->keyword(), parDict, *this);
}

pFlow::iEntry* pFlow::dictionary::clonePtr
(
	const dictionary& parDict
) const
{
	auto ptr = makeUnique<dictionary>(this->keyword(), parDict, *this);
	return ptr.release();
}