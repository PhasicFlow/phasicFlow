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
 * @class includeMask
* @brief Base class for creating inclusion masks for data filtering
* 
 * includeMask is an abstract base class for creating masks that filter data
 * from a fieldsDataBase. Derived classes implement specific filtering criteria
 * through the getMask() method which returns a Mask object - a wrapper around
 * a vector of booleans that indicates which elements to include/exclude.
 * 
 * This class follows a factory pattern with create() methods that instantiate
 * the appropriate derived mask type based on dictionary settings.
 * 
 * Derived classes can implement various filtering strategies such as:
 * - Filtering by field values
 * - Filtering by spatial regions
 * - Filtering by predefined criteria
 * 
 * The Mask objects returned by getMask() can be used in postprocessing operations
 * to include only the data points that match the specified criteria.
 */

#ifndef __includeMask_hpp__
#define __includeMask_hpp__

#include <vector> 

#include "virtualConstructor.hpp"

namespace pFlow
{
	class dictionary;
}

namespace pFlow::postprocessData
{

// forward declaration
class fieldsDataBase;


class includeMask
{
public:

	/// @brief Wrapper around a boolean vector that represents elements to include/exclude
	/// Provides a functional interface to access the underlying mask vector
	/// where true indicates inclusion and false indicates exclusion.
	class Mask
	{
		/// @brief Reference to the underlying boolean vector
		const std::vector<bool>& mask_;

		public:

		/// @brief Constructor from a boolean vector
		/// @param msk Boolean vector where true means include, false means exclude
		Mask(const std::vector<bool>& msk)
		:
			mask_(msk)
		{}

		/// @brief Copy constructor
		Mask(const Mask&) = default;

		/// @brief Move constructor
		Mask(Mask&&) = default;
				
		/// @brief Destructor
		~Mask()=default;

		/// @brief Get the size of the mask
		auto size()const
		{
			return mask_.size();
		}

		/// @brief Check if element at index i should be included
		/// @param i Index to check
		bool operator()(uint32 i)const
		{
			return mask_[i];
		}
	};

private:
	
	/// @brief Reference to the database containing fields to be filtered
    fieldsDataBase&     database_;
	
public:

	TypeInfo("includeMask");

	/// @brief Constructor
	/// @param opDict Dictionary containing operation settings
	/// @param feildsDB Database of fields 
	includeMask(const dictionary& opDict, fieldsDataBase& feildsDB);

	/// @brief Constructor with explicit type
	/// @param type Type of mask to create
	/// @param opDict Dictionary containing operation settings
	/// @param feildsDB Database of fields 
    includeMask(const word& type, const dictionary& opDict, fieldsDataBase& feildsDB);

	/// @brief Virtual destructor
	virtual ~includeMask() = default;

	/// @brief Virtual constructor pattern implementation using dictionary
    create_vCtor
    (
        includeMask,
        dictionary,
        (
            const dictionary& opDict, fieldsDataBase& feildsDB
        ),
        (opDict, feildsDB)		
    );

	/// @brief Virtual constructor pattern implementation using type and dictionary
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
	 
	/// @brief Get const access to the database
	/// @return Const reference to the fields database
	const fieldsDataBase& database()const
	{
		return database_;
	}

	/// @brief Get non-const access to the database
	/// @return Reference to the fields database
    fieldsDataBase& database()
    {
        return database_;
    }

	/// @brief Get the mask for filtering elements
	/// @return Mask object indicating which elements to include
    virtual 
    Mask getMask()= 0;

	/// @brief Factory method to create appropriate mask type from dictionary
	/// @param opDict Dictionary with mask settings including type
	/// @param feildsDB Database of fields to filter
	/// @return Unique pointer to created mask instance
	static 
	uniquePtr<includeMask> create(
        const dictionary& opDict, 
        fieldsDataBase& feildsDB);
    
	/// @brief Factory method to create mask with explicit type
	/// @param type Type of mask to create
	/// @param opDict Dictionary with mask settings
	/// @param feildsDB Database of fields to filter
	/// @return Unique pointer to created mask instance
    static
    uniquePtr<includeMask> create(
        const word& type,
        const dictionary& opDict, 
        fieldsDataBase& feildsDB);
	
};



} // pFlow::postprocessData

#endif //__IncludeMask_hpp__


