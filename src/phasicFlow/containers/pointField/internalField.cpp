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
pFlow::internalField<T, MemorySpace>::internalField
(
	const word& name, 
	const internalPoints& internal
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
	internalPoints_(internal)
{}

template<class T, class MemorySpace>
pFlow::internalField<T, MemorySpace>::internalField
(
	const word &name, 
	const internalPoints &internal, 
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