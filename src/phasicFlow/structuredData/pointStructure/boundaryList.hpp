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


#ifndef __boundaryList_hpp__
#define __boundaryList_hpp__

#include "boundaryBase.hpp"
#include "ListPtr.hpp"

#include "streams.hpp"

namespace pFlow
{

class simulationDomain;

class boundaryList
{

protected:

	//// - data members
        internalPoints& 		internal_;
		
        const simulationDomain& simDomain_;

        ListPtr<boundaryBase> 	boundaryList_;

        bool                    listSet_ = false;

    

public:
	
	/// type info
	TypeInfo("boundaryList");
	

	//// - Constructors

	boundaryList(
		const simulationDomain& simD, 
		internalPoints& internal);

	//~boundaryList() = default;

  ~boundaryList() = default;

  bool updateLists();
};

} // pFlow




#endif //__boundaryList_hpp__


