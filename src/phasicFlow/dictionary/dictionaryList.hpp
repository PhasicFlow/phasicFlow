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

#ifndef __dictionaryList_hpp__
#define __dictionaryList_hpp__

#include "List.hpp"
#include "dictionary.hpp"


namespace pFlow
{

class dictionaryList   
:
    public List<dictionary>
{
private:

    const dictionary& parrent_;

    word              name_;


public:
    
    TypeInfoNV("dictionaryList");

    dictionaryList(const word& name,  const dictionary& parrent)
    :
        List<dictionary>(),
        parrent_(parrent),
        name_(name)
    {}
    
    dictionaryList(const dictionaryList&) = default;

    dictionaryList(dictionaryList&&) = default;

    dictionaryList& operator = (const dictionaryList&) = default;

    dictionaryList& operator = (dictionaryList&&) = default;

    ~dictionaryList() = default;

    const word& name()const
    {
        return name_;
    }

    const word globalName()const
    {
        return parrent_.globalName();
    }

    List<dictionary>& dicts()
    {
        return *this;
    }

    const List<dictionary>& dicts()const
    {
        return *this;
    }

    const dictionary& parrent()const
    {
        return parrent_;
    }
    
};

inline
dictionaryList readDictList(const word& name, const dictionary& dict)
{
    if(!dict.containsDataEntry(name))
    {
        fatalErrorInFunction
            <<"data entry: "<< name
            <<" does not exist in dictionary "<< dict.globalName()<<endl;
    }

    dictionaryList allDicts(name, dict);
    auto dicts = dict.getVal<List<dictionary>>(name);
    for(auto& d:dicts)
    {
        allDicts.emplace_back(d.name(), dict, d);
    }

    return allDicts;
}

}



#endif