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

#include "insertionRegion.hpp"
#include "dictionary.hpp"
#include "insertion.hpp"
#include "particles.hpp"
#include "twoPartEntry.hpp"
#include "types.hpp"

namespace pFlow
{
template<typename T>
bool
setOneEntry(const twoPartEntry& tpEntry, anyList& varList)
{
	if (getTypeName<T>() != tpEntry.firstPart())
		return false;

	T val = tpEntry.secondPartVal<T>();
	varList.emplaceBack(tpEntry.keyword(), val);

	return true;
}

bool
readOneEtrty(const dataEntry& entry, anyList& varList)
{
	twoPartEntry stField(entry);

	if (!(setOneEntry<real>(stField, varList) ||
	      setOneEntry<realx3>(stField, varList) ||
	      setOneEntry<realx4>(stField, varList) ||
	      setOneEntry<int8>(stField, varList) ||
	      setOneEntry<uint8>(stField, varList) ||
	      setOneEntry<uint32>(stField, varList) ||
	      setOneEntry<uint64>(stField, varList) ||
	      setOneEntry<int32>(stField, varList) ||
	      setOneEntry<int64>(stField, varList)))
	{
		fatalErrorInFunction << "un-supported data type " << stField.firstPart()
		                     << endl;
		return false;
	}
	return true;
}
}

bool
pFlow::insertionRegion::readInsertionRegion(const dictionary& dict)
{
	type_ = dict.getVal<word>("regionType");

	rate_ = dict.getVal<real>("rate");

	pRegion_ = peakableRegion::create(type_, dict.subDict(type_ + "Info"));

	mixture_ = makeUnique<shapeMixture>(
	  dict.subDict("mixture"),
	  insertion_.Particles().getShapes().shapeNameList()
	);

	numInserted_ = mixture_().totalInserted();

	if (dict.containsDictionay("setFields"))
	{
		setFieldDict_ =
		  makeUnique<dictionary>("setFields", dict, dict.subDict("setFields"));
	}

	if (setFieldDict_)
	{
		if (!readSetFieldDict())
		{
			fatalErrorInFunction << "Error in reading dictionary "
			                     << setFieldDict_().globalName() << endl;
			return false;
		}
	}

	return true;
}

bool
pFlow::insertionRegion::writeInsertionRegion(dictionary& dict) const
{
	if (!dict.add("type", type_))
		return false;

	if (pRegion_)
	{
		auto& prDict = dict.subDictOrCreate(type_ + "Info");
		if (!pRegion_().write(prDict))
			return false;
	}

	if (mixture_)
	{
		auto& mixDict = dict.subDictOrCreate("mixture");
		if (!mixture_().write(mixDict))
			return false;
	}

	/*if(setFields_)
	{
	    auto& sfDict = dict.subDictOrCreate("setFields");
	    setFields_().write(sfDict);
	}*/

	return true;
}

bool
pFlow::insertionRegion::readSetFieldDict()
{
	wordList Keys = setFieldDict_().dataEntryKeywords();

	for (const auto& key : Keys)
	{
		if (!readOneEtrty(setFieldDict_().dataEntryRef(key), setFieldList_))
		{
			return false;
		}
	}

	return true;
}

pFlow::insertionRegion::insertionRegion(
  const word&      name,
  const insertion& instn
)
  : name_(name),
    dict_(instn.subDict(name)),
    insertion_(instn),
    tControl_(dict_, "insertion")
{
	if (!readInsertionRegion(dict_))
	{
		fatalExit;
	}
}

const pFlow::pointStructure&
pFlow::insertionRegion::pStruct() const
{
	return Insertion().pStruct();
}

pFlow::uint32
pFlow::insertionRegion::numberToBeInserted(uint32 iter, real t, real dt)
{
	if (!tControl_.isInRange(iter, t, dt))
		return 0u;

	if (tControl_.isTimeStep())
	{
		return static_cast<uint32>(
		  (iter - tControl_.startIter() + tControl_.iInterval()) * dt * rate_ -
		  numInserted_
		);
	}
	else
	{
		return static_cast<uint32>(
		  (t - tControl_.startTime() + tControl_.rInterval()) * rate_ -
		  numInserted_
		);
	}
}
