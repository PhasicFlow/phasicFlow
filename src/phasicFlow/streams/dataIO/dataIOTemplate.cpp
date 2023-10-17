
template<typename T>
bool pFlow::dataIO::writeDataEnd(
	const fileSystem& filePath,
	const span<T>& data)
{

	span<unsigned char> charSpan(
		reinterpret_cast<unsigned char*> (const_cast<T*>(data.data())),
		data.size()*sizeof(T));

	switch (IOType_)
	{
		case MasterProcessor:
		case AllProcessorsSimilar:
		{
			// This	means that only master processor should write
			// in this case we perform write on master processor only
			// this means that the data 
			if(processors::isMaster())
			{
				return writeDataToFileEndSTD(filePath, charSpan);
			}
			else
			{
				return true;
			}
			break;
		}
		case AllProcessorsDifferent:
		{
			// This means that all processors should write their own
			// copy of data 
			return writeDataToFileEndMPI(filePath, charSpan);
			break;
		}
	}

	return false;
}

template<typename T>
bool pFlow::dataIO::readData
(
	const fileSystem& filePath, 
	std::vector<T>& data
)
{
	std::vector<uint64> chunkSizes;
	uint64 startPosBinaryBlock;

	// read meta data
	switch (IOType_)
	{
		case MasterProcessor:
		{
			if(!readMetaSTD(
				filePath, 
				chunkSizes, 
				startPosBinaryBlock))
			{
				return false;
			}
			break;
		}
		case AllProcessorsDifferent:
		case AllProcessorsSimilar:
		{
			if(!readMetaMPI(
				filePath,
				chunkSizes,
				startPosBinaryBlock))
			{
				return false;
			}
			break;
		}
	}

	data.clear();
	if(IOType_ == MasterProcessor)
	{
		auto sizeOfData = std::accumulate( 
			chunkSizes.begin(), 
			chunkSizes.end(),
			static_cast<uint64>(0));

		data.resize(sizeOfData/sizeof(T));

		span<unsigned char> charSpan(
			reinterpret_cast<unsigned char*>(data.data()),
			data.size()*sizeof(T));

		readDataSTD(filePath, chunkSizes, charSpan, startPosBinaryBlock);
	}
	if( IOType_ == AllProcessorsDifferent )
	{
		auto thisProc = processors::globalRank();

		data.resize(chunkSizes[thisProc]/sizeof(T));

		span<unsigned char> charSpan(
			reinterpret_cast<unsigned char*>(data.data()),
			data.size()*sizeof(T));

		std::cout<<"MPI part"<<std::endl;
		readDataMPI(filePath, chunkSizes, charSpan, startPosBinaryBlock);

	}
	
	return true;
}