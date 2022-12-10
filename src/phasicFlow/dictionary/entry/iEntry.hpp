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

#ifndef __iEntry_hpp__
#define __iEntry_hpp__


#include "types.hpp"
#include "typeInfo.hpp"
#include "uniquePtr.hpp"


namespace pFlow
{


class dictionary;

class iEntry
{

public:
	
	//// - public static methods

		// - read a keyword from stream 
		static bool readKeyword(iIstream &is, word& keyword, token& tok );

		// - create an entry (dataEntry or dictionary) from stream 
		static bool createEntry(dictionary& parDict, iIstream& is, bool hasBlockToken = false);

protected:

	// keyWord of entry
	word keyword_;

	bool writeKeyword(iOstream& os)const;

public:
	

	TypeInfo("iEntry");

	//// - Constructors

		// - empty constructor
		iEntry()
		{}

		// - construct with a keyword
		iEntry(const word& key)
		{
			// this moved here due to a very strange core dumped error!
			keyword_ = key;
		}

		// - destructor
		virtual ~iEntry()
		{}


	//// - Methods

		// - return keyword
		virtual const word& keyword() const
		{
			return keyword_;
		}

		// - return keyword
		virtual word& keyword()
		{
			return keyword_;
		}

		virtual word name()const
		{
			return keyword();
		}

		// global name of entry, separated with dots
		virtual word globalName()const = 0;

		// - pointer to this dictionary
		virtual dictionary* dictPtr()
		{
			return nullptr;
		}

		// - const pointer to this dictionary
		virtual const dictionary* dictPtr() const
		{
			return nullptr;
		}

		// - if this is a dictionary
		virtual bool isDictionary() const
		{
			return false;
		}

		// - const ref to parrent dictionary 
		virtual const dictionary& parrentDict() const = 0;
		
		// - ref to this dictionary, if it is a dictionary
		virtual dictionary& dict() = 0;

		// - const ref to this dictionary, if it is a dicrionary
		virtual const dictionary& dict() const = 0;

		// clone the object
		virtual iEntry* clonePtr() const = 0;

		virtual uniquePtr<iEntry> clone() const = 0;

		// clone the object and change its ownership to parDict 
		virtual iEntry* clonePtr(const dictionary& parDict) const = 0;

		virtual uniquePtr<iEntry> clone(const dictionary& parDict)const = 0;

	//// - IO operatoins

		// read from stream
		virtual bool read(iIstream& is) = 0;

		// write to stream
		virtual bool write(iOstream& os) const =0;

};


iOstream& operator << (iOstream& os, const iEntry& e);


iIstream& operator >> (iIstream& is, iEntry& e);


}


#endif //__iEntry_hpp__
