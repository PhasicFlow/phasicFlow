
#include "dataIONoMPI.hpp"


pFlow::dataIONoMPI::dataIONoMPI
(
	IOPattern::IOType iotype,
	word exeMode
)
:
	dataIO(iotype, exeMode)
{}


bool pFlow::dataIONoMPI::gatherData( span<char> sp )
{
	/// in serial mode, no gathering is required. 
	buffer_ = sp;
	return true;
}