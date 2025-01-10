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


template<class T, class MemorySpace>
bool pFlow::internalField<T, MemorySpace>::insert(const anyList& varList)
{
	const word eventName = message::eventName(message::ITEM_INSERT);

	const auto& indices = varList.getObject<uint32IndexContainer>(
		eventName);
	bool success = false;
	
	if(varList.contains(name()))
	{
		// a single value is assigned
		T val = varList.getObject<T>(name());
		success = field_.insertSetElement(indices, val);
		
	}
	else if(varList.contains(name()+"Vector"))
	{
		// a vector of values is going to be assigned
		const auto& valVec = varList.getObject<Vector<T>>(name()+"Vector");
		success = field_.insertSetElement(indices,valVec);
	}
	else
	{
		success = field_.insertSetElement(indices, defaultValue_);
	}

	if(!success)
	{
		fatalErrorInFunction;
		return false;
	}

	return true;
}

template<class T, class MemorySpace>
bool pFlow::internalField<T, MemorySpace>::rearrange(const anyList& varList)
{
	const word eventName = message::eventName(message::ITEM_REARRANGE);

	const auto& indices = varList.getObject<uint32IndexContainer>(
		eventName);
	
	field_.reserve( internalPoints_.capacity());
	field_.resize(internalPoints_.size());
	if(!field_.reorderItems(indices))
	{
		fatalErrorInFunction<<
		"cannot reorder items in field "<< name()<<endl;
		return false;
	}

	return true;
}


template<class T, class MemorySpace>
pFlow::internalField<T, MemorySpace>::internalField
(
	const word& name, 
	const internalPoints& internal,
	const T& defVal
)
:
    observer
    (
		&internal, 
		defaultMessage_
    ),
	field_
	(
		name,
		"ineternalField",
		internal.capacity(),
		internal.size(),
		RESERVE()
	),
	defaultValue_(defVal),
	internalPoints_(internal)
{}

template<class T, class MemorySpace>
pFlow::internalField<T, MemorySpace>::internalField
(
	const word &name, 
	const internalPoints &internal, 
	const T& defVal,
	const T &val
)
:
	observer
    (
		&internal, 
		defaultMessage_
    ),
	field_
	(
		name,
		"ineternalField",
		internal.capacity(),
		internal.size(),
		RESERVE()
	),
	defaultValue_(defVal),
	internalPoints_(internal)
{
	fillInternal(val);
} 


template <class T, class MemorySpace>
typename pFlow::internalField<T, MemorySpace>::FieldTypeHost 
	pFlow::internalField<T, MemorySpace>::activeValuesHost() const
{
	auto maskH = internalPoints_.activePointsMaskHost();
    auto fieldH = field_.hostView();

	FieldTypeHost aField
	(
		field_.name(), 
		field_.fieldKey(),
		maskH.numActive(),
		maskH.numActive(),
		RESERVE()
	);

    auto aRange = maskH.activeRange();

	uint32 n = 0;
    for(auto i=aRange.start(); i<aRange.end(); i++)
    {
        if( maskH.isActive(i) )
        {
            aField[n] = fieldH[i];
            n++;
        }
    }
    return aField;
}

template <class T, class MemorySpace>
bool pFlow::internalField<T, MemorySpace>:: hearChanges
(
	real t,
	real dt,
	uint32 iter,
	const message& msg, 
	const anyList& varList
)
{
	if(msg.equivalentTo(message::CAP_CHANGED))
	{
		auto newCap = varList.getObject<uint32>(
			message::eventName(message::CAP_CHANGED));

		field_.reserve(newCap);
	}
	if(msg.equivalentTo(message::SIZE_CHANGED))
	{
		auto newSize = varList.getObject<uint32>(
			message::eventName(message::SIZE_CHANGED));
		field_.resize(newSize);
	}
	if(msg.equivalentTo(message::ITEM_DELETE))
	{
		// do nothing
	}
	if(msg.equivalentTo(message::ITEM_INSERT))
	{
		return insert(varList);
	}
	if(msg.equivalentTo(message::ITEM_REARRANGE))
	{
		return rearrange(varList);

	}
	return true;
}

template<class T, class MemorySpace>
bool pFlow::internalField<T, MemorySpace>::write
(
	iOstream& os, 
	const IOPattern& iop
)const
{
	auto aField = activeValuesHost();
	return aField.write(os, iop);
}