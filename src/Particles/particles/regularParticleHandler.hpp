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
#ifndef __regularParticleHandler_hpp__
#define __regularParticleHandler_hpp__

#include "particleIdHandler.hpp"

namespace pFlow
{

class regularParticleHandler
:
    public particleIdHandler
{
private:
    
    uint32 maxId_ = -1;

public:

    TypeInfo("particleIdHandler<regular>");

    explicit regularParticleHandler(uint32PointField_D & id);

    ~regularParticleHandler()override = default;

	add_vCtor
	(
		particleIdHandler,
		regularParticleHandler,
		pointField
	);

    Pair<uint32, uint32> getIdRange(uint32 nNewParticles)override;

	bool initialIdCheck()override;

};

}


#endif //__regularParticleHandler_hpp__