
#include "phasicFlowConfig.H"
#ifdef pFlow_Build_MPI
	#include <mpi.h>
#endif

#include <cstdio>
#include <numeric>


#include "dataIO.hpp"
#include "fileSystem.hpp"


#include "streams.hpp"

static const size_t numcharFlag = 8;

static inline unsigned char binaryFlag__[numcharFlag] = {255, 255, 255, 255, 255, 255, 255 ,0};

inline 
pFlow::uint64 chunkSizeOffeset(pFlow::uint64 procNo)
{
	// 1 is added for number of processors 
	return (1+procNo)*sizeof(pFlow::uint64);
}

bool writeBinaryBlockFlagSTD(std::FILE* fh)
{

	std::size_t nw = std::fwrite( binaryFlag__ , sizeof(unsigned char), numcharFlag, fh );

	if(nw < numcharFlag) 
		return false;
	else
		return true;
}

pFlow::uint64 findBindaryBlockFlagSTD(std::FILE* fh)
{
	// get the current postion 
	long fpos;
	if( fpos = std::ftell( fh) ; fpos == -1L )
	{
		fatalErrorInFunction;
		return -1;
	}

	pFlow::uint64 filePos = static_cast<pFlow::uint64>(fpos);

	// start reading char by char 
	int c; 
	unsigned char ch;
	int currPos = 0;
    while ( std::fread(&ch, sizeof(ch), 1, fh) == 1 ) 
    {
   		if(std::ferror(fh)) return -1;	
   		if(std::feof(fh))return -1;

   		filePos++;

   		if(ch == binaryFlag__[currPos] ) 
   		{
   			currPos++;

   			if(currPos == numcharFlag) return filePos;
   		}
   		else
   		{
   			currPos = 0;
   		}   
    }

    return -1;
}


#ifdef pFlow_Build_MPI
pFlow::uint64 findBindaryBlockFlagMPI(MPI_File fh)
{
	// get the current postion 
	MPI_Offset fpos;
	if( MPI_File_get_position( fh, &fpos) != MPI_SUCCESS )
	{
		fatalErrorInFunction;
		return false;
	}

	pFlow::uint64 filePos = static_cast<pFlow::uint64>(fpos);

	// start reading char by char 
	unsigned char ch;
	int currPos = 0;

    while( MPI_File_read(
    	fh, 
    	&ch, 
    	1, 
    	MPI_UNSIGNED_CHAR, 
    	MPI_STATUS_IGNORE) == MPI_SUCCESS ) 
    {
   		
   		filePos++;
   		
   		if(ch == binaryFlag__[currPos] ) 
   		{
   			currPos++;

   			if(currPos == numcharFlag) return filePos;
   		}
   		else
   		{
   			currPos = 0;
   		}   
    }

    return -1;
}

#endif


bool pFlow::dataIO::writeDataToFileEndSTD
(
	const fileSystem& filePath, 
	const span<unsigned char>& data
)
{

	if(!processors::isMaster()) return true;

	// openfile 
	word wFile = filePath.wordPath();
	auto fh = std::fopen(wFile.c_str(), "ab");

	if(!fh)
	{
		fatalErrorInFunction<<
		"Error in Opening file "<< filePath <<endl;
		std::fclose(fh);
		return false;
	}

	// write the start of binary block flag
	if(std::fseek(fh, 0 , SEEK_END)!=0)
	{
		fatalErrorInFunction<<
		"error at reaching end of file "<<filePath<<endl;
		std::fclose(fh);
		return false;
	}

	if(!writeBinaryBlockFlagSTD(fh) )
	{
		fatalErrorInFunction<<
		"Error in writing to file "<< filePath<<endl;
		std::fclose(fh);
		return false;
	}
		
	uint64 numChunks = 1;	

	// write number of data chunks
	auto wc = std::fwrite(&numChunks, sizeof(numChunks), 1, fh);

	if(wc < 1)
	{
		fatalErrorInFunction<<
		"Error in writing numChunks to file "<< filePath <<endl;
		std::fclose(fh);
		return false;
	}


	// write size of each data chunk
	uint64 sizeOfData = data.size();

	wc = std::fwrite(&sizeOfData, sizeof(sizeOfData), 1, fh);
	if(wc <1)
	{
		fatalErrorInFunction<<
		"Error in writing size of data chunk to file "<< filePath <<endl;
		std::fclose(fh);
		return false;	
	}

	if(sizeOfData>0 )
	{
		// write data chunk to file
		wc = std::fwrite(data.data(), sizeof(unsigned char), sizeOfData, fh);
		if(wc < sizeOfData )
		{
			fatalErrorInFunction<<
			"Error in writing size of data to file "<< filePath <<endl;
			std::fclose(fh);
			return false;	
		}	
	}

	// close the file 
	std::fclose(fh);

	return true;
}

bool pFlow::dataIO::writeDataToFileEndMPI
(
	const fileSystem& filePath, 
	const span<unsigned char>& data
)
{	

#ifdef pFlow_Build_MPI
	
	// collect information from other processes
	uint64 numProc 	= processors::globalSize();
	uint64 thisSize = data.size();
	uint64 offset;

	CheckMPI
	(
		MPI_Scan(&thisSize, &offset, 1, MPI_UINT64_T,  MPI_SUM, MPI_COMM_WORLD),
		true
	);

	word wFile = filePath.wordPath();

	MPI_File fh;
	
	if( MPI_File_open(
			MPI_COMM_WORLD,
			wFile.c_str(),
			MPI_MODE_WRONLY+MPI_MODE_APPEND,
			MPI_INFO_NULL,
			&fh) != MPI_SUCCESS)
	{
		fatalErrorInFunction<<
		"Cannot open file "<< filePath<<endl;
		return false;
	}	

	uint64 startPos;

	if( processors::isMaster())
	{
		// set position to the end of file 
		MPI_File_seek(fh, 0 , MPI_SEEK_END);		

		if( MPI_File_write(
			fh, 
			binaryFlag__,
			numcharFlag,
			MPI_UNSIGNED_CHAR,
			MPI_STATUS_IGNORE
			) != MPI_SUCCESS )
		{
			fatalErrorInFunction<<
			"Cannot write binary block flag into "<< filePath<<endl;
			return false;
		}
		
		MPI_Offset posOfBlock;
		if( MPI_File_get_position(fh, &posOfBlock) != MPI_SUCCESS )
		{
			fatalErrorInFunction<<
			"Cannot get the end pos of file "<< filePath<<endl;
			return false;
		}

		
		startPos = static_cast<uint64>(posOfBlock);
	}

		
	if( MPI_Bcast(
		&startPos, 
		1, 
		MPI_UINT64_T, 
		processors::masterNo(), 
		MPI_COMM_WORLD) != MPI_SUCCESS)
	{
		fatalErrorInFunction;
		return false;
	}
 

	if( processors::isMaster())
	{
		uint64 numProc = processors::globalSize();
		if(MPI_File_write(
			fh,
			&numProc,
			1,
			MPI_UINT64_T,
			MPI_STATUS_IGNORE) != MPI_SUCCESS)
		{
			fatalErrorInFunction<<
			"Cannot write number of chunks into "<<filePath<<endl;
			return false;
		}
	}

	uint64 sizeOffset = startPos  + chunkSizeOffeset(processors::globalRank());

	
	if(MPI_File_write_at_all(
		fh,
		sizeOffset, 
		&thisSize,  
		1,
		MPI_UINT64_T,
		MPI_STATUS_IGNORE)!= MPI_SUCCESS)
	{
		fatalErrorInFunction<<
		"Cannot write size of chunk into "<<filePath<<endl;
		return false;
	}

	offset -= thisSize;

	uint64 chunkOffset = startPos + chunkSizeOffeset(processors::globalSize()) + offset;

	
	if(MPI_File_write_at_all(
		fh,
		chunkOffset,
		data.data(),
		thisSize,
		MPI_UNSIGNED_CHAR,
		MPI_STATUS_IGNORE) != MPI_SUCCESS)
	{
		fatalErrorInFunction<<
		"Cannot write data into "<<filePath<<endl;
		return false;;
	}

	MPI_File_close(&fh);
	MPI_Barrier(MPI_COMM_WORLD);
	return true;

#else
	return writeDataToFileEndSTD(filePath, data);
#endif

	
}


bool pFlow::dataIO::readDataSTD
(
	const fileSystem& filePath,
	const std::vector<uint64> chunkSizes,
	span<unsigned char>& data,
	uint64 binaryBlockStart
)
{
	// sum of all chuncks
	uint64 toRecv = std::accumulate(
		chunkSizes.begin(), 
		chunkSizes.end(), 
		static_cast<uint64>(0));

	if( data.size() != toRecv )
	{
		fatalErrorInFunction;
		return false;
	}

	word wFile = filePath.wordPath();
	auto fh = std::fopen(wFile.c_str(), "rb");

	if(!fh)
	{
		fatalErrorInFunction<<
		"Error in Opening file "<< filePath<<endl;
		return false;
	}

	// start of data chunks 
	uint64 offset = binaryBlockStart + chunkSizeOffeset(chunkSizes.size());
	
	if(auto res = std::fseek(fh, offset, SEEK_SET); res!= 0 )
	{
		fatalErrorInFunction<<
		"Error in file seek "<< filePath<<endl;
		std::fclose(fh);
		return false;
	}

	if(auto res = std::fread(
			data.data(),
			sizeof(unsigned char),
			data.size(),
			fh);
		res!= data.size() )
	{
		fatalErrorInFunction<<
		"Error in reading file "<< filePath<<endl;
		std::fclose(fh);
		return false;
	}

	std::fclose(fh);
	return true;
}


bool pFlow::dataIO::readDataMPI
(
	const fileSystem& filePath, 
	const std::vector<uint64> chunkSizes,
	span<unsigned char>& data,
	uint64 binaryBlockStart
)
{

#ifdef pFlow_Build_MPI

	if(chunkSizes.size() != processors::globalSize() )
	{
		fatalErrorInFunction;
		return false;
	}
	
	word wFile = filePath.wordPath();
	
	MPI_File fh;

	if( MPI_File_open(
		MPI_COMM_WORLD,
		wFile.c_str(),
		MPI_MODE_RDONLY,
		MPI_INFO_NULL,
		&fh))
	{
		fatalErrorInFunction;
		return false;
	}
		
	
	auto procNo = processors::globalRank();

	uint64 toRecv = chunkSizes[procNo];

	// start of data chunks 
	uint64 offset = binaryBlockStart + chunkSizeOffeset(processors::globalSize());

	for(auto i=0; i<procNo; i++)
	{
		offset += chunkSizes[i];
	}

	if( data.size() != toRecv )
	{
		fatalErrorInFunction;
		return false;
	}

	
	MPI_Status status;
	
	if( MPI_File_read_at_all(
		fh,
		offset,
		data.data(),
		data.size(),
		MPI_UNSIGNED_CHAR,
		&status) != MPI_SUCCESS )
	{
		fatalErrorInFunction;
		return false;
	}
#else

#endif

	return true;
}


bool pFlow::dataIO::readMetaMPI
(
	const fileSystem& filePath, 
	std::vector<uint64>& chunkSizes,
	uint64 &startPosBinaryBlock
)
{
	word wFile = filePath.wordPath();

#ifdef pFlow_Build_MPI	
	MPI_File fh;

	if(MPI_File_open(
		MPI_COMM_WORLD, 
		wFile.c_str(),
		MPI_MODE_RDONLY,
		MPI_INFO_NULL,
		&fh) != MPI_SUCCESS)
	{
		fatalErrorInFunction;
		return false;
	}
	

	uint64 startPos = findBindaryBlockFlagMPI(fh);
	if( startPos == -1 )
	{
		fatalErrorInFunction;
		return false;	
	} 

	startPosBinaryBlock  = startPos; 

	uint64 numProcInFile;

	if( MPI_File_read_at_all(
		fh,
		startPos,
		&numProcInFile,
		1,
		MPI_UINT64_T,
		MPI_STATUS_IGNORE) != MPI_SUCCESS)
	{
		fatalErrorInFunction;
		return false;
	}

	
	chunkSizes.resize(numProcInFile);
	
	if(MPI_File_read_at_all(
		fh,
		startPos + sizeof(numProcInFile),
		chunkSizes.data(), 
		chunkSizes.size(), 
		MPI_UINT64_T,
		MPI_STATUS_IGNORE) != MPI_SUCCESS)
	{
		fatalErrorInFunction;
		return false;
	}

	MPI_File_close(&fh);

#endif

	return true;
}

bool pFlow::dataIO::readMetaSTD
(
	const fileSystem& filePath, 
	std::vector<uint64>& chunkSizes,
	uint64 &startPosBinaryBlock
)
{
	// only on master
	if( !processors::isMaster()) return true;

	word wFile = filePath.wordPath();
	std::FILE *fh = std::fopen(wFile.c_str(), "rb");

	if(!fh)
	{
		fatalErrorInFunction<<
		"Error in Opening file "<< filePath<<endl;
		return false;
	}

	uint64 startPos = findBindaryBlockFlagSTD(fh);
	if(startPos == -1 )
	{
		fatalErrorInFunction;
		return false;
	}

	startPosBinaryBlock = startPos;


	std::fseek(fh, startPos, SEEK_SET); 

	uint64 numProcInFile;
	
	auto res = std::fread(&numProcInFile, sizeof(numProcInFile), 1, fh);
	

	if(res != 1 )
	{
		fatalErrorInFunction<<
		"Error in reading file "<< filePath<<endl;
		std::fclose(fh);
		return false;
	}

	
	chunkSizes.resize(numProcInFile);

	res = std::fread(chunkSizes.data(), sizeof(numProcInFile), numProcInFile, fh);

	if(res!= numProcInFile)
	{
		fatalErrorInFunction<<
		"Error in reading chunkSizes from file "<< filePath<<endl;
		std::fclose(fh);
		return false;
	}

	std::fclose(fh);
	return true;
}


