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


#include "wordVectorHost.hpp"

bool pFlow::VectorSingle<pFlow::word,pFlow::HostSpace>::insertSetElement(
    const uint32IndexContainer& indices, 
    const word& val
)
{
    if(indices.empty()) return true;
	
	auto maxInd = indices.max();

	if(this->empty() || maxInd > size()-1 )
	{
		resize(maxInd+1);
	}

    auto ind = indices.hostView();
    auto s = indices.size();

    for(uint32 i=0; i< s; i++)
    {
        container_[ind[i]] = val;    
    }

    return true;
}

bool pFlow::VectorSingle<pFlow::word,pFlow::HostSpace>::insertSetElement(
    const uint32IndexContainer& indices, 
    const std::vector<word>& vals
)
{
    if(indices.empty())return true;
	if(indices.size() != vals.size())return false;

	auto maxInd = indices.max(); 
	
	if(this->empty() || maxInd > size()-1 )
	{
		resize(maxInd+1);
	} 

	auto ind = indices.hostView();
	auto s = indices.size();

	for(uint32 i=0; i< s; i++)
	{
		container_[ind[i]] = vals[i];    
	}

	return true;
}

bool pFlow::VectorSingle<pFlow::word,pFlow::HostSpace>::insertSetElement(
	const uint32IndexContainer& indices, 
	const ViewType1D<word, memory_space> vals
)
{
	if(indices.empty())return true;
	if(indices.size() != vals.size())return false;

	auto maxInd = indices.max(); 
	
	if(this->empty() || maxInd > size()-1 )
	{
		resize(maxInd+1);
	} 

	auto ind = indices.hostView();
	auto s = indices.size();

	for(uint32 i=0; i< s; i++)
	{
		container_[ind[i]] = vals[i];    
	}

	return true;
}

