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

#ifndef __vtkFile_hpp__
#define __vtkFile_hpp__ 

#include "types.hpp"
#include "uniquePtr.hpp"
#include "fileSystem.hpp"
#include "streams.hpp"

namespace pFlow
{


class vtkFile
{
protected:

	fileSystem dirPath_;

	word       baseName_;

	real  	   time_ = 0.0;

	bool		append_=false;

	uniquePtr<oFstream>  oStream_= nullptr;

	bool openStream(bool wHeader);

	virtual bool writeHeader();

public:

	vtkFile(
		const fileSystem dir, 
		const word& bName, 
		real time,
		bool append = false);

	virtual ~vtkFile() = default;

	inline oFstream& operator ()()
	{
		if(!oStream_)
		{
			if(!openStream(!append_))
			{
				fatalErrorInFunction<<
				"  error in opening vtkFile "<< fileName() <<endl;
				fatalExit;
			}
		}
		return oStream_();
	}

	inline explicit operator bool() const
	{
		if(!oStream_)return false;
		if(oStream_().fail())return false;
		return true;
	}

	inline bool operator!()const
	{
		if( !oStream_) return true;
		if( oStream_().fail() )return true;
		
		return false;
	}

	virtual fileSystem fileName()const;	

};

}

#endif //__vtkFile_hpp__
