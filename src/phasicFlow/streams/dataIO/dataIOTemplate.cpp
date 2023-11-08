
template<typename T>
bool pFlow::dataIO::writeData(iOstream& os, span<T> data)
{
	/// first gather data from all processors (if any)
	if(!gatherData( createSpan(data) ) )
	{
		fatalErrorInFunction<<
		"Error in gathering data for out stream "<< os.name()<<endl;
		return false;
	}

	if( ioPattern_.thisProcWriteData())
	{
		if( os.isBinary() )
		{
			// first write the size of data 		    
			uint64 len = buffer_.size()/sizeof(T);
			os<< len << endl;

			if(!os.write(buffer_.data(), buffer_.size()))
			{
				fatalErrorInFunction<<
		    	"error in writing binary data to "<<os.name()<<endl;
		    	return false;
			}

		}
		else
		{

			/// cast the data into T
			span<T> allData( 
				reinterpret_cast<T*>(buffer_.data()), 
				buffer_.size()/sizeof(T));
		    
		    if( !allData.writeASCII(os) )
		    {
		    	fatalErrorInFunction<<
		    	"error in writing ASCII data to "<<os.name()<<endl;
		    	return false;
		    }
		}

		return os.check(FUNCTION_NAME);
	}
	else
	{
		return true;
	}
}

template<typename T>
bool pFlow::dataIO::readData
(
	iIstream& is, 
	std::vector<T>& data
)
{
	
	data.clear();

	if(ioPattern_.thisProcReadData())
	{
		if(is.isBinary())
		{
			// read length of data 			
			token firstToken(is);
			
			size_t len = 0;
			if( firstToken.isInt64())
			{
				len = firstToken.int64Token();
				data.resize(len);	
			}
			else
			{
				fatalErrorInFunction<<
				"expected length of vector in the stream "<<is.name()<<
				"but found this "<< firstToken<<endl;
				return false;
			}

			is.read(reinterpret_cast<char*>(data.data()), len*sizeof(T));	
			
			return is.check(FUNCTION_NAME);
		}
		else
		{
			
			return readAscii(is, data);
		}
	}
	else
	{
		return true;
	}
}

template<typename T>
bool pFlow::dataIO::readAscii
(
	iIstream& is,
	std::vector<T>& vec
)
{

	if( !ioPattern_.thisProcReadData() ) return true;
	

	is.fatalCheck(FUNCTION_NAME);

	vec.clear();
	
	token firstToken(is);

	size_t len = 0;
	if( firstToken.isInt64())
	{
		len = firstToken.int64Token();
		vec.reserve(len);
		firstToken  = token(is);
	}

	T val{};
	if( firstToken.isPunctuation() ) // start of vector 
	{
		if(firstToken != token::BEGIN_LIST)
		{
			warningInFunction
			<< "expected token "<< token::BEGIN_LIST 
			<< " but found "<< firstToken ;
			return false;

		}

		token lastToken(is);
		
		is.fatalCheck(FUNCTION_NAME);
		
		while(!(lastToken.isPunctuation()
             	&& lastToken == token::END_LIST ))
		{

			is.putBack(lastToken);
			
			is >> val;
			vec.push_back(val);
			
			is >> lastToken;
			is.fatalCheck(FUNCTION_NAME);
		}

	} else
	{
		warningInFunction
		<< "expected token "<< token::BEGIN_LIST 
		<< " but found "<< firstToken<<endl; ;
		return false;
	}
	
	if(len>0&& len != vec.size())
	{
		warningInFunction<<"vector lendth specified "<< len << 
		" is different from number of elements "<< vec.size()<<endl;
		return false;
	}

	return true;
}

/*template<typename T>
bool pFlow::dataIO::writeDataEnd(
	const word& wordPath,
	const span<T>& data)
{

	if( ioPattern_.thisProcWriteData() )
	{
		span<unsigned char> charSpan(
		reinterpret_cast<unsigned char*> (const_cast<T*>(data.data())),
		data.size()*sizeof(T));

		if( ioPattern_.isParallel() )
		{
			return writeDataToFileEndMPI(wordPath, charSpan);
		}
		else
		{
			return writeDataToFileEndSTD(wordPath, charSpan);
		}
	}
	else
	{
		return true;
	}

	return false;
}

template<typename T>
bool pFlow::dataIO::writeAsciiEnd
(
	iOstream& os,
	const span<T>& data
)
{
	if(ioPattern_.thisProcWriteData())
	{
		return data.writeASCII(os);
	}
	else
		return true;
}



template<typename T>
bool pFlow::dataIO::readDataBinary
(
	const word& wordPath, 
	std::vector<T>& data,
	uint64 startPos
)
{

	std::vector<uint64> chunkSizes;
	uint64 startPosBinaryBlock;

	data.clear();
	
	if( ioPattern_.thisProcReadData())
	{
		
		// read meta 
		if(!readMetaSTD(
				wordPath, 
				chunkSizes, 
				startPos,
				startPosBinaryBlock))
		{
			fatalErrorInFunction;
			return false;
		}
		
		
		if( ioPattern_.isMasterProcessor()  ||
			ioPattern_.isAllProcessorSimilar()
		)
		{

			auto sizeOfData = std::accumulate( 
			chunkSizes.begin(), 
			chunkSizes.end(),
			static_cast<uint64>(0));
			
			
			data.resize(sizeOfData/sizeof(T));

			span<unsigned char> charSpan(
				reinterpret_cast<unsigned char*>(data.data()),
				data.size()*sizeof(T));

			if(!readDataSTD(wordPath, chunkSizes, charSpan, startPosBinaryBlock))
			{
				fatalErrorInFunction;
				return false;
			}

			return true;

		}
		else if( ioPattern_.isAllProcessorDifferent() )
		{

			if(chunkSizes.size() != ioPattern_.globalSize())
			{
				if( ioPattern_.isMaster())
				{
					auto sizeOfData = std::accumulate( 
					chunkSizes.begin(), 
					chunkSizes.end(),
					static_cast<uint64>(0));
					
					
					data.resize(sizeOfData/sizeof(T));

					span<unsigned char> charSpan(
						reinterpret_cast<unsigned char*>(data.data()),
						data.size()*sizeof(T));

					if(!readDataSTD(wordPath, chunkSizes, charSpan, startPosBinaryBlock))
					{
						fatalErrorInFunction;
						return false;
					}
					return true;

				}
				else
				{
					return true;
				}
			}
			else
			{
				auto thisProc = ioPattern_.globalRank();

				data.resize(chunkSizes[thisProc]/sizeof(T));

				span<unsigned char> charSpan(
					reinterpret_cast<unsigned char*>(data.data()),
					data.size()*sizeof(T));

				
				if( !readDataMPI(wordPath, chunkSizes, charSpan, startPosBinaryBlock) )
				{
					fatalErrorInFunction;
					return false;
				}
				return true;	
			}

			
		}
		else
		{
			fatalErrorInFunction;
			return false;	
		}
	}
	else
	{
		return true;
	}

	return false;
		
	
}

template<typename T>
bool pFlow::dataIO::readAscii
(
	iIstream& is,
	std::vector<T>& vec
)
{

	if( !ioPattern_.thisProcReadData() ) return true;
	

	is.fatalCheck(FUNCTION_NAME);

	vec.clear();
	
	token firstToken(is);

	size_t len = 0;
	if( firstToken.isInt64())
	{
		len = firstToken.int64Token();
		vec.reserve(len);
		firstToken  = token(is);
	}

	T val{};
	if( firstToken.isPunctuation() ) // start of vector 
	{
		if(firstToken != token::BEGIN_LIST)
		{
			warningInFunction
			<< "expected token "<< token::BEGIN_LIST 
			<< " but found "<< firstToken ;
			return false;

		}

		token lastToken(is);
		
		is.fatalCheck(FUNCTION_NAME);
		
		while(!(lastToken.isPunctuation()
             	&& lastToken == token::END_LIST ))
		{

			is.putBack(lastToken);
			
			is >> val;
			vec.push_back(val);
			
			is >> lastToken;
			is.fatalCheck(FUNCTION_NAME);
		}

	} else
	{
		warningInFunction
		<< "expected token "<< token::BEGIN_LIST 
		<< " but found "<< firstToken<<endl; ;
		return false;
	}
	
	if(len>0&& len != vec.size())
	{
		warningInFunction<<"vector lendth specified "<< len << 
		" is different from number of elements "<< vec.size()<<endl;
		return false;
	}

	return true;
}

template<typename T>
bool pFlow::dataIO::readData
(
	iIstream& is, 
	std::vector<T>& vec,
	bool resume
)
{
	if(is.isBinary())
	{
		uint64 currPos = 0;
		if( resume )
		{
			currPos = is.tell();
		}

		if(! BcastPos(currPos) ) return false;
		
		if(!this->readDataBinary(is.name(), vec, currPos))
		{
			fatalErrorInFunction;
			return false;
		}

		auto lastPos = lastPosRead_;
		maxReduction( lastPos, lastPosRead_);
		//std::cout<<"last post read "<< lastPosRead_<<std::endl;
		
		/// set the stream indicator to the last position 
		is.seek(lastPosRead_);
		
		return true;
	}
	else
	{
		if(!readAscii(is, vec))
		{
			fatalErrorInFunction;
			return false;
		}
		return true;
	}
}

template<typename T>
bool pFlow::dataIO::writeData
(
	iOstream& os,
	const span<T>& sp
)
{
	if( os.isBinary() )
	{
	    os.startOfBinaryStreaming();

	    if(!writeDataEnd(os.name(), sp))
	    {
	        fatalErrorInFunction;
	        return false;
	    }

	    os.endOfBinaryStreaming();

	}
	else
	{
	    if( !writeAsciiEnd(os, sp) )
	    {
	        fatalErrorInFunction;
	        return false;
	    }
	}
	return true; 

}*/



