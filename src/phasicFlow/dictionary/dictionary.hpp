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


#ifndef __dictionary_hpp__
#define __dictionary_hpp__

#include "types.hpp"
#include "iEntry.hpp"
#include "dataEntry.hpp"
#include "MapPtr.hpp"
#include "List.hpp"
#include "fileSystem.hpp"

namespace pFlow
{


class dictionary
:
	public iEntry
{
protected:

	//// - Data members

		// global name of dictionary, separated with dots
		word name_;

		// all the entries (data and dictionary) of the current dictionary
		wordOrderedMapPtr<iEntry> entries_;

		// entries in order of insertion
		List<iEntry*> orderedEntries_;

		// - ref to parrent dictionary
		const dictionary& parDict_;

		bool isGlobal_;


	//// - protected methods 

		// - find an entry based on keyword
		//   return nullptr if not found
		iEntry* findEntry(const word& keyword);

		// - find and entry based on keyword
		//   return nullptr if not found
		iEntry* findEntry(const word& keyword)const;

		// - reads a dataEntry with keyword from dictionary
		template<typename T>
		bool readDataEntry( const word& keyword, T& val)const;

		// - read dictionary from stream - without keyword
		bool readDictionary(iIstream & is);

		// - write dictionary to stream - with keyword
		bool writeDictionary(iOstream& os, bool withBlock = true)const;
	

public:

	// null dictionary object, to be used for references 
	static dictionary nullDict;

	TypeInfo("dictionary");

	//// - Constructors

		// - cunstructs a null dictionary
		dictionary();

		// - construct an empty dictionary with keyword
		dictionary(const word& keyword);

		// - construct an empty dictionary with keyword and make it global/fileDictionary (if true)
		dictionary(const word& keyword, bool global);

		// - construct a dictionary with name and read it from file 
		dictionary(const word& keyword, const fileSystem& file);

		// - cunstruct an empty dictionary with keyword and parDict
		dictionary(const word& keyword, const dictionary& parDict);

		// - cunstruct a dictionary with keyword and parDict and from stream
		dictionary( const word& keyword, const dictionary& parDict, iIstream& is);

		// - copy construct with keyword and new parrent dict
		//   discard the keyword and parDict of dict
		dictionary(const word& keyword, const dictionary& parDict, const dictionary& dict);

		// - copy construct as default behavior
		// 	 entries_ are copied smoothly. set parrent dict to nullDict
		dictionary(const dictionary& );


		// - assignment preserve name of this dictionary 
		// - only entries are transfered with ownership
		dictionary& operator=(const dictionary& rhs);


	//// - Methods

		// - pointer to this dictionary
		virtual dictionary* dictPtr();
		
		// - pointer to this dictionary
		virtual const dictionary* dictPtr() const;
		
		// - if this is a dictionary
		virtual bool isDictionary() const;

		// - global name of entry, separated with dots
		virtual word globalName()const;

		// - const ref to parrent dictionary 
		virtual const dictionary& parrentDict() const;

		// - ref to this dictionary, if it is a dictionary
		virtual dictionary& dict();

		// - const ref to this dictionary, if it is a dictionary
		virtual const dictionary& dict() const;

		// - if dictionary is file dictionary, return false
		virtual bool isFileDict()const;

		// - add a pointer entry (dictionary/dataEntry)
		//   replaces this entry with existing one
		bool addPtr(const word& keyword, uniquePtr<iEntry>& etry );

		// - add a float dataEntry 
		bool add(const word& keyword, const float& v);

		// - add a double dataEntry
		bool add(const word& keyword, const double& v);

		// - add a word dataEntry
		bool add(const word& keyword, const word& v);

		// - add a int64 dataEntry
		bool add(const word& keyword, const int64& v);

		// - add a int32 dataEntry
		bool add(const word& keyword, const int32& v);

		// - add a int16 dataEntry
		bool add(const word& keyword, const int16& v);

		// - add a int8 dataEntry
		bool add(const word& keyword, const int8& v);

		// - add a label dataEntry
		bool add(const word& keyword, const label& v);

		// - add a uint32 dataEntry
		bool add(const word& keyword, const uint32& v);

		// add a dictionary with the specifiedd keyword
		bool addDict(const word& keyword, const dictionary& dict);

		// - add a dataEntry of type T
		template<typename T>
		bool add(const word& keyword, const T& v );


		void clear();

		// - pointer to a subdictionary 
		//   fatalExit if not found
		dictionary* subDictPtr(const word& keyword);

		// - ref to a subdictioanry 
		//   fatalExit if not found 
		dictionary& subDict(const word& keyword);

		// - const ref to a subdictioanry 
		//   fatalExit if not found 
		const dictionary& subDict(const word& keyword) const;

		// - pointer to a dataEntry 
		//   fatalExit if not found/not a dataEntry
		dataEntry* dataEntryPtr(const word& keyword);

		// - ref to a subdictioanry 
		//   fatalExit if not found/not a dataEntry 
		dataEntry& dataEntryRef(const word& keyword);

		// - const ref to a subdictioanry 
		//   fatalExit if not found/not a dataEntry 
		const dataEntry& dataEntryRef(const word& keyword)const;

		// - search for a sub-dict with keyword
		//   create a new sub-dict if not found and return a ref to it
		//   fatalExit if fails
		dictionary& subDictOrCreate(const word& keyword);
		
		// - get the value of data entry 
		template<typename T>
		T getVal(const word& keyword) const;

		// - get the value of data entry or 
		//   if not found, set the value to setVal
		template<typename T>
		T getValOrSet(const word& keyword, const T& setVal)const;

		// return number of entris in this dictionary
		size_t numEntries()const;
		
		// return number of non-nullptr dataEntries
		size_t numDataEntries()const;

		// return number of non-nullptr dictionaries
		size_t numDictionaries()const;

		// return all keywords (non-nullptr) in this dictionary
		wordList allKeywords()const;

		// return a list of all dataEntries (non-nullptr) keywords
		wordList dataEntryKeywords()const;

		// return a list of all dictionary (non-null) keywords
		wordList dictionaryKeywords()const;

		// check if a sub-dictionary exists
		bool containsDictionay(const word& name)const;
		
		// check if a data entry exist
		bool containsDataEntry(const word& name)const;

		// clone polymorphic object (here dictionary)
		virtual uniquePtr<iEntry> clone() const;

		virtual iEntry* clonePtr() const;

		// - clone the polymorhpic object with parDict as the new parrent dictionary
		virtual uniquePtr<iEntry> clone(const dictionary& parDict)const;

		virtual iEntry* clonePtr(const dictionary& parDict) const;


	//// IO operations 

		// - read from stream
		virtual bool read(iIstream& is);

		// - write to stream
		virtual bool write(iOstream& os) const;

};



template<typename T>
bool dictionary::add(const word& keyword, const T& v )
{

	uniquePtr<iEntry> ptr = makeUnique<dataEntry>(keyword, *this ,v);
	return addPtr(keyword, ptr);
		
}


template<typename T>
bool dictionary::readDataEntry
(
	const word& keyword,
	T& val
)const 
{
	
	if( auto entry = findEntry(keyword); entry!= nullptr && !entry->isDictionary()  )
	{
		iTstream& is = dynamic_cast<dataEntry&>(*entry).stream();
	
		is >> val;
		return true;
	}
	else
	{
		return false;	
	}

}

template<typename T>
T dictionary::getVal
(
	const word& keyword
)const 
{
	T val{};
	if(!readDataEntry(keyword, val))
	{
		fatalErrorInFunction <<
		"cannot find dataEntry "<< keyword <<" in dictionary "<< globalName()<<endl;
		fatalExit;
	}
	return val;
}

template<typename T>
T dictionary::getValOrSet
(
	const word& keyword,
	const T& setVal
)const 
{
	T val{};
	if( readDataEntry(keyword, val) )
	{
		return val;
	}
	else
	{
		return setVal;
	}
}

}


#endif // __dictionary_hpp__
