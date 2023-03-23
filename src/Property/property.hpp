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

// forward 
class dictionary;

/** 
 * property holds the pure properties of materials.
 * 
 * This class holds a list of all materials name and their densities that are 
 * used in the simulation: for walls and particles.  
 */
class property
{
protected:

	// - protected data members

		/// pointer to the dictionary, if it is constructed from a file/dictionary
		uniquePtr<dictionary> dict_ = nullptr;

		/// list of name of materials
		wordVector 		materials_;

		/// list of density of materials 
		realVector 		densities_;

		/// rapid mapping from name to index
		wordHashMap<uint32>		nameIndex_;

		/// number of materials
		uint32 			numMaterials_ = 0;

	
	// - protected member functions

		/// read from dict
		bool readDictionary(const dictionary& dict);

		/// write to dict
		bool writeDictionary(dictionary& dict)const;

		/// creates a mapp
		bool makeNameIndex();

public:

	/// Type info
	TypeInfoNV("property");

	
	// - Constructors 

		/// Emptry constructor, used for reading from a file 
		property(){}

		/// Constructe from materials and densities 
		property(const wordVector& materials, const realVector& densities);

		/// Construct from file
		property(const fileSystem& file);	

		/// Construct from dictionary dict
		property(const dictionary& dict);

		/// Default copy
		property(const property& ) = default;

		/// Default move
		property(property&& ) = default;

		/// Default copy assignment
		property& operator= (const property&) = default;

		/// Default move assignment
		property& operator= (property&&) = default;

		/// Default destructor 
		~property() = default;

	
	// - Methods 

		/// Return dictionary
		inline const auto& dict()const
		{
			return dict_();
		}

		/// Return number of materials 
		inline auto numMaterials()const
		{
			return numMaterials_;
		}

		/// Return list of material names
		inline const auto& materials()const{
			return materials_;
		}

		/// Return the list of densities
		inline const auto& densities()const{
			return densities_;
		}

		/// Return the material name of material i
		inline const word& material(uint32 i)const
		{
			return materials_[i];
		}

		/// Get the name of material i.
		/// Return true, if i is in the range and otherwise false
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

		/// Return density of material i
		inline real density(uint32 i)const 
		{
			return densities_[i];
		}

		/// Get the density of material i.
		/// Return true, if i is in the range and otherwise false
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
		
		/// Get the name of material in index idx
		/// Return true, if the name found, otherwise false
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

		/// Read from dictionary 
		bool read(const dictionary& dict)
		{
			return readDictionary(dict);
		}

		/// Write to dictionary 
		bool write(dictionary& dict)const
		{
			return writeDictionary(dict);
		}

};

}

#endif // __property_hpp__
