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

#ifndef __sphereShape_H__
#define __sphereShape_H__

#include "Vectors.H"
#include "hashMap.H"

namespace pFlow
{

class dictionary;

class sphereShape
{
protected:
	
	// - diameter of spheres
	realVector 			diameters_;
	
	// - property name of spheres
	wordVector 			materials_;

	// - hashed list of spheres names
	wordHashMap<uint32> names_;

	size_t 				numShapes_;


	bool insertNames(const wordVector& names);

	bool readDictionary(const dictionary& dict);

	bool writeDictionary(dictionary& dict)const;

public:

	// - type info
	TypeNameNV("sphereShape");


		sphereShape(){}

		sphereShape(
			const realVector& diameter,
			const wordVector& property,
			const wordVector& name
			);

		sphereShape(const sphereShape&) = default;

		sphereShape(sphereShape&&) = default;

		sphereShape& operator=(const sphereShape&) = default;

		sphereShape& operator=(sphereShape&&) = default;

		auto clone()const
		{
			return makeUnique<sphereShape>(*this);
		}

		sphereShape* clonePtr()const
		{
			return new sphereShape(*this);
		}

		~sphereShape() = default;

	//// - Methods
		const auto& names()const{
			return names_;
		}

		const auto& diameters()const{
			return diameters_;
		}

		const auto& materials()const{
			return materials_;
		}

		const auto diameter(label i)const{
			return diameters_[i];
		}

		const auto material(label i)const{
			return materials_[i];
		}


		// name to index
		bool nameToIndex(const word& name, uint32& index)const
		{
			if(auto[iter, found] = names_.findIf(name); found )
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

		uint32 nameToIndex(const word& name)const
		{
			return names_.at(name);
		}

		bool indexToName(uint32 i, word& name)const
		{
			for(auto& nm: names_)
			{
				if(nm.second == i)
				{
					name = nm.first;
					return true;
				}
			}
			name = "";
			return false;
		}

		bool shapeToDiameter(wordVector& names, realVector& diams)const;

		void diameterMinMax(real& minD, real& maxD)const
		{
			minD = min(diameters_);
			maxD = max(diameters_);
		}

	//// - IO operatoin 

		// - read from stream/file
		bool read(iIstream& is);

		// - write to stream/file
		bool write(iOstream& os)const;

		// - read from dictionary 
		bool read(const dictionary& dict)
		{
			return readDictionary(dict);
		}

		// - write to dictionary 
		bool write(dictionary& dict)const
		{
			return writeDictionary(dict);
		}


};

} // pFlow

#endif //__sphereShape_H__
