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

namespace pFlow
{

class rotatingAxis;

INLINE_FUNCTION_HD
realx3 rotate(const realx3 &p, const line& ln, real theta);

INLINE_FUNCTION_HD
realx3 rotate(const realx3& p, const rotatingAxis& ax, real dt);

INLINE_FUNCTION_HD
void rotate(realx3* p, uint32 n, const line& ln, real theta);

INLINE_FUNCTION_HD
void rotate(realx3* p, uint32 n, const rotatingAxis& ax, real dt);

}

