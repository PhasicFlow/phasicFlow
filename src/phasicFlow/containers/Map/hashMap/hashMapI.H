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


template<class Key, class T, class Compare >
bool pFlow::hashMap<Key, T, Compare>::insertIf(const keyType& k, const mappedType & v)
{
	auto [iter, success] = this->insert( valueType(k,v));
	return success;
}

template<class Key, class T, class Compare >
bool pFlow::hashMap<Key, T, Compare>::insertIf( keyType&& k, mappedType && v)
{
	auto [iter, success] = this->insert
									( 
										std::move( valueType(k,v) )
									);
	return success;
}

template<class Key, class T, class Compare >
bool pFlow::hashMap<Key, T, Compare>::search(const keyType k) const
{
	auto [iter, found] = findIf(k);
	return found;
}

template<class Key, class T, class Compare >
std::pair<typename pFlow::hashMap<Key, T, Compare>::iterator, bool>
	pFlow::hashMap<Key, T, Compare>::findIf(const keyType& k)
{	
	if( auto iter = this->find(k); iter!= this->end() )
		return {iter,true};
	else
		return {iter,false};
}

template<class Key, class T, class Compare >
const std::pair<typename pFlow::hashMap<Key, T, Compare>::constIterator, bool> 
	pFlow::hashMap<Key, T, Compare>::findIf(const keyType& k) const
{
	if( auto iter = this->find(k); iter!= this->end() )
		return {iter,true};
	else
		return {iter,false};
}


template<typename T>
inline iOstream& printKeys(iOstream& os, const wordHashMap<T> & m)
{
	if (m.empty())
		return os<<"wordHashMap is empty"<<endl;

	for(auto iter = m.cbegin(); iter != m.cend(); iter++ )
	{
		os << iter->first<<endl;
	}

	return os;
}


template<typename T>
inline iOstream& printKeys(iOstream& os, const labelHashMap<T> & m)
{
	if (m.empty())
		return os<<"labelHashMap is empty"<<endl;

	for(auto iter : m )
	{
		os << iter->first<<endl;
	}

	return os;
}

template<typename T>
inline iOstream& printKeys(iOstream& os, const uint32HashMap<T> & m)
{
	if (m.empty())
		return os<<"uint32HashMap is empty"<<endl;

	for(auto iter : m )
	{
		os << iter->first<<endl;
	}

	return os;
}

template<typename T>
inline iOstream& printKeys(iOstream& os, const int64HashMap<T> & m)
{
	if (m.empty())
		return os<<"int64HashMap is empty"<<endl;

	for(auto iter : m )
	{
		os << iter->first<<endl;
	}

	return os;
}


template<typename T>
inline iOstream& printKeys(iOstream& os, const int32HashMap<T> & m)
{
	if (m.empty())
		return os<<"int32HashMap is empty"<<endl;

	for(auto iter : m )
	{
		os << iter->first<<endl;
	}

	return os;
}
