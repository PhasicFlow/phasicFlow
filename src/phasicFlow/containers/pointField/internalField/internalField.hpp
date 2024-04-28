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

#ifndef __internalField_hpp__
#define __internalField_hpp__

#include "internalPoints.hpp"
#include "Field.hpp"
#include "observer.hpp"
#include "dataIO.hpp"
#include "anyList.hpp"


namespace pFlow
{

template<class T, class MemorySpace=void>
class internalField
:
    public observer
{

public:
	
	using InternalFieldType = internalField<T, MemorySpace>;

	using FieldType         = Field<T, MemorySpace>;

	using FieldTypeHost 	= Field<T, HostSpace>;
  	
	using VectorType  		= typename FieldType::VectorType;

	using memory_space 		= typename FieldType::memory_space;

	using execution_space 	= typename FieldType::execution_space;

protected:

	/// Field
    FieldType           	field_;

	/// @brief value when a new item is added to field
	T 						defaultValue_;

	/// const ref to internal points 
    const internalPoints&   internalPoints_;

	static inline
	const message defaultMessage_ = 
	(
		message::CAP_CHANGED+
		message::SIZE_CHANGED+
		message::ITEM_INSERT+
		message::ITEM_REARRANGE+
		message::ITEM_DELETE
	);

	bool insert(const anyList& varList);

public:

	internalField(
		const word& name, 
		const internalPoints& internal,
		const T& defVal);
	
	internalField(
		const word& name, 
		const internalPoints& internal,
		const T& defVal,
		const T& val);
	
	inline 
	const auto& deviceViewAll()const
	{
		return field_.deviceViewAll();
	}
	inline
	auto deviceView()const
	{
		return field_.deviceView();
	}

	inline
	auto hostView()const
	{
		return field_.hostView();
	}

	inline
	const FieldType& field()const
	{
		return field_;
	}

	inline
	FieldType& field()
	{
		return field_;
	}

	const pFlagTypeDevice& activePointsMaskDevice()const
	{
		return internalPoints_.activePointsMaskDevice();
	}
	
	const pFlagTypeHost& activePointsMaskHost()const
	{
		return internalPoints_.activePointsMaskHost();
	}

	FieldTypeHost activeValuesHost()const;

	inline
	auto size()const
	{
		return field_.size();
	}

	inline
	auto capacity()const
	{
		return field_.capacity();
	}

	inline 
	word name()const
	{
		return field_.name();
	}

	inline 
	word fieldKey()const
	{
		return field_.fieldKey();
	}

	inline
	const T& defaultValue()const
	{
		return defaultValue_;
	}

	inline
	auto activeRange()const
	{
		return internalPoints_.activeRange();
	}

	inline
	auto isAllActive()const
	{
		return internalPoints_.isAllActive();
	}

	inline 
	void fillInternal(const T& val)
	{
		field_.fillField(activeRange(), val);
	}

	inline 
	bool insertSetElement(uint32IndexContainer indices, const T& val)
	{
		return field_.insertSetElement(indices, val);
	}

	inline const Time& time()const
	{
		return internalPoints_.time();
	}

	bool hearChanges
	(
		real t,
		real dt,
		uint32 iter,
		const message& msg, 
		const anyList& varList
	) override;
	

	//// - IO

	bool write(iOstream& os, const IOPattern& iop)const;

};

template<class T, class MemorySpace>
inline
iOstream& operator<<
(
	iOstream& os, 
	const internalField<T,  MemorySpace>& ifeild
)
{
	if( !ifeild.write(os, IOPattern::AllProcessorsDifferent) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}

	return os;
}

} // pFlow

#include "internalField.cpp"
#include "internalFieldAlgorithms.hpp"

#endif // __internalField_hpp__