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

#include "PostprocessComponentGaussian.hpp"
#include "PostprocessComponentUniform.hpp"
#include "PostprocessComponentArithmetic.hpp"

// region types 
#include "sphereRegionPoints.hpp"
#include "lineRegionPoints.hpp"
#include "multipleSpheresRegionPoints.hpp"

namespace pFlow::postprocessData
{
template class PostprocessComponentGaussian<sphereRegionPoints>;
template class PostprocessComponentUniform<sphereRegionPoints>;
template class PostprocessComponentArithmetic<sphereRegionPoints>;

template class PostprocessComponentGaussian<multipleSpheresRegionPoints>;
template class PostprocessComponentUniform<multipleSpheresRegionPoints>;
template class PostprocessComponentArithmetic<multipleSpheresRegionPoints>;

template class PostprocessComponentGaussian<lineRegionPoints>;
template class PostprocessComponentUniform<lineRegionPoints>;
template class PostprocessComponentArithmetic<lineRegionPoints>;

}