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

/**
 * @brief A template class implementing includeMask for filtering data based on field values
 * 
 * The IncludeMask class creates boolean masks that identify which elements from a field
 * satisfy a given condition. It applies an operator to each element of a field and
 * generates a mask (vector of booleans) where true means the element satisfies the
 * condition and should be included.
 * 
 * @tparam T The data type of the field (real, realx3, realx4)
 * @tparam Operator The operation class that defines the condition (e.g., greaterThanOp)
 * 
 * The class has a specialized implementation for allOp operator which includes all elements.
 * 
 * Usage example in postprocessDataDict:
 * ```
 * // Create a dictionary with the required configuration for filtering
 * 
 * {
 *     includeMask lessThan;
 *
 *     // Diameter of par1 is 0.003, so these settings
 *     // will select only particles of type par1
 *     lessThanInfo
 *     {
 *         field   diameter;
 *         value   0.0031;
 *     }
 *    
 * }
 * 
 * ```
 */

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

    /// Type alias for the mask container returned by getMask()
    using Mask = typename includeMask::Mask;

private:
	
    /// Boolean vector the filtering status of each element (true = include)
    std::vector<bool>       mask_;

    /// Comparison operator instance that evaluates the filtering condition
    Operator                operator_;

    /// Name of the field to apply filtering on
    word                    fieldName_;
    
    /// Timestamp when mask was last updated (-1 indicates never updated)
    timeValue               lastUpdated_ = -1;

    /// Updates the mask based on current field values if needed, returns true if successful
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

    /// Returns the name of the operator as a string (from operator's TYPENAME)
    static
    word operatorName()
    {
        return Operator::TYPENAME();
    }

public:

	TypeInfoTemplate12("IncludeMask", T, Operator);

    /// Constructs an IncludeMask using settings from dictionary and field database
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
    
    /// Add virtual constructor pattern for creating instances
	add_vCtor(
		includeMask,
		IncludeMask,
		dictionary);

    /// Returns the mask for filtering elements (updates the mask if necessary)
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


