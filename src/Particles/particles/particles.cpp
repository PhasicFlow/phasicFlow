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

#include "particles.hpp"
#include "shape.hpp"

pFlow::particles::particles(systemControl& control, const shape& shapes)
  : observer(defaultMessage_),
    demComponent("particles", control),
    dynPointStruct_(control, shapes.maxBoundingSphere()),
    shapeIndex_(
      objectFile(
        "shapeIndex",
        "",
        objectFile::READ_ALWAYS,
        objectFile::WRITE_ALWAYS
      ),
      dynPointStruct_,
      0
    ),
    contactForce_(
      objectFile(
        "contactForce",
        "",
        objectFile::READ_IF_PRESENT,
        objectFile::WRITE_ALWAYS
      ),
      dynPointStruct_,
      zero3
    ),
    contactTorque_(
      objectFile(
        "contactTorque",
        "",
        objectFile::READ_IF_PRESENT,
        objectFile::WRITE_ALWAYS
      ),
      dynPointStruct_,
      zero3
    ),
    idHandler_(particleIdHandler::create(dynPointStruct_)),
    baseFieldBoundaryUpdateTimer_("baseFieldBoundaryUpdate",&timers())
{
	this->addToSubscriber(dynPointStruct_);

	//idHandler_().initialIdCheck();
}

pFlow::particles::~particles()
{
  // invalidates / unsobscribe from subscriber before its actual destruction
  addToSubscriber(nullptr, message::Empty());
}

bool
pFlow::particles::beforeIteration()
{
	if( !dynPointStruct_.beforeIteration())
  {
    return false;
  }

  zeroForce();
	zeroTorque();
  baseFieldBoundaryUpdateTimer_.start();
  shapeIndex_.updateBoundariesSlaveToMasterIfRequested();
  idHandler_().updateBoundariesSlaveToMasterIfRequested();
  baseFieldBoundaryUpdateTimer_.end();
  return true;
}

bool
pFlow::particles::iterate()
{
	return dynPointStruct_.iterate();
}

bool
pFlow::particles::afterIteration()
{
	return dynPointStruct_.afterIteration();
}

void
pFlow::particles::boundingSphereMinMax(real& minDiam, real& maxDiam) const
{
	auto& shp = getShapes();

	minDiam = shp.minBoundingSphere();
	maxDiam = shp.maxBoundingSphere();
}
