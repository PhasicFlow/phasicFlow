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

#ifndef __pointFieldToVTK_hpp__
#define __pointFieldToVTK_hpp__

#include "systemControl.hpp"
#include "pointStructure.hpp"
#include "pointFields.hpp"

namespace pFlow::PFtoVTK
{

	bool convertTimeFolderPointFields(
		systemControl &control,
		const fileSystem &destPath,
		const word &bName);

	bool convertTimeFolderPointFieldsSelected(
		systemControl &control,
		const fileSystem &destPath,
		const word &bName,
		const wordVector &fieldsName,
		bool mustExist);

	bool addUndstrcuturedGridField(
		iOstream &os,
		realx3 *position,
		uint32 numPoints);

	bool convertRealTypePointField(
		iOstream &os,
		const IOfileHeader &header,
		pointStructure &pStruct);

	bool convertRealx3TypePointField(
		iOstream &os,
		const IOfileHeader &header,
		pointStructure &pStruct);

	template <typename IntType>
	bool addIntPointField(
		iOstream &os,
		const word &fieldName,
		IntType *field,
		uint32 numData);

	bool addRealPointField(
		iOstream &os,
		const word &fieldName,
		const real *field,
		uint32 numData);

	bool addRealx3PointField(
		iOstream &os,
		const word &fieldName,
		const realx3 *field,
		uint32 numData);

	template <typename Type>
	bool checkFieldType(word objectType);

	bool regexCheck(const word &TYPENAME, const word &fieldType);

	template <typename Type>
	inline bool checkFieldType(word objectType)
	{
		return regexCheck(pointField<Type>::TYPENAME(), objectType);
	}

	template <typename IntType>
	inline bool convertIntPointField(
		iOstream &os,
		const IOfileHeader &header,
		pointStructure &pStruct)
	{

		using PointFieldType = pointField<IntType, HostSpace>;

		word objectType = header.objectType();

		if (!checkFieldType<IntType>(objectType))
		{
			return false;
		}

		auto field = PointFieldType(
			header,
			pStruct,
			static_cast<IntType>(0));

		const IntType *data = field.deviceViewAll().data();

		REPORT(1) << "writing " << Green_Text(header.objectName()) << " field to vtk.\n";

		return addIntPointField(
			os,
			header.objectName(),
			data,
			pStruct.numActive());
	}

	template <typename IntType>
	inline bool addIntPointField(
		iOstream &os,
		const word &fieldName,
		IntType *field,
		uint32 numData)
	{
		if (numData == 0)
			return true;

		os << "FIELD FieldData 1\n"
		   << fieldName << " 1 " << numData << " int\n";
		for (uint32 i = 0; i < numData; ++i)
		{
			os << field[i] << '\n';
		}

		return true;
	}

}

#endif
