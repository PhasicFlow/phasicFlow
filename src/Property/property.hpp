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


#ifndef __property_hpp__
#define __property_hpp__

#include "Vectors.hpp"
#include "hashMap.hpp"
#include "fileSystem.hpp"
#include "iFstream.hpp"

namespace pFlow
{

class dictionary;

class property
{
protected:

	uniquePtr<dictionary> dict_ = nullptr;

	// - name of materials
	wordVector 		materials_;

	// - density of materials 
	realVector 		densities_;

	// fast mapping from name to index
	wordHashMap<uint32>		nameIndex_;

	// - number of properties 
	uint32 			numMaterials_ = 0;

	bool readDictionary(const dictionary& dict);

	bool writeDictionary(dictionary& dict)const;

	bool makeNameIndex();

public:

	// type info
	TypeInfoNV("property");

		// - emptry, for reading from file 
		property(){}

		property(const wordVector& materils, const realVector& densities);

		property(const fileSystem& file);	

		property(const dictionary& dict);

		property(const property& ) = default;

		property(property&& ) = default;

		property& operator= (const property&) = default;

		property& operator= (property&&) = default;

		~property() = default;

	//// - Methods 

		inline const auto& dict()const
		{
			return dict_();
		}

		inline auto numMaterials()const
		{
			return numMaterials_;
		}

		inline const auto& materials()const{
			return materials_;
		}

		// - return densities
		inline const auto& densities()const{
			return densities_;
		}

		inline const word& material(uint32 i)const
		{
			return materials_[i];
		}

		inline bool material(uint32 i, word& name)const
		{
			if(i<numMaterials_)
			{
				name = material(i);
				return true;
			}
			else
			{
				name.clear();
				return false;
			}
		}

		inline real density(uint32 i)const 
		{
			return densities_[i];
		}

		inline bool density(uint32 i, real& rho)const
		{
			if(i<numMaterials_)
			{
				rho = density(i);
				return true;
			}
			else
			{
				rho = 0.00001;
				return false;
			}	
		}
	
		inline bool nameToIndex(const word& name, uint32& idx)const
		{
			if(auto[iter, found] = nameIndex_.findIf(name); found )
			{
				idx = iter->second;
				return true;
			}
			else
			{
				idx = 0;
				return false;
			}	
		}
		
	//// - IO operatoins

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

}

#endif // __property_hpp__
