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

#ifndef __includeMask_hpp__
#define __includeMask_hpp__

#include <vector> 

#include "virtualConstructor.hpp"

namespace pFlow
{


class fieldsDataBase;
class dictionary;

class includeMask
{
public:

	class Mask
	{
		const std::vector<bool>& mask_;

		public:

		Mask(const std::vector<bool>& msk)
		:
			mask_(msk)
		{}

		Mask(const Mask&) = default;

		Mask(Mask&&) = default;
				
		~Mask()=default;

		auto size()const
		{
			return mask_.size();
		}

		bool operator()(uint32 i)const
		{
			return mask_[i];
		}
	};

private:
	
    fieldsDataBase&     database_;
	
public:

	TypeInfo("includeMask");

	includeMask(const dictionary& opDict, fieldsDataBase& feildsDB);

    includeMask(const word& type, const dictionary& opDict, fieldsDataBase& feildsDB);

	virtual ~includeMask() = default;

    create_vCtor
    (
        includeMask,
        dictionary,
        (
            const dictionary& opDict, fieldsDataBase& feildsDB
        ),
        (opDict, feildsDB)		
    );

    create_vCtor
    (
        includeMask, 
        word,
        (
            const word& type, 
            const dictionary& opDict, 
            fieldsDataBase& feildsDB
        ),
        (type, opDict, feildsDB)
    );
	 
	const fieldsDataBase& database()const
	{
		return database_;
	}

    fieldsDataBase& database()
    {
        return database_;
    }

    virtual 
    Mask getMask()= 0;

	static 
	uniquePtr<includeMask> create(
        const dictionary& opDict, 
        fieldsDataBase& feildsDB);
    
    static
    uniquePtr<includeMask> create(
        const word& type,
        const dictionary& opDict, 
        fieldsDataBase& feildsDB);
	
};



} // pFlow

#endif //__IncludeMask_hpp__


