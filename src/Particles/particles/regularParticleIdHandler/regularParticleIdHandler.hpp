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
#ifndef __regularParticleIdHandler_hpp__
#define __regularParticleIdHandler_hpp__

#include "particleIdHandler.hpp"

namespace pFlow
{

class regularParticleIdHandler
:
    public particleIdHandler
{
private:
    
    uint32 maxId_ = static_cast<uint32>(-1);

  bool initialIdCheck()override; 
public:

    ClassInfo("particleIdHandler<regular>");
    
    explicit regularParticleIdHandler(pointStructure& pStruct);

    ~regularParticleIdHandler()override = default;

	add_vCtor
	(
		particleIdHandler,
		regularParticleIdHandler,
		pointStructure
	);

    Pair<uint32, uint32> getIdRange(uint32 nNewParticles)override;

	uint32 maxId()const override
	{
		return maxId_;
	}

};

}


#endif //__regularParticleIdHandler_hpp__