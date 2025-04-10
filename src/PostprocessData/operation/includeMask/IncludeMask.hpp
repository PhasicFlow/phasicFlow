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

#ifndef __IncludeMask_hpp__
#define __IncludeMask_hpp__

#include "dictionary.hpp"
#include "includeMask.hpp"
#include "fieldsDataBase.hpp"
#include "maskOperations.hpp"
#include "Time.hpp"


namespace pFlow
{


template<typename T, typename Operator>
class IncludeMask
:
	public includeMask
{
public:

    using Mask = typename includeMask::Mask;

private:
	
    std::vector<bool>       mask_;

    Operator                operator_;

    word                    fieldName_;
    
    timeValue               lastUpdated_ = -1;

    bool updateMask()
    {
        timeValue t = database().currentTime();
        
        if( equal( t, lastUpdated_)) return true;
        
        span<T> s;
        if constexpr (std::is_same_v<T,real>)
        {
            s = database().updateFieldReal(fieldName_);
        }
        else if constexpr ( std::is_same_v<T,realx3>)
        {
            s = database().updateFieldRealx3(fieldName_);
        }
        else if constexpr( std::is_same_v<T,realx4>)
        {
            s = database().updateFieldRealx4(fieldName_);   
        }
        else
        {
            fatalErrorInFunction<<"Type "<< getTypeName<T>()
                <<" is not supported for  IncludeMask for field "
                << fieldName_ << endl;
            return false;
        }
        
        
        mask_.resize(s.size());

        for(uint32 i=0; i<s.size(); i++)
        {
            mask_[i] = operator_(s[i]);
        }

        lastUpdated_ = t ;
        
        return true;
    }

    static
    word operatorName()
    {
        return Operator::TYPENAME();
    }

public:

	TypeInfoTemplate12("IncludeMask", T, Operator);

	IncludeMask(
		const dictionary& dict, 
        fieldsDataBase& feildsDB)
	:
		includeMask(dict, feildsDB),
		operator_(dict.subDict(operatorName()+"Info")),
        fieldName_(
            dict.subDict
            (
                operatorName()+"Info"
            ).getVal<word>("field"))
	{}

	add_vCtor(
		includeMask,
		IncludeMask,
		dictionary);

    Mask getMask() override
    {
        updateMask();
        return Mask(mask_);
    }
	
};


template<typename T>
class IncludeMask<T,allOp<T>>
:
	public includeMask
{
public:

    using Mask = typename includeMask::Mask;

private:
	
    std::vector<bool>       mask_;

    timeValue               lastUpdated_ = -1;

    bool updateMask()
    {
        timeValue t = database().currentTime();
        
        if( equal( t, lastUpdated_)) return true;
        
        span<realx3>  s = database().updatePoints();        
        mask_.resize(s.size(), true);

        lastUpdated_ = t ;
        
        return true;
    }   

public:

	TypeInfoTemplate12("IncludeMask", T, allOp<int8>);

	IncludeMask(
		const dictionary& opDict, 
        fieldsDataBase& feildsDB)
	:
		includeMask(opDict, feildsDB)
	{
        span<realx3>  s = database().updatePoints();        
        mask_.resize(s.size(), true);
	}

	add_vCtor(
		includeMask,
		IncludeMask,
		dictionary);

    Mask getMask()override
    {
        updateMask();
        return Mask(mask_);
    }
	
};


} // pFlow

#endif //__IncludeMask_hpp__


