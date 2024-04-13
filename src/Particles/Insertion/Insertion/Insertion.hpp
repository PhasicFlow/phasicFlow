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

#ifndef __Insertion_hpp__
#define __Insertion_hpp__

#include "InsertionRegion.hpp"
#include "ListPtr.hpp"
#include "insertion.hpp"
#include "particles.hpp"

namespace pFlow
{

/**
 * This class manages all the insertion regions for particles insertion
 * in the simulation.
 *
 * Any number of insertion regions can be defined in a simulation. The
 * data for particle insertion is provided in particleInsertion file, which
 * looks like this. A list of insertion regions (class insertionRegion) can be
defined in this file.
 * For more information see file insertionRegion.hpp.
 * \verbatim
active  yes;

region1
{
    // the data for insertionRegion
}

region2
{
    // Data for insertionRegion
}
 \endverbatim
 */
template<typename ShapeType>
class Insertion : public insertion
{
private:

	const ShapeType&                    shapes_;

	// - insertion regions
	ListPtr<InsertionRegion<ShapeType>> regions_;


	bool setInsertionRegions();

	// bool readInsertionDict(const dictionary& dict);

	// bool writeInsertionDict(dictionary& dict)const;

public:

	TypeInfoTemplateNV11("Insertion", ShapeType);

	Insertion(particles& prtcl, const ShapeType& shapes);

	// Insertion(fileSystem file, particles& prtcl, const ShapeType& shapes);

	bool insertParticles(uint32 iter, real t, real dt);

	/*virtual bool read(iIstream& is) override;

	virtual bool write(iOstream& os)const override;*/
};

}

#include "Insertion.cpp"

#endif
