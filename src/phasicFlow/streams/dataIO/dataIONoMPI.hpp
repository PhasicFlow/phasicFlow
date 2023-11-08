#ifndef __datIONoMPI_hpp__
#define __datIONoMPI_hpp__

#include "dataIO.hpp"

namespace pFlow
{


class dataIONoMPI
:
	public dataIO
{
protected:

public:

	TypeInfo("dataIO<NoMPI>");

	dataIONoMPI(IOPattern::IOType iotype, word exeMode);

	dataIONoMPI(const dataIONoMPI&) = default;

	dataIONoMPI(dataIONoMPI&&) = default;


	dataIONoMPI& operator=(const dataIONoMPI&) = default;

	dataIONoMPI& operator=(dataIONoMPI&&) = default;

	~dataIONoMPI() = default;

	add_vCtor
	(
		dataIO,
		dataIONoMPI,
		word
	);

	bool gatherData(span<char> sp ) override;
	
};


}


#endif