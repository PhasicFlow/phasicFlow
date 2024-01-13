
template<typename T>
bool pFlow::dataIO<T>::writeData(iOstream& os, span<T> data)
{
	/// first gather data from all processors (if any)
	if(!gatherData( data ) )
	{
		fatalErrorInFunction<<
		"Error in gathering data for out stream "<< os.name()<<endl;
		return false;
	}

	if( ioPattern_.thisProcWriteData())
	{
		if( os.isBinary() )
		{
			// first write the number of data
			uint64 numData = bufferSpan_.size();
			os<< numData << endl;

            // write the bindary data
            auto chBuffer = charSpan(bufferSpan_);

			if(!os.write(chBuffer.data(), chBuffer.size()))
			{
				fatalErrorInFunction<<
		    	"error in writing binary data to "<<os.name()<<endl;
		    	return false;
			}

		}
		else
		{
		    if( !bufferSpan_.writeASCII(os) )
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

template<>
inline
bool pFlow::dataIO<pFlow::word>::writeData(iOstream& os, span<word> data)
{
    notImplementedFunction;
    fatalExit;
    /// first gather data from all processors (if any)
	if(!gatherData( data ) )
	{
		fatalErrorInFunction<<
		"Error in gathering data for out stream "<< os.name()<<endl;
		return false;
	}

	if( !bufferSpan_.writeASCII(os) )
    {
    	fatalErrorInFunction<<
    	"error in writing ASCII data to "<<os.name()<<endl;
    	return false;
    }

	return true;
}


template<typename T>
bool pFlow::dataIO<T>::readData
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
bool pFlow::dataIO<T>::readAscii
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
		warningInFunction<<"vector length specified "<< len << 
		" is different from number of elements "<< vec.size()<<endl;
		return false;
	}

	return true;
}