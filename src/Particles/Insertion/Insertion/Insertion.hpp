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


#include "insertion.hpp"
#include "ListPtr.hpp"
#include "InsertionRegion.hpp"
#include "particles.hpp"

namespace pFlow
{

template<typename ShapeType>
class Insertion
:
	public insertion
{
protected:

	const ShapeType& shapes_;

	// - insertion regions 
	ListPtr<InsertionRegion<ShapeType>>  regions_;


	bool readInsertionDict(const dictionary& dict);

	bool writeInsertionDict(dictionary& dict)const;

public:

	TypeInfoTemplateNV("Insertion",ShapeType);

	Insertion(particles& prtcl, const ShapeType& shapes);

	Insertion(fileSystem file, particles& prtcl, const ShapeType& shapes);


	bool insertParticles(real currentTime, real dt);

	virtual bool read(iIstream& is) override;

	virtual bool write(iOstream& os)const override;

};

}

#include "Insertion.cpp"

#endif
