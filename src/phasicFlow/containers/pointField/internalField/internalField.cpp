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
	const word eventName = message::eventName(message::ITEMS_INSERT);

	if(!varList.checkObjectType<uint32IndexContainer>(eventName))
	{
		fatalErrorInFunction<<"Insertion failed in internalField, "<< eventName <<
		" does not exist or the type is incorrect"<<endl;
		return false;
	}

	const auto& indices = varList.getObject<uint32IndexContainer>(
		eventName);
	bool success = false;
	
	if(varList.contains(name()))
	{
		// a single value is assigned
		if(!varList.checkObjectType<T>(name()))
		{
			fatalErrorInFunction<<"wrong type for variable "<< name()<<endl;
			return false;
		}
		T val = varList.getObject<T>(name());
		success = field_.insertSetElement(indices, val);
		
	}
	else if(word fn = name()+"Vector"; varList.contains(fn))
	{
		// a vector of values is going to be assigned
		if(!varList.checkObjectType<Vector<T>>(fn))
		{
			fatalErrorInFunction<<"wrong type for variable "<< fn<<endl;
			return false;
		}
		const auto& valVec = varList.getObject<Vector<T>>(fn);
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
	const word eventName = message::eventName(message::ITEMS_REARRANGE);

	if(!varList.checkObjectType<uint32IndexContainer>(eventName))
	{
		fatalErrorInFunction<<"Wrong type for variable "<< eventName<<endl;
		return false;
	}
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
	const timeInfo& ti,
	const message& msg, 
	const anyList& varList
)
{
	if(msg.equivalentTo(message::RANGE_CHANGED))
	{
		auto varName = message::eventName(message::RANGE_CHANGED);
		if( !varList.checkObjectType<rangeU32>(varName) )
		{
			fatalErrorInFunction<<"Wrong type for variable "<< varName<<endl;
			return false;
		}

		auto newRange = varList.getObject<rangeU32>(varName);

		if(newRange.end() > size()) 
			field_.resize(newRange.end());
		return true;
	}
	else if(msg.equivalentTo(message::ITEMS_DELETE))
	{
		// do nothing
		return true;
	}
	else if(msg.equivalentTo(message::ITEMS_INSERT))
	{
		return insert(varList);
	}
	else if(msg.equivalentTo(message::ITEMS_REARRANGE))
	{
		return rearrange(varList);
	}
	else
	{
		fatalErrorInFunction<<"hear changes in internal field is not processing "<< 
		message::eventName(message::RANGE_CHANGED)<<
		" event with message code "<< msg<<endl;
		return false;
	}
	
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