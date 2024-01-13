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
/*

template<typename T, typename... Args>
T& pFlow::repository::emplaceObject(const objectFile& objf, Args&&... args)
{
	
	if( auto [iter2, success2] = objects_.findIf(objf.name()); !success2 )
	{
		auto ptr = IOobject::make_object_t<T>(std::forward<Args>(args)...);
		auto [iter, success] = objects_.emplace(std::piecewise_construct,
					 std::forward_as_tuple(objf.name()),
					 std::forward_as_tuple(objf, this, std::move(ptr) )
					 );
	
		return iter->second.template getObject<T>();
	}
	else
	{
		fatalErrorInFunction<<
		"IOobject " << objf.name() << " already exists in repository " << name() <<endl;
		fatalExit;
		return	iter2->second.template getObject<T>();
		
	}
}

template<typename T, typename... Args>
T& pFlow::repository::emplaceObjectOrGet(const objectFile& objf, Args&&... args)
{

	if(auto [iter, success] = objects_.findIf(objf.name()); !success )
	{
		return emplaceObject<T>(objf, std::forward<Args>(args)... );
	}
	else
	{
		// type check 
		if( checkForObjectType<T>( iter->second ) )
		{
			return iter->second.template getObject<T>();
		}
		else
		{
			fatalErrorInFunction<<
			"  IOobject "<< objf.name() <<" already exist in the repository "<< name() <<
			". Trying to return the existing object but there is a type mismatch. \n"<<
			reportTypeError<T>( iter->second );
			fatalExit;
			return iter->second.template getObject<T>(); // this is never executed 
		}
	}
}

template<typename T, typename... Args>
T& pFlow::repository::emplaceReplaceObject(const objectFile& objf, Args&&... args)
{
	
	eraseObject(objf.name());

	auto ptr = IOobject::make_object_t<T>(std::forward<Args>(args)...);
	auto [iter, success] = objects_.emplace(std::piecewise_construct,
					 std::forward_as_tuple(objf.name()),
					 std::forward_as_tuple(objf, this, std::move(ptr) )
					 );
	
	return iter->second.template getObject<T>();
}

template<typename T>
T& pFlow::repository::insertReplaceObject(uniquePtr<IOobject>&& ptr )
{
	if( !ptr->owner() )
	{
		eraseObject(ptr->name());
		objectFile objf( ptr() );
		
		auto [iter, success] = objects_.emplace
					(
						std::piecewise_construct,
					 	std::forward_as_tuple(ptr->name()),
					 	std::forward_as_tuple(objf, this, std::move(ptr))
					 );
		return iter->second.template getObject<T>();
	}else
	{
		return ptr().getObject<T>();
	}
}

template<typename T>
T& pFlow::repository::insertReplaceObject(const objectFile& objf, uniquePtr<IOobject>&& ptr )
{
	if( !ptr->owner() )
	{
		eraseObject(objf.name());
		
		auto [iter, success] = objects_.emplace
					(
						std::piecewise_construct,
					 	std::forward_as_tuple(objf.name()),
					 	std::forward_as_tuple(objf, this, std::move(ptr))
					 );
		return iter->second.template getObject<T>();
	}else
	{
		return ptr().getObject<T>();
	}
}


*/

template <typename Type1>
pFlow::word pFlow::repository::reportTypeError(IOobject& object)
{
	word err;
	err = "Object " + object.name() + " with type " + Type1::TYPENAME() + 
    "is requested, while the type " +
	  	object.typeName() + "is found in repository " + this->name()+".";

	return err;
}

template <typename Type>
bool pFlow::repository::checkForObjectType(IOobject& object)
{
	return Type::TYPENAME() == object.typeName();
}

template<typename T>
T& pFlow::repository::lookupObject(const word& name)
{
	if( auto [iter, success] = objects_.findIf(name); success )
    {

    	if( checkType<T>(iter->second) )
    	{
    		return static_cast<T&>(iter->second);
            

    	}else
    	{
    		fatalErrorInFunction << 
    		reportTypeError<T>(iter->second)<<endl;
    		fatalExit;
    		return static_cast<T&>(iter->second);
    	}
        
    }
    else
    {
    	fatalErrorInFunction << 
		"Object with name " << name << " is not found in repository " << this->name()<<endl <<
		"list of avaiable objest is \n" << objectNames();
        fatalExit;
        return iter->second.template getObject<T>();
    }
}