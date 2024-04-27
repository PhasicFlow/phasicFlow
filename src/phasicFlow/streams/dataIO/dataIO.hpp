
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

#ifndef __dataIO_hpp__
#define __dataIO_hpp__

#include <vector>

#include "span.hpp"
#include "IOPattern.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"
#include "virtualConstructor.hpp"
#include "pFlowProcessors.hpp"

namespace pFlow
{

// - Forward 
template<typename T>
bool writeDataAsciiBinary
(
	iOstream& os, 
	span<T> data
);

template<typename T>
bool writeDataASCII
(
	iOstream& os, 
	span<T> data
);

template<typename T>
bool readDataAscii
(
	iIstream& is,
	std::vector<T>& vec
);

template<typename T>
bool readDataAsciiBinary
(
	iIstream& is, 
	std::vector<T>& data
);

template<typename T>
class dataIO
{
protected:

	IOPattern 	    ioPattern_;

	std::vector<T> 	buffer_;

    span<T>         bufferSpan_; 


    /// gather data from all processors and put the results in buffer_
	virtual bool gatherData(span<T> data) = 0;  

public:

	/// Type info
	TypeInfo("dataIO");

	dataIO(const IOPattern& iop)
	:
	    ioPattern_(iop)
	{}

	dataIO(const dataIO&) = default;

	dataIO(dataIO &&) = default;

	dataIO& operator = (const dataIO&) = default;

	dataIO& operator = (dataIO&&) = default;

	virtual ~dataIO() = default;

	create_vCtor
	(
		dataIO,
		IOPattern,
		(const IOPattern& iop),
		(iop)
	);

	/// Write data to the end of file from all processors.
	/// This method should be called from all processors. 
	bool writeData(iOstream& os, span<T> data);

	bool readData(
        iIstream& is, 
        std::vector<T>& data);

	static
	uniquePtr<dataIO> create(const IOPattern& iop);

};

template<typename T>
inline 
iOstream& operator<<(iOstream& os, const span<T>& s)
{
	if(!writeDataAsciiBinary(os, s))
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}
    return os;
}


}

#include "dataIO.cpp"

#endif
