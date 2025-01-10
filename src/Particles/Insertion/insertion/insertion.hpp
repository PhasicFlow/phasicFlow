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
#ifndef __insertion_hpp__
#define __insertion_hpp__

#include "fileDictionary.hpp"

namespace pFlow
{

// forward
class particles;
class pointStructure;

/**
 * Base class for particle insertion
 */
class insertion 
: 
    public fileDictionary
{
private:

	/// Is insertion active
	Logical    active_ = "No";

	/// Check for collision? It is not active now
	Logical    checkForCollision_ = "No";

	/// if increase velocity in case particles are failed
	/// to be inserted due to collision
	Logical    increaseVelocity_ = "No";

	word 		diameterName_ = "diameter";

	word 		velocityName_ = "velocity";

	/// Ref to particles
	particles& particles_;

	bool       readFromFile_ = false;

	/// Read from dictionary
	bool readInsertionDict();

protected:

	/// Write to dictionary
	virtual bool writeInsertionDict(dictionary& dict)const;

public:

	/// Type info
	TypeInfo("insertion");

	/// Construct from component
	explicit insertion(particles& prtcl);

	/// Destructor
	~insertion() override = default;

	/// is Insertion active
	inline bool isActive() const
	{
		return readFromFile_ && active_();
	}

	inline bool checkForCollision() const
	{
		return checkForCollision_();
	}

	inline particles& Particles()
	{
		return particles_;
	}

	inline const particles& Particles() const
	{
		return particles_;
	}

	const pointStructure& pStruct()const;

	inline bool readFromFile() const
	{
		return readFromFile_;
	}

	const word& diameterName()const
	{
		return diameterName_;
	}

	const word& velocityName()const
	{
		return velocityName_;
	}

	/// read from stream
	//bool read(iIstream& is, const IOPattern& iop) override;

	/*/// read from iIstream
	virtual bool read(iIstream& is) = 0;*/

    using fileDictionary::write;

	/// write to iOstream
	bool write(iOstream& os, const IOPattern& iop)const override ;
};

}

#endif
