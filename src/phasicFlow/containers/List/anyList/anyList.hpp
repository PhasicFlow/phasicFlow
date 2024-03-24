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

#ifndef __anyList_hpp__
#define __anyList_hpp__

#include <any>

#include "types.hpp"
#include "List.hpp"


namespace pFlow
{


class anyList
{

public:

	using anyListType 	= List<std::any>;

	using reference 	= typename anyListType::reference;

	using iterator 		= typename anyListType::iterator;

	using const_iterator= typename anyListType::const_iterator;


protected:

	/// Contains a list of variables with any type 
	anyListType 	anyList_;

	/// List of variable names in anyList_ 
	wordList 		names_;

	wordList 		types_;	

public:

	// - Type info
	TypeInfoNV("anyList");

	//// - Constructors

		// All five constructors are created by compiler 
	

	//// -  Methods
		
		/// Size of container
		size_t size()const;

		/// Is container empty
		bool empty()const;

		/// Does container contain this variable name?
		bool contains(const word& name)const;

		/// List of varibales names
		const wordList& names()const;

		/// Create variable using constructor in-place 
		template<typename T, typename... Args>
		reference emplaceBack(const word& name, Args&&... args)
		{
			if( contains(name))
			{
				fatalErrorInFunction<<
				"variable name "<< name << " already exists in the anyList."<<endl<<
				"list of variables is "<<names_<<endl;
				fatalExit;
			}
			names_.push_back(name);
			types_.push_back(getTypeName<T>());
			return anyList_.emplace_back( 
				std::in_place_type<T>, 
				std::forward<Args>(args)...);
		}

		/// Create variable using copy constructor
		template<typename T>
		reference emplaceBack(const word& name, const T & other)
		{
			if( contains(name) )
			{
				fatalErrorInFunction<<
				"variable name "<< name << " already exists in the anyList."<<endl<<
				"list of variables is "<<names_<<endl;
				fatalExit;
			}
			names_.push_back(name);
			types_.push_back(getTypeName<T>());
			return anyList_.emplace_back(std::in_place_type<T>, other);
		}

		/// Create variable using move constructor 
		template<typename T>
		reference emplaceBack(const word& name, T&& other)
		{
			if( contains(name) )
			{
				fatalErrorInFunction<<
				"variable name "<< name << " already exists in the anyList."<<endl<<
				"list of variables is "<<names_<<endl;
				fatalExit;
			}
			names_.push_back(name);
			types_.push_back(getTypeName<T>());
			return anyList_.emplace_back(std::in_place_type<T>, std::move(other));
		}

		/// Get the reference to variable by index
		template<typename T>
		T& getObject(size_t i)
		{
			std::any *a = &(*anyList_.pos(i));
			if(!a->has_value())
			{
				fatalErrorInFunction<<
				"any does not have a value for dereferencing. "<<
				"index in anyList is "<<i<<"."<<endl;
				fatalExit;
			}
			return *std::any_cast<T>(a);
		}

		/// Get the reference to variable by name 
		template<typename T>
		T& getObject(const word& name)
		{
			int32 i = names_.findi(name);
			if(i == -1 )
			{
				fatalErrorInFunction<<
				"variable name "<< name << " does not exist in the anyList."<<endl<<
				"list of variables is "<<names_<<endl;
				fatalExit;
			}
			std::any *a = &(*anyList_.pos(i));
			if(!a->has_value())
			{
				fatalErrorInFunction<<
				"any does not have a value for dereferencing. "<<
				"index in anyList is "<<i<<"."<<endl;
				fatalExit;
			}
			return *std::any_cast<T>(a);
		}

		/// Get the const reference to variable by name 
		template<typename T>
		const T& getObject(const word& name)const
		{
			int32 i = names_.findi(name);
			if(i == -1 )
			{
				fatalErrorInFunction<<
				"variable name "<< name << " does not exist in the anyList."<<endl<<
				"list of variables is "<<names_<<endl;
				fatalExit;
			}
			const std::any *a = &(*anyList_.pos(i));
			if(!a->has_value())
			{
				fatalErrorInFunction<<
				"any does not have a value for dereferencing. "<<
				"index in anyList is "<<i<<"."<<endl;
				fatalExit;
			}
			return *std::any_cast<const T>(a);
		}

		/// Get the const reference to object by index
		template<typename T>
		const T& getObject(size_t i)const
		{
			const std::any *a = &(*anyList_.pos(i));
			if(!a->has_value())
			{
				fatalErrorInFunction<<
				"any does not have a value for dereferencing. "<<
				"index in anyList is "<<i<<"."<<endl;
				fatalExit;
			}
			return *std::any_cast<const T>(a);
		}

		/// Get the pointer to variable by index
		template<typename T>
		T* getObjectPtr(size_t i)
		{
			if(i>= size())return nullptr;

			std::any *a = &(*anyList_.pos(i));
			if( a->has_value())
			{
				return std::any_cast<T>(a);	
			}
			else
			{
				return nullptr;
			}	
		}

		/// Get the const pointer to variable by index 
		template<typename T>
		const T* getObjectPtr(size_t i)const
		{
			if(i>=size())return nullptr;
			const std::any *a = &(*anyList_.pos(i));
			if( a->has_value())
			{
				return std::any_cast<const T>(a);	
			}
			else
			{
				return nullptr;
			}	
		}
			
};



} // pFlow


#endif //__anyList_hpp__
