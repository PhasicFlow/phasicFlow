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


#include "repository.hpp"


pFlow::repository::repository
(
	const word& name,
	const fileSystem& localPath,
	repository* owner
)
:
	name_(name),
	localPath_(localPath),
	owner_(owner)
{

	if(owner)
	{
		owner->addToRepository(this);
	}
}

pFlow::repository::~repository()
{
	if(owner_)
	{
		owner_->removeRepository(this);
	}
}

pFlow::word pFlow::repository::name()const
{
	return name_;
}

pFlow::fileSystem pFlow::repository::localPath()const
{
	return localPath_;
}

pFlow::fileSystem pFlow::repository::path()const 
{
	if(owner_)
	{
		return owner_->path()/localPath();
	}
	else
	{
		return localPath();
	}
}

const pFlow::repository* pFlow::repository::owner()const
{
	return owner_;
}

pFlow::repository* pFlow::repository::owner()
{
	return owner_;
}

const pFlow::repository& pFlow::repository::thisRepository()const
{
	return *this;
}

pFlow::repository& pFlow::repository::thisRepository()
{
	return *this;
}

bool pFlow::repository::addToRepository(repository* rep)
{
	if( !repositories_.insertIf(rep->name(), rep) )
	{

		warningInFunction<<
    	"Failed to add repository " << rep->name() <<
    	" to repository " << this->name() << 
    	". It is already in this repository. \n";
    	return false;
		
	}

	return true;
}	

bool pFlow::repository::removeRepository(repository* rep)
{
	auto name = rep->name();
	return repositories_.erase(name) == 1; 
}



bool pFlow::repository::lookupObjectName(const word& nm)const
{
	return objects_.search(nm);
}

pFlow::word pFlow::repository::lookupObjectTypeName(
	const word& nm
	)const
{
	if(auto [iter, found] = objects_.findIf(nm); found)
	{
		return iter->second.typeName();
	}
	else
	{
		fatalErrorInFunction<<
		"Object name " << nm << " is not found in repository "<< name() <<endl;
		fatalExit;
		return "";
	}
}

bool pFlow::repository::globalLookupObjectName
(
	const word& nm,
	bool  downward
)const
{	

	
	
	if(!downward)
	{	
		// the object to start search and its owner 
		auto object = this;
		auto owner = object->owner();

		// get to the top-most repository 
		while(!owner)
		{
			object = owner;
			owner = object->owner();
		}

		return object->globalLookupObjectName(nm, true);
	}

	if( lookupObjectName(nm) ) return true;

	for(auto& rep:repositories_ )
	{
		if(rep.second)
		{
			if(rep.second->globalLookupObjectName(nm, true))return true;
		}
	}

	return false;

}

bool pFlow::repository::lookupRepositoryName(const word& nm)const
{
	return repositories_.search(nm);
}

bool pFlow::repository::lookupName(const word nm)const
{
	if(lookupObjectName(nm))	return true;
	if(lookupRepositoryName(nm))return true;
	return false;
}

size_t pFlow::repository::numObjects()const
{
	return objects_.size();
}

size_t pFlow::repository::numRepositories()const
{
	return repositories_.size();
}

pFlow::repository& pFlow::repository::lookupRepository(const word& name)
{

	if( auto [iter, success] = repositories_.findIf(name); success )
    {
    	return *iter->second;   
    }
    else
    {
    	fatalErrorInFunction << 
		"repository with name " << name << " is not found in repository " << this->name()<<endl <<
		"list of avaiable repositories is \n" << repositoryNames();
        fatalExit;
        return *iter->second;
    }
}

pFlow::wordList pFlow::repository::objectNames()const
{
	wordList names;
	for(auto& ob:objects_)
	{
		names.push_back(ob.first);
	}
	return names;
}

pFlow::wordList pFlow::repository::repositoryNames()const
{
	wordList names;
	for(auto& rep:repositories_)
	{
		names.push_back(rep.first);
	}
	return names;
}


bool pFlow::repository::write
(
	bool verbose
) const
{
	
	for(auto& obj:objects_)
	{
		if(verbose)
		{
			REPORT(1)<< "Writing to " << obj.second.path()<<endREPORT;
		}
		
		if(!obj.second.write())
		{
			return false;
		}
	}

	// - write sub-repositories 
	for(auto& rep:repositories_)
	{
		
		if( rep.second )
		{
			if(! rep.second->write(verbose) )
			{
				fatalErrorInFunction<< 
				"  error in writing repository " << rep.first <<endl;
				return false;
			}
		}
		else
		{
			fatalErrorInFunction<<
			"  repository " << rep.first << " is not a valid object to be referenced. \n";
			return false;
		}
	}

	return true;
}