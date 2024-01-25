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


template <typename Type>
bool pFlow::setFieldEntry::checkForType()const
{
	word typeName( entry_.firstPart() );
	return basicTypeName<Type>() == typeName;
};

template <typename Type>
bool pFlow::setFieldEntry::checkForTypeAndValue()const
{
	if( !checkForType<Type>() ) return false;
	
	Type defValue = entry_.secondPartVal<Type>();
	CONSUME_PARAM(defValue);
	return true;
}

template <typename Type>
pFlow::uniquePtr<pFlow::pointField_H<Type>>
	 pFlow::setFieldEntry::setPointFieldDefaultValueNew
(
	pointStructure& pStruct,
	bool verbose
)
{

	if( !checkForType<Type>() ) return nullptr;
	
	Type defValue = entry_.secondPartVal<Type>();
	
	if(verbose)
		REPORT(2)<<"Creating pointField " << Green_Text(fieldName())<< " with default value " << Cyan_Text(defValue)<<
	             " in repository "<< pStruct.owner()->name() <<END_REPORT;
	
	auto Ptr = makeUnique<pointField_H<Type>>(
		objectFile
		(
			fieldName(),
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_ALWAYS
		),
		pStruct,
		defValue,
		defValue	
	);
	
	return Ptr;
}

/*template <typename Type>
void* pFlow::setFieldEntry::setPointFieldDefaultValueStdNew
(
	repository& owner,
	pointStructure& pStruct,
	bool verbose
)
{

	if( !checkForType<Type>() ) return nullptr;
	
	Type defValue = entry_.secondPartVal<Type>();
	
	if(verbose)
		REPORT(2)<<"Creating pointField " << Green_Text(fieldName())<< " with default value " << cyanText(defValue)<<
	             " in repository "<< owner.name() <<END_REPORT;

	// by default we perform operations on host
	auto& field = 
	owner.emplaceObject<pointField<Vector,Type, vecAllocator<Type>>>
	(
		objectFile
		(
			fieldName(),
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_ALWAYS
		),
		pStruct,
		defValue
	);

	return &field;
}*/

template <typename Type>
bool pFlow::setFieldEntry::setPointFieldSelected
(
	repository& owner,
	uint32IndexContainer& selected,
	bool verbose
)
{
	if( !checkForType<Type>() ) return false;

	
	auto fName = fieldName();

	if( !owner.lookupObjectName(fName) )
	{
		fatalErrorInFunction<<
		"Cannot find "<< fName << " in repository " << owner.name() << ". \n";
		return false;
	}

	Type value = entry_.secondPartVal<Type>();

	if(verbose)
		REPORT(2)<< "Setting selected points of " << Green_Text(fName)
		 		 << " to value " << Cyan_Text(value) <<END_REPORT;
	
	
	auto fieldTypeName = owner.lookupObjectTypeName(fName);

	if( getTypeName<pointField_H<Type>>() == fieldTypeName )
	{
		
		auto& field = owner.lookupObject<pointField_H<Type>>(fName);
		if(field.insertSetElement(selected, value))
			return true;
		else
			return false;
	}

	fatalErrorInFunction<<
	fieldTypeName<< " is not a supported field type for setFieldEntry.\n";
	return false;
}

/*template <typename Type>
void* pFlow::setFieldEntry::setPointFieldSelectedStd
(
	repository& owner,
	int32IndexContainer& selected,
	bool verbose
)
{
	
	if( !checkForType<Type>() ) return nullptr;

	
	auto fName = fieldName();

	if( !owner.lookupObjectName(fName) )
	{
		fatalErrorInFunction<<
		"  Cannot find "<< fName << " in repository " << owner.name() << ". \n";
		return nullptr;
	}

	
	Type value = entry_.secondPartVal<Type>();

	if(verbose)
		REPORT(2)<< "Setting selected points of " << Green_Text(fName)
		 		 << " to value " << cyanText(value) <<END_REPORT;
	
	
	auto fieldTypeName = owner.lookupObjectTypeName(fName);

	if( pointField<Vector, Type, vecAllocator<Type>>::TYPENAME() == fieldTypeName )
	{
		auto& field = owner.lookupObject<pointField<Vector,Type, vecAllocator<Type>>>(fName);
		if(field.insertSetElement(selected, value))
			return &field;
		else
			return nullptr;
	}

	return nullptr;
}*/