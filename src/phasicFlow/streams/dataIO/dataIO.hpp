
#ifndef __dataIO_hpp__
#define __dataIO_hpp__

#include <vector>

#include "types.hpp"
#include "span.hpp"
#include "IOPattern.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"



namespace pFlow
{

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

	/// Write data to the end of file from all processors.
	/// This method should be called from all processors. 
	bool writeData(iOstream& os, span<T> data);

	bool readData(
        iIstream& is, 
        std::vector<T>& data);

	bool readAscii(
		iIstream& is,
		std::vector<T>& vec	);

};
	


}

#include "dataIOTemplate.cpp"

#endif
