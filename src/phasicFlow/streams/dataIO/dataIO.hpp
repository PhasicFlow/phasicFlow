
#ifndef __dataIO_hpp__
#define __dataIO_hpp__

#include <vector>

#include "types.hpp"
#include "span.hpp"
#include "processors.hpp"

namespace pFlow
{

class fileSystem;


class dataIO
{

public:

	/**
	 * Type of input/output
	 * MasterProcessor: Read or write is done on master processor
	 *    and the data on master processor is affected.
	 * AllProcessorsDifferent: Read or write is done on all processors, 
	 *    and each processor munipulates its own data.
	 * AllProcessorsSimilar: Read is done on all processors but the read data
	 *    is similar on processors. Write is done on master processor, since 
	 *    all processors have the same copy of data. 
	 */
	enum IOType : int
	{
		MasterProcessor,
		AllProcessorsDifferent,
		AllProcessorsSimilar
	};
protected:

	IOType IOType_;

	
	bool writeDataToFileEndSTD(
		const fileSystem& filePath, 
		const span<unsigned char>& data);

	bool writeDataToFileEndMPI(
		const fileSystem& filePath, 
		const span<unsigned char>& data);
	
	bool readDataSTD(
		const fileSystem& filePath,
		const std::vector<uint64> chunkSizes,
		span<unsigned char>& data,
		uint64 binaryBlockStart);
	
	bool readDataMPI(
		const fileSystem& filePath, 
		const std::vector<uint64> chunkSizes,
		span<unsigned char>& data,
		uint64 binaryBlockStart);

	bool readMetaMPI(
		const fileSystem& filePath, 
		std::vector<uint64>& chunkSizes,
		uint64 &startPosBinaryBlock);

	bool readMetaSTD(
		const fileSystem& filePath, 
		std::vector<uint64>& chunkSizes,
		uint64 &startPosBinaryBlock);

public:

	dataIO(IOType ioT)
	:
		IOType_(ioT)
	{}

	~dataIO()=default;

	template<typename T> 
	bool writeDataEnd(
		const fileSystem& filePath,
		const span<T>& data);

	template<typename T>
	bool readData(
		const fileSystem& filePath, 
		std::vector<T>& data);

	

};

}

#include "dataIOTemplate.cpp"

#endif



