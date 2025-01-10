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
auto pFlow::List<T>::pos
(
	size_t i,
	bool noError
)
{
	if( i >= size() )
	{
		if(noError)
		{
			return std::end(*this);
		}
		else
		{
			fatalErrorInFunction<<
			"out of range access to list element. \n";
			fatalExit;
		}
	}
	auto iter = listType::begin();
	std::advance(iter, i);
	return iter;
}

template<typename T>
const auto pFlow::List<T>::pos
(
	size_t i,
	bool noError
)const 
{
	if( i >= size() )
	{
		if(noError)
		{
			return std::end(*this);
		}
		else
		{
			fatalErrorInFunction<<
			"out of range access to list element. \n";
			fatalExit;
		}
	}
	auto iter = listType::cbegin();
	std::advance(iter, i);
	return iter;
}


template<typename T>
inline pFlow::int32 pFlow::List<T>::countElement
(
	const T& elm
) const
{
	return std::count( listType::begin(), listType::end(), elm);
}

template<typename T>
inline size_t pFlow::List<T>::size()const
{
	return listType::size();
}

template<typename T>
inline T& pFlow::List<T>::operator[]
(
	size_t i
)
{
	return *pos(i);
}

template<typename T>
inline const T& pFlow::List<T>::operator[]
(
	size_t i
)const
{
	
	return *pos(i);
}

template<typename T>
inline typename  pFlow::List<T>::const_iterator pFlow::List<T>::find
(
	const T& val
) const
{
	return std::find(this->begin(),this->end(), val);
}

template<typename T>
inline typename  pFlow::List<T>::iterator pFlow::List<T>::find
(
	const T& val
)
{
	return std::find(this->begin(),this->end(), val);
}

template<typename T>
pFlow::int32 pFlow::List<T>::findi(const T& val) const
{
	auto pos = find(val);
	if( pos == this->end() )return -1;
	return static_cast<int32> (std::distance(this->begin(), pos));
}

template<typename T>
bool pFlow::List<T>::search
(
	const T& val
) const
{
	if( find(val) == this->end())return false;
	return true;
}


template<typename T>
inline void pFlow::List<T>::set(size_t i, const T& val)
{
	auto p = pos(i);
	*p = val;
}

template<typename T>
inline void pFlow::List<T>::set(size_t i, T&& val)
{
	auto p = pos(i);
	*p = std::move(val);
}


template<typename T>
inline bool pFlow::List<T>::writeList
(
	iOstream& os
) const
{
	
	size_t len = size();
	size_t stride = getListStride(len);

	// start of List
	
	os << beginListToken();
	
	for(auto elm = listType::begin(); elm!=listType::end(); )
	{
		os<< *elm++;
		
		for(size_t j=0; j<stride && elm!=listType::end(); j++)
		{
			os<<spaceToken()<< *elm++;
		}

		if( elm!=listType::end() )
			os<<newLineToken();
	}

	os<< endListToken();

    //os.check(FUNCTION_NAME);

    return os.check(FUNCTION_NAME);
}

template<typename T>
inline bool pFlow::List<T>::readList
(
	iIstream& is
)
{
	// first clear the list
	listType::clear();

	is.fatalCheck(FUNCTION_NAME);

	token firstToken(is);

	if( firstToken.isPunctuation() ) // start of list 
	{
		if(firstToken != beginListToken() )
		{
			ioErrorInFile(is.name(), is.lineNumber())
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

			T val;
			is >> val;
			
			listType::push_back(val);
			
			is >> lastToken;
			is.fatalCheck(FUNCTION_NAME);

		}

	}
	else
	{
		ioErrorInFile(is.name(), is.lineNumber())
				<< "expected token "<< beginListToken()
				<< " but found "<< firstToken ;
				return false;

	}
	
	return is.fatalCheck(FUNCTION_NAME);

}

template<typename T>
inline pFlow::iOstream& pFlow::operator << (iOstream& os, const List<T>& lst )
{
	if(!lst.writeList(os))
	{
		fatalExit;
	}
	return os;
}

template<typename T>
inline pFlow::iIstream& pFlow::operator >>(iIstream& is, List<T>& lst)
{
	if( !lst.readList(is) )
	{
		fatalExit;
	}
	return is;
}