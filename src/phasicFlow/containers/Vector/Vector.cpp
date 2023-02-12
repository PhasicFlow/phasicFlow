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


template<typename T, typename Allocator>
pFlow::Vector<T, Allocator>::Vector(iIstream& is)
{
	readVector(is);
}

template<typename T, typename Allocator>
bool pFlow::Vector<T, Allocator>::readVector
(
	iIstream& is
)
{
	this->clear();

	is.fatalCheck(FUNCTION_NAME);

	token firstToken(is);

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
		
		while 
			( !(
					lastToken.isPunctuation()
             	&& lastToken == token::END_LIST
             	 
             	) 
            )
		{

			is.putBack(lastToken);
			
			is >> val;
			this->push_back(val);
			
			is >> lastToken;
			is.fatalCheck(FUNCTION_NAME);
		}

	} else
	{
		warningInFunction
		<< "expected token "<< token::BEGIN_LIST 
		<< " but found "<< firstToken ;
		return false;

	}

	return true;
}


template<typename T, typename Allocator>
bool pFlow::Vector<T, Allocator>::writeVector
(
	iOstream& os
) const
{

	
	auto len = size();
	auto stride  = getVectorStride(len);

	// start of 
	os << token::BEGIN_LIST;
	label i = 0;
	while( i<len )
	{

		os << this->operator[](i++);
		for(label j=0; j<stride-1 && i<len; j++ )
		{
			os << token::SPACE << this->operator[](i++);	
		}

	 	if(i<len)
	 		os<< token::NL;
	}
    
    os << token::END_LIST;

    os.check(FUNCTION_NAME);

    return true;
}

template<typename T, typename Allocator>
bool pFlow::Vector<T, Allocator>::deleteElement_sorted
(
	const Vector<label>& indices
)
{
	if( indices.size() == 0 )return true;

	if( indices.size() == 1 )
	{
		return deleteElement(indices[0]);
	}

	if( *(indices.end()-1) >= size() ) return false;
	

	VectorType tmp(capacity(), RESERVE());
	tmp.clear();

	label lindex = 0;
	for(auto& delem: indices)
	{
		for(label i=lindex; i<delem; i++)
		{
			tmp.push_back( vectorType::operator[](i) );
		}
		lindex = delem+1;
	}

	// copy after the last delete element 
	for(label i=lindex; i<size(); i++)
	{
		tmp.push_back( vectorType::operator[](i) );
	}
	

	vectorType::swap(tmp);

	return true;
}


template<typename T, typename Allocator>
bool pFlow::Vector<T, Allocator>::deleteElement
(
	const Vector<label>& indices
)
{
	if( indices.size() == 0 )return true;

	if( indices.size() == 1)
	{
		return deleteElement(indices[0]);
	}

	// sorts 
	auto sorted = indices;
	sort(sorted);
	return deleteElement_sorted(sorted);
}

template<typename T, typename Allocator>
bool pFlow::Vector<T, Allocator>::deleteElement
(
	label index
)
{
	if(index < size())
	{
		auto iter = vectorType::begin();
		advance(iter, index);
		vectorType::erase(iter);
		return true;
	}
	else
		return false;
}

template<typename T, typename Allocator>
bool pFlow::Vector<T, Allocator>::insertSetElement(
	const  int32IndexContainer& indices,
	const T& val)
{
	
	if(indices.size() == 0)return true;
	
	auto hIndices = indices.hostView();

	ForAll(i, indices)
	{
		auto s = size();
		auto idx = hIndices[i];

		if( idx < s )
		{
			this->operator[](idx) = val;
		}
		else if(idx == s )
		{	
			this->push_back(val);
		} 
		else
		{
			this->resize(idx+1);
			this->operator[](idx) = val;
		}
	}
	return true;
}

template<typename T, typename Allocator>
bool pFlow::Vector<T, Allocator>::insertSetElement(
	const  int32IndexContainer& indices,
	const Vector<T>& vals)
{
	if(indices.size() == 0)return true;
	if(indices.size() != vals.size())return false;

	auto hIndices = indices.hostView();
	
	ForAll(i, indices)
	{
		auto s = size();
		auto idx = hIndices[i];
		if( idx < s )
		{
			this->operator[](idx) = vals[i];
		}
		else if(idx == s )
		{	
			this->push_back(vals[i]);
		} 
		else
		{
			this->resize(idx+1);
			this->operator[](idx) = vals[i];
		}
	}
	return true;

}

template<typename T, typename Allocator>
bool pFlow::Vector<T, Allocator>::insertSetElement
(
	const Vector<int32>& indices,
	const T& val
)
{
	if(indices.size() == 0)return true;
	
	ForAll(i, indices)
	{
		auto s = size();
		auto idx = indices[i];
		if( idx < s )
		{
			this->operator[](idx) = val;
		}
		else if(idx == s )
		{	
			this->push_back(val);
		} 
		else
		{
			this->resize(idx+1);
			this->operator[](idx) = val;
		}
	}
	return true;
}


template<typename T, typename Allocator>
bool pFlow::Vector<T, Allocator>::insertSetElement
(
	const Vector<int32>& indices,
	const Vector<T>& vals
)
{
	if(indices.size() == 0)return true;
	if(indices.size() != vals.size())return false;

	ForAll(i, indices)
	{
		auto s = size();
		auto idx = indices[i];
		if( idx < s )
		{
			this->operator[](idx) = vals[i];
		}
		else if(idx == s )
		{	
			this->push_back(vals[i]);
		} 
		else
		{
			this->resize(idx+1);
			this->operator[](idx) = vals[i];
		}
	}
	return true;
}

template<typename T, typename Allocator>
inline bool pFlow::Vector<T, Allocator>::insertSetElement
(
	int32 idx,
	const T & val
)
{
	
	auto s = size();
	
	if( idx < s )
	{
		this->operator[](idx) = val;
	}
	else if(idx == s)
	{	
		this->push_back(val);
	} 
	else
	{
		this->resize(idx+1);
		this->operator[](idx) = val;
	}
	
	return true;
}