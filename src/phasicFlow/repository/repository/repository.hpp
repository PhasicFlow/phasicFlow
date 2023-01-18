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


#ifndef __repository_hpp__
#define __repository_hpp__


#include "fileSystem.hpp"
#include "Maps.hpp"
#include "Lists.hpp"
#include "IOobject.hpp"

namespace pFlow
{

class repository
{
protected:
	
	// - repository name
	word 		name_;

	// - local path of repository, relative to owner
	fileSystem localPath_;
	
	// - owner of this repository, if any
	repository* 	 owner_;

	// - sorted names of objects with object index in vector of objects
	wordMap<IOobject>  	objects_;

	// - list of repositories that this repository owns
	// - it is not a managed list of pointers!
	wordMap<repository*> repositories_;


	template <typename Type1>
		word reportTypeError (IOobject& object);

	template <typename Type>
		bool checkForObjectType(IOobject& object);

public:

	TypeInfo("repository");
	
	//// - Constructors

		// - from a name, local path, and owner 
		repository(const word& name, const fileSystem& localPath, repository* owner = nullptr);

		// - no copy
		repository(const repository&) = delete;

		// - no assignment
		repository& operator=(const repository& )= delete;

		// - destructor 
		virtual ~repository();


	//// - Methods

		// - name of repository 
		word name()const;
		
		// - local path of repository
		virtual fileSystem localPath()const;
		
		// full path of repository 
		virtual fileSystem path()const ;
		
		// - const pointer to the owner
		const repository* owner()const;
		
		// - pointer to the owner
		repository* owner();
		
		// - const ref to this repository 
		const repository& thisRepository()const;
		
		// - ref to this repository 
		repository& thisRepository();
		
		// - add rep to this repository 
		//   return false if the name already exists 
		bool addToRepository(repository* rep);
		
		// - remove rep from the list of repositories 
		bool removeRepository(repository* rep);
	

	//// - Add/remove objects 

		// - insert the object into repository if it does not exist
		//   return a refernce to underlying data object, this reference never invalidated 
		//   until it is deleted from repository.
		//   issue a fatal error if it is already exists
		//   ** one time construction and no move/copy of object ** 
		template<typename T, typename... Args>
		T& emplaceObject(const objectFile& objf, Args&&... args);
		
		// - insert the object into repository if it does not exist
		//   return a refernce to underlying data object, this reference never invalidated 
		//   until it is deleted from repository.
		// - if the object already exist, after type check(if consistent), return the 
		//   reference of the existing object and create no new object. 
		//   ** one time construction and no move/copy of object ** 
		template<typename T, typename... Args>
		T& emplaceObjectOrGet(const objectFile& objf, Args&&... args);

		// - insert the object into repository and replace if it already exists (old object is destroyed) 
		//   return a refernce to underlying data object, this reference never invalidated 
		//   until it is deleted from repository.
		//   ** one time construction and no move/copy of object ** 
		template<typename T, typename... Args>
		T& emplaceReplaceObject(const objectFile& objf, Args&&... args);
		

		// - Insert_or_replace the IOobejct into repository and change its ownership  
		//   to this repository, take effect only if the object does not belong to 
		//   any other repository
		template<typename T>
		T& insertReplaceObject(uniquePtr<IOobject>&& ptr );
		

		// - Insert_or_replace the IOobejct into repository with new objectFile and   
		//   change the ownership to this repository, take effect only if the object
		//   does not belong to any other repository
		template<typename T>
		T& insertReplaceObject(const objectFile& objf, uniquePtr<IOobject>&& ptr );
		
		// - rease an object from this repository
		bool eraseObject(const word& name)
		{
			return objects_.erase(name) == 1;
		}


	//// - lookups and queries
	
		// - check if name of object exists
		bool lookupObjectName(const word& nm)const;

		// - find the object and return the typeName of the object 
		word lookupObjectTypeName(const word& nm)const;

		// - check if name of object exists
		//   search all the repositories under the hood of Control (master repository)
		bool globalLookupObjectName(const word& nm, bool downward = false)const;

		// - check if name of the repository exists 
		bool lookupRepositoryName(const word& nm)const;
		
		// - check if name (object and repository) exists
		bool lookupName(const word nm)const;
		
		// - return number of objects 
		size_t numObjects()const;
		
		// - return number of repositories 
		size_t numRepositories()const;

		virtual
		size_t outFilePrecision() const
		{
			if(owner_)
			{
				return owner_->outFilePrecision();
			}else
			{
				return 6;	
			}
		}
		
		// - return a ref to the underlaying data in the object 
		template<typename T>
		T& lookupObject(const word& name);
		
		// - search the name and return a ref to repository
		repository& lookupRepository(const word& name);

		// list of object names in this repository 
		wordList objectNames()const;
		
		// list of repository names in this repository 
		wordList repositoryNames()const;

	//// - IO operations 
		virtual bool write(bool verbose = false) const;
		
};

}

#include "repositoryTemplates.cpp"

#endif //__repository_hpp__
