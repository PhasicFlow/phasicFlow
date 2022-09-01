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

template<typename T, typename... Args>
auto pFlow::IOobject::make
(
	const objectFile& objf,
	Args&&... args
)
{
	return makeUnique<IOobject>
		(
			objf, nullptr , make_object_t<T>(std::forward<Args>(args)...)
		);
}

template<typename T, typename... Args>
auto pFlow::IOobject::make_object_t(Args&&... args)
{
	auto ptr = makeUnique<object_t<T>>(std::forward<Args>(args)...);
	return ptr;
}

template<typename T>
auto& pFlow::IOobject::getObject()
{
	if( !isObjectValid() )
	{
		fatalErrorInFunction<<
		"accessing an invalid objecct "<< name() <<endl;
		fatalExit;
	}
	return  dynamic_cast<object_t<T>&>(*object_).data_;
}


template<typename T>
const auto& pFlow::IOobject::getObject()const
{
	if( !isObjectValid() )
	{
		fatalErrorInFunction<<
		"accessing an invalid objecct "<< name() <<endl;
		fatalExit;
	}
	return  dynamic_cast<const object_t<T>&>(*object_).data_;	
}