
#ifndef __dataIO_hpp__
#define __dataIO_hpp__

#include <vector>

#include "types.hpp"
#include "span.hpp"
#include "IOPattern.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"
#include "virtualConstructor.hpp"


namespace pFlow
{

class fileSystem;


class dataIO
{

public:


	static inline const uint64 ErrorReturn = static_cast<uint64>(-1);
	
protected:

	IOPattern 	ioPattern_;

	word 		executionMode_;

	span<char> 			buffer_;

	template<typename T>
	span<char> createSpan( span<T> sp )
	{
		return span<char>( reinterpret_cast<char*>(sp.data()), sp.size()*sizeof(T) );
	}

public:

	/// Type info
	TypeInfo("dataIO");

	dataIO(IOPattern::IOType iotype, word exeMode)
	:
	ioPattern_(iotype),
	executionMode_(exeMode)
	{}


	dataIO(const dataIO&) = default;

	dataIO(dataIO &&) = default;

	dataIO& operator = (const dataIO&) = default;

	dataIO& operator = (dataIO&&) = default;

	virtual ~dataIO() = default;

	create_vCtor
	(
		dataIO,
		word,
		(
			IOPattern::IOType iotype, 
			word exeMode
		),
		(iotype, exeMode)
	);

	/// Gather data from all processors and put the results in 
	/// buffer_
	virtual bool gatherData(span<char> sp ) = 0;

	/// Write data to the end of file from all processors.
	/// This method should be called from all processors. 
	template<typename T>
	bool writeData(iOstream& os, span<T> data);

	template<typename T>
	bool readData(iIstream& is, std::vector<T>& data);

	template<typename T>
	bool readAscii(
		iIstream& is,
		std::vector<T>& vec	);

	static
	uniquePtr<dataIO> create(IOPattern::IOType iotype, word exeMode);


};
	

template<>
inline 
bool dataIO::writeData<word>(iOstream& os, span<word> data)
{
	/*std::ostringstream ist();

	for(auto i=0; i<data.size(); i++)
	{
		ist<< data[i]<<std::endl;
	}

	span<char> sp(ist.str().data(), ist.str().size());

	if(!gatherData( sp) )
	{
		fatalErrorInFunction<<
		"Error in gathering data for out stream "<< os.name()<<endl;
		return false;
	}

	uint64 len = buffer_.size();
	os<< len << endl;

	if(!os.write(buffer_.data(), buffer_.size()))
	{
		fatalErrorInFunction<<
    	"error in writing binary data to "<<os.name()<<endl;
    	return false;
	}*/

	if( !data.writeASCII(os) )
    {
    	fatalErrorInFunction<<
    	"error in writing ASCII data to "<<os.name()<<endl;
    	return false;
    }

	return true;
}

}

#include "dataIOTemplate.cpp"

#endif



/*bool writeDataToFileEndSTD(
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
}*/