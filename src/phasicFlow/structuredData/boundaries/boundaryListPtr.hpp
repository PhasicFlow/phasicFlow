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
#ifndef __boundaryListPtr_hpp__
#define __boundaryListPtr_hpp__

#include <bitset>
#include <array>

#include "uniquePtr.hpp"
#include "error.h"
#include "iOstream.hpp"

namespace pFlow
{

template<typename BoundaryType>
class boundaryListPtr 
{
private:
	std::array<BoundaryType*,6> 		boundaries_;

	std::bitset<6>						activeBoundaries_=0B000000;

public:

	boundaryListPtr():
		boundaries_({nullptr, nullptr, nullptr, nullptr, nullptr, nullptr})
	{}

	void set(size_t i, uniquePtr<BoundaryType>&& ptr )
	{
		if( i > boundaries_.size() )
		{
			fatalErrorInFunction<<
			"Out of range access of boundaryListPtr members. List size is "<<
			size() << "and you are accessing "<< i << "\n";
			fatalExit;
		}

		if(boundaries_[i]) delete boundaries_[i];
		boundaries_[i] = ptr.release();

		if(boundaries_[i])
		{
			// query if this boundary active or not
			activeBoundaries_.set(i,boundaries_[i]->isActive());	
		}
	}

	~boundaryListPtr()
	{
		clear();
	}

	void clear()
	{
		for(auto& bndry:boundaries_)
		{
			if(bndry != nullptr)
			{
				delete bndry;
				bndry = nullptr;
			}
		}
		activeBoundaries_.reset();
	}

	// - access to ith element
	inline    
	BoundaryType& operator[](size_t i)
	{
		if(boundaries_[i]== nullptr)
		{
			fatalErrorInFunction<<"Accessing nullptr element"<<endl;
			fatalExit;
		}
		return *boundaries_[i];
	}
	
	// - const access to ith element
	//   fatalexit if out of range or nullptr  
	inline  
	const BoundaryType& operator[](size_t i) const
	{
		if(boundaries_[i]== nullptr)
		{
			fatalErrorInFunction<<"Accessing nullptr element"<<endl;
			fatalExit;
		}
		return *boundaries_[i];
	}

	inline
	BoundaryType* boundaryPtr(size_t i)
	{
		if(boundaries_[i]== nullptr)
		{
			fatalErrorInFunction<<"Accessing nullptr element"<<endl;
			fatalExit;
		}
		return boundaries_[i];
	}

	inline
	const BoundaryType* boundaryPtr(size_t i)const 
	{
		if(boundaries_[i]== nullptr)
		{
			fatalErrorInFunction<<"Accessing nullptr element"<<endl;
			fatalExit;
		}
		return boundaries_[i];
	}

	inline constexpr
	size_t size()const
	{
		return 6ul;
	}

	inline
	size_t firstActive()const
	{
		if(activeBoundaries_[0]) return 0ul;
		if(activeBoundaries_[1]) return 1ul;
		if(activeBoundaries_[2]) return 2ul;
		if(activeBoundaries_[3]) return 3ul;
		if(activeBoundaries_[4]) return 4ul;
		if(activeBoundaries_[5]) return 5ul;
		return 6ul;
	}

	inline
	size_t nextActive(size_t i)const
	{
		if(i>=5ul) return 6ul;
		if(activeBoundaries_[i+1]) 
			return i+1;
		else
			return nextActive(i+1);
	}

	inline
    bool allActive()const
    {
        return activeBoundaries_.all();
    }

    inline
    bool anyActive()const
    {
        return activeBoundaries_.any();
    }
    
    inline
    bool noneActive()const
    {
        return activeBoundaries_.none();
    }
};

}

#define ForAllBoundaries(i,bndryList) for(size_t i = 0ul; i<(bndryList).size(); i++)
#define ForAllBoundariesPtr(i,bndryList) for(size_t i = 0ul; i<bndryList->size(); i++)
#define ForAllActiveBoundaries(i,bndryList) for(size_t i = bndryList.firstActive(); i<bndryList.size(); i=bndryList.nextActive(i))
#define ForAllActiveBoundariesPtr(i,bndryList) for(size_t i = bndryList->firstActive(); i<bndryList->size(); i=bndryList->nextActive(i))

#endif //__boundaryListPtr_hpp__