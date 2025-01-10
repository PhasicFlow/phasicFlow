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


#ifndef __dataEntry_hpp__
#define __dataEntry_hpp__


#include "iEntry.hpp"
#include "iTstream.hpp"
#include "oTstream.hpp"



namespace pFlow
{

class dictionary;

/**
 * Data entry to be used in dictionries. Its format is:
 * 
 * ```
 * entryName   value;
 * 
 * entryName2  (list of values);
 * ```
 */
class dataEntry
:
	public iEntry
{

protected:

	//// - data memebers

		/// ref to parrent dictionary
		const dictionary& parDict_;

		/// list the tokens as input token stream
		iTstream tokenStream_;


	//// - protected member functions

		/// read dataEntry from stream
		bool readDataEntry(iIstream& is);

		/// write dataEntry to stream
		bool writeDataEntry(iOstream& os) const;

public:

	static dataEntry nullDataEntry;

	//// - constructors

		/// construct null dataEntry
		dataEntry();

		/// construct from keyword and parDict, empty dataEntry
		dataEntry(const word& keyword, const dictionary& parDict);

		/// construct from keyword, parDict and input token stream
		dataEntry(const word& keyWord, const dictionary& parDict, const iTstream& is);

		//- construct from keyword, parDict and input stream
		dataEntry(const word& keyWord, const dictionary& parDict, iIstream& is);
		
		/// construct from keyword, parDict and a single token
		dataEntry(const word& keyword, const dictionary& parDict, const token& tok);

		/// construct from keyword, parDict, and data of type T
		template<typename T>
		dataEntry(const word& keyword, const dictionary& parDict, const T& v);

		/// copy construct with new keyword and parDict
		dataEntry(const word& keyword, const dictionary& parDict, const dataEntry& entry );
		
		/// copy construct
		dataEntry(const dataEntry& src )= default;


	//// - Methods
	
		/// global name of entry, separated with dots
		virtual word globalName()const;

		
		/// access to token stream
		virtual iTstream& stream();

		/// not permited to be called
		virtual dictionary* dictPtr();
		
		/// not permited to be called
		virtual const dictionary* dictPtr() const;
		
		/// should returen false;
		virtual bool isDictionary()const;
		
		/// const ref to parrent dictionary 
		virtual const dictionary& parrentDict() const;

		/// not permited to be called
		virtual dictionary& dict();

		/// not permited to be called
		virtual const dictionary& dict() const;

		// clone the object
		virtual iEntry* clonePtr() const;

		virtual uniquePtr<iEntry> clone() const;

		// clone the object and change its ownership to parDict 
		virtual iEntry* clonePtr(const dictionary& parDict) const;

		virtual uniquePtr<iEntry> clone(const dictionary& parDict)const;

	//// - IO operations 

		/// read from stream
		virtual bool read(iIstream& is);

		/// write to stream
		virtual bool write(iOstream& os) const;

};




template<typename T>
dataEntry::dataEntry(const word& keyword, const dictionary& parDict, const T& v)
:
	dataEntry(keyword, parDict)
{

	oTstream os("oTStream for " + globalName());

	os<< v << endStatementToken();

	tokenStream_ = std::move(os.tokens());
}


} // pFlow


#endif //__dataEntry_hpp__
