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

#ifndef __Time_hpp__
#define __Time_hpp__

#include "error.hpp"
#include "types.hpp"

#include "fileSystem.hpp"
#include "repository.hpp"
#include "timeControl.hpp"


namespace pFlow
{

class dictionary;

class Time
  : public repository
  , public timeControl
{
private:

	bool       outFormatBinary_ = false;

	// - geometry folder/repository
	repository geometry_;

	bool       readDictionary(const dictionary& dict);

public:

	// Constructor with owner and settings dict
	Time(repository* owner, const dictionary& setiingsDict);

	Time(
	  repository* owner,
	  dictionary& setiingsDict,
	  real        startTime,
	  real        endTime,
	  real        saveInterval,
	  word        startTimeName
	);

	//// -  Methods
	fileSystem        localPath() const override;

	// - geometry repository
	const repository& geometry() const
	{
		return geometry_;
	}

	repository& geometry()
	{
		return geometry_;
	}

	fileSystem integrationFolder() const;

	/// Write to the file with binary format?
	bool       outFileBinary() const override
	{
		return outFormatBinary_;
	}

	// override the base write to manage write operation
	// based on the valid write time intervals
	virtual bool write(bool verbose = false) const;
};

} // namespace pFlow

#endif // __Time_hpp__
