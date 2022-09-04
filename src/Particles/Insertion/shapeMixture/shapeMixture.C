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


#include "shapeMixture.H"
#include "dictionary.H"


pFlow::shapeMixture::shapeMixture
(
	const dictionary & dict
)
{
	if( !read(dict))
	{
		fatalExit;
	}
}

pFlow::word pFlow::shapeMixture::getNextShapeName()
{
	
	forAll(i, names_)
	{
		if(current_[i]< number_[i])
		{
			current_[i]++;
			numberInserted_[i]++;
			return names_[i];
		}
	}
	
	fill(current_, static_cast<uint32>(0));
	return getNextShapeName();
}

void pFlow::shapeMixture::getNextShapeNameN
(
	size_t n,
	wordVector& names
)
{
	names.clear();
	

	for(label i=0; i<n; ++i)
	{
		names.push_back( getNextShapeName() );
	}
}

bool pFlow::shapeMixture::read(const dictionary& dict)
{

	bool containNumberIneserted = false;
	
	auto shNames = dict.dataEntryKeywords();

	for (auto nm = shNames.begin(); nm != shNames.end(); )
	{
        if ( *nm  == "numberInserted")
        {
        	nm = shNames.erase(nm);
        	containNumberIneserted = true;
        }
        else
            ++nm;
    }

    for(const auto& nm:shNames)
    {
    	names_.push_back(nm);
    	uint32 num = dict.getVal<uint32>(nm);
    	if( num <= 0  )
    	{
    		fatalErrorInFunction<<
    		"  number inserte in front of "<< nm << " is invalid: "<< num<<endl<<
    		"  in dictionary "<<dict.globalName()<<endl;
    		return false;
    	}
    	number_.push_back( num );
    }

    if(containNumberIneserted)
    {
    	numberInserted_ = dict.getVal<uint32Vector>("numberInserted");
    }
    else
    {
    	numberInserted_ = uint32Vector(size(), static_cast<uint32>(0));
    }

    if(numberInserted_.size() != names_.size() )
    {
    	fatalErrorInFunction<<
    	"  number of elements in numberInserted ("<<numberInserted_.size()<<
    	") is not equal to the number of shape names: "<< names_<<endl;
    	return false;
    }

    current_.clear();
    forAll(i, numberInserted_)
    {
    	current_.push_back(numberInserted_[i]%number_[i]);
    }

    return true;
}

bool pFlow::shapeMixture::write
(
	dictionary& dict
) const
{

	forAll(i, names_)
	{
		if(!dict.add(names_[i],number_[i]))
		{
			fatalErrorInFunction<<
			"  error in writing "<< names_[i] << " to dictionary "<<dict.globalName()<<endl;
			return false;
		}
	}

	if(!dict.add("numberInserted", numberInserted_))
	{
		fatalErrorInFunction<<
		"  error in writing numberInserted to dictionary "<< dict.globalName()<<endl;
		return false;
	}	

	return true;
}