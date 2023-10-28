
#ifndef __dataIO_hpp__
#define __dataIO_hpp__

#include <vector>

#include "types.hpp"
#include "span.hpp"
#include "IOPattern.hpp"

namespace pFlow
{

class fileSystem;


class dataIO
{

public:


	static inline const uint64 ErrorReturn = static_cast<uint64>(-1);
	
protected:

	IOPattern ioPattern_;

	uint64 lastPosWrite_ = 0;

	uint64 lastPosRead_  = 0;

	
	bool writeDataToFileEndSTD(
		const word& wordPath, 
		const span<unsigned char>& data);

	bool writeDataToFileEndMPI(
		const word& wordPath, 
		const span<unsigned char>& data);
	
	bool readDataSTD(
		const word& wordPath,
		const std::vector<uint64> chunkSizes,
		span<unsigned char>& data,
		uint64 binaryBlockStart);
	
	bool readDataMPI(
		const word& wordPath, 
		const std::vector<uint64> chunkSizes,
		span<unsigned char>& data,
		uint64 binaryBlockStart);

	bool readMetaMPI(
		const word& wordPath, 
		std::vector<uint64>& chunkSizes,
		uint64 startPosSearch,
		uint64 &startPosBinaryBlock);

	bool readMetaSTD(
		const word& wordPath, 
		std::vector<uint64>& chunkSizes,
		uint64 startPosSearch,
		uint64 &startPosBinaryBlock);

	bool waitForAllMPI();

	bool maxReduction( uint64& src, uint64& dst);

	bool BcastPos(uint64 & pos);

public:

	dataIO(IOPattern::IOType iotype)
	:
		ioPattern_(iotype)
	{}

	~dataIO()=default;

	template<typename T> 
	bool writeDataEnd(
		const word& wordPath,
		const span<T>& data);

	template<typename T>
	bool writeAsciiEnd(
		iOstream& os,
		const span<T>& data);

	template<typename T>
	bool readDataBinary(
		const word& wordPath, 
		std::vector<T>& data,
		uint64 startPos = 0);

	template<typename T>
	bool readAscii(
		iIstream& is,
		std::vector<T>& vec	);

	template<typename T>
	bool readData(
		iIstream& is, 
		std::vector<T>& vec,
		bool resume = false); // resume only works for binary
							  // for ascii, by default it starts from the current position


	template<typename T>
	bool writeData(iOstream& os,
		const span<T>& data);

	
};

template<>
inline
bool pFlow::dataIO::writeData<pFlow::word>
(
	iOstream& os,
	const span<word>& data
)
{
	if( !writeAsciiEnd(os, data) )
    {
        fatalErrorInFunction;
        return false;
    }
	return true;	
}

template<>
inline
bool pFlow::dataIO::readData<pFlow::word>(
	iIstream& is, 
	std::vector<word>& vec,
	bool resume)
{
	if(!readAscii(is, vec))
	{
		fatalErrorInFunction;
		return false;
	}
	return true;
}


}

#include "dataIOTemplate.cpp"

#endif



