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

#include "particleIdHandler.hpp"



pFlow::particleIdHandler::particleIdHandler(int32PointField_HD & id)
{
	int32 maxID = maxActive<DeviceSide>(id);

	if( maxID != -1 	 && id.size() == 0 )
	{
		nextId_ = 0;
	}
	else if( maxID == -1 && id.size()>0 )
	{

		nextId_ = 0; 
		id.modifyOnHost();

		ForAll(i,id)
		{
			if(id.isActive(i))
			{
				id[i] = getNextId();	
			}
		}

		id.syncViews();
	}
	else if( maxID >= static_cast<int32>(id.size()) )
	{
		nextId_ = maxID + 1;
	}
	else
	{
		nextId_ = id.size();
	}
}

