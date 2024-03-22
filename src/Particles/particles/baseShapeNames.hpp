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
#ifndef __baseShapeNames_hpp__
#define __baseShapeNames_hpp__

#include "fileDictionary.hpp"
#include "Vectors.hpp"
#include "hashMap.hpp"


namespace pFlow
{

class repository;

class baseShapeNames
:
	public fileDictionary
{
private:

	size_t 				numShapes_ = 0;

	// - hashed list of spheres names
	wordHashMap<uint32> hashNames_;

	/// list of shape names 
	wordVector 			shapeNames_{"shapeNames"};

	/// hash for names 
	Vector<size_t> 		hashes_{"hashes"};
	
	bool createHashNames();

	bool readFromDictionary1();	

	using hasher 	= typename wordHashMap<uint32>::hasher;

protected:

	virtual
	bool writeToDict(dictionary& dict)const;

public:
    
    TypeInfo("baseShapeNames");

	baseShapeNames(
		const word& fileName,
		repository* owner);

    ~baseShapeNames() override=default;
	

    inline
    const wordVector& shapeNames()const
	{
		return shapeNames_;
	}

	inline 
	wordList shapeNameList()const
	{
		wordList wl;
		wl.insert(wl.begin(), shapeNames_.begin(), shapeNames_.end());
		return wl;
	}

	inline
	const auto& hashes()const
	{
		return hashes_;
	}

    inline
    size_t numShapes()const
	{
		return numShapes_;
	}

	// name to index
	inline
	bool shapeNameToIndex(const word& name, uint32& index)const
	{
		if(auto[iter, found] = hashNames_.findIf(name); found )
		{
			index = iter->second;
			return true;
		}
		else
		{
			index = 0;
			return false;
		}
	}

	inline
	bool indexToShapeName(uint32 i, word& name)const
	{
		if( i < numShapes_)
		{
			name = shapeNames_[i];
			return true;
		}
		return false;
	}

	inline bool hashToIndex(const size_t& hs, uint32& idx)
	{
		for(auto i=0; i<hashes_.size(); i++)
		{
			if(hashes_[i]==hs)
			{
				idx = i;
				return true;
			}
		}
		idx = -1;
		return false;
	}

	inline
	bool indexValid(uint32 index)const
	{
		return index < numShapes_;
	}

	// - IO

	bool write(iOstream& os)const override;

};

}

#endif //__shapeNames_hpp__