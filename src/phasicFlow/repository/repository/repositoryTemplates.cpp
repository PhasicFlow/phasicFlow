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
    		return static_cast<T&>(*iter->second);
            

    	}else
    	{
    		fatalErrorInFunction << 
    		reportTypeError<T>(*iter->second)<<endl;
    		fatalExit;
    		return static_cast<T&>(*iter->second);
    	}
        
    }
    else
    {
    	fatalErrorInFunction << 
		"Object with name " << name << " is not found in repository " << this->name()<<endl <<
		"list of avaiable objest is \n" << objectNames();
        fatalExit;
        return static_cast<T&>(*iter->second);
    }
}