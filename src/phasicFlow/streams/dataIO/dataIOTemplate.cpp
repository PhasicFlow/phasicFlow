/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

template<typename T>
bool pFlow::writeDataAsciiBinary(iOstream& os, span<T> data)
{

	if( os.isBinary() )
	{
		// first write the number of data
		uint64 numData = data.size();
		os<< numData << endl;

		// write the bindary data
		auto chBuffer = charSpan(data);

		if(!os.write(chBuffer.data(), chBuffer.size()))
		{
			fatalErrorInFunction<<
			"error in writing binary data to "<<os.name()<<endl;
			return false;
		}

	}
	else
	{
		if( !writeDataASCII(os, data) )
		{
			fatalErrorInFunction<<
			"error in writing ASCII data to "<<os.name()<<endl;
			return false;
		}
	}

	return os.check(FUNCTION_NAME);
	
}

template<typename T>
bool pFlow::writeDataASCII(iOstream& os, span<T> data)
{
	os<< data.size()<<endl;
	os << token::BEGIN_LIST;
	if(data.size()>0)
	{
		for(uint32 i=0; i<data.size()-1; i++)
		{
			os << data[i]<<token::NL;
		}
		os << data[data.size()-1] << token::END_LIST;
	}
	else
	{
		os<< token::END_LIST;
	}
	
	os.check(FUNCTION_NAME);
	return true;
}

template<typename T>
bool pFlow::readDataAsciiBinary
(
	iIstream& is, 
	std::vector<T>& data
)
{
	
	if(is.isBinary())
	{
		data.clear();
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
		
		return readDataAscii(is, data);
	}
	
}

template<typename T>
bool pFlow::readDataAscii
(
	iIstream& is,
	std::vector<T>& vec
)
{
	
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
		return writeDataAsciiBinary(os, data);
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
    
	if( ioPattern_.isParallel()	)
	{
		notImplementedFunction<<
		"data transfer for type word is not supported in parallel mode!"<<endl;
		fatalExit;
	}

    /// first gather data from all processors (if any)
	if(!gatherData( data ) )
	{
		fatalErrorInFunction<<
		"Error in gathering data for out stream "<< os.name()<<endl;
		return false;
	}
	
	if( ioPattern_.thisProcWriteData())
	{
		return writeDataASCII(os, data);
	}
	else
	{
		return true;
	}
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
		return readDataAsciiBinary(is, data);
	}
	else
	{
		return true;
	}
}

