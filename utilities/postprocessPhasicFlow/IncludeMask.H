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

#ifndef __IncludeMask_H__
#define __IncludeMask_H__


#include "includeMask.H"

namespace pFlow
{


template<typename T>
struct greaterThanOp
{
	TypeNameNV("greaterThan");

 	inline
 	bool operator()(const T &compVal, const T &val) const {
   		return val > compVal; }
};

template<typename T>
struct greaterThanEqOp
{
	TypeNameNV("greaterThanEq");

 	inline
 	bool operator()(const T &compVal, const T &val) const {
   		return val >= compVal; }
};

template<typename T>
struct lessThanOp
{
	TypeNameNV("lessThan");

	inline
	bool operator()(const T &compVal, const T &val) const {
		return val < compVal; }
};

template<typename T>
struct lessThanEqOp
{
	TypeNameNV("lessThanEq");

	inline
	bool operator()(const T &compVal, const T &val) const {
		return val <= compVal; }
};

template<typename T>
struct equalOp
{
	TypeNameNV("equal");

	inline
	bool operator()(const T &compVal, const T &val) const {
		return equal(val , compVal); }
};


template<typename T>
struct betweenOp
{
	TypeNameNV("between");

	inline
	bool operator()(const T &compVal1, const T &compVal2 ,const T &val) const {
		return val>compVal1 && val<compVal2; }
};


template<typename T>
struct betweenEqOp
{
	TypeNameNV("betweenEq");

	inline
	bool operator()(const T &compVal1, const T &compVal2 ,const T &val) const {
		return val>=compVal1 && val<=compVal2; }
};

template<typename T>
struct allOp
{
	TypeNameNV("all");

	inline
	bool operator()() const {return true; }
};



template<typename T, template<class> class Operator>
class compareOne
{
public:

	using opertorType = Operator<T>;

protected:
	T 				compValue_{};
	opertorType 	operator_{};
public:

	TypeNameNV(Operator<T>::TYPENAME());

	compareOne(const dictionary& dict)
	:
		compValue_(dict.getVal<T>("value"))
	{}

	bool operator()(const T& value)const
	{
		return operator_(compValue_, value);
	}
};

template<typename T, template<class> class Operator>
class compareTwo
{
public:
	using opertorType = Operator<T>;
protected:
	T 				compValue1_;
	T  				compValue2_;
	opertorType 	operator_{};
public:
	
	TypeNameNV(opertorType::TYPENAME());

	compareTwo(const dictionary& dict)
	:
		compValue1_(dict.getVal<T>("value1")),
		compValue2_(dict.getVal<T>("value2"))
	{}

	bool operator()(const T& value)const
	{
		return operator_(compValue1_, compValue2_, value);
	}
};

template<typename T, typename Operator>
class compareZero
{
protected:
	Operator 	operator_{};
public:

	TypeNameNV(Operator::TYPENAME());
	compareZero(const dictionary& dict);

	bool operator()(const T& value) const
	{
		return operator_();
	}
};

template<typename T, typename Operator>
class IncludeMask
:
	public includeMask
{
protected:
		
	Operator 				operator_;
	
	pointField_H<T> 		field_;

public:

	TypeNameTemplate2("IncludeMask", T, Operator);

	IncludeMask(
		const dictionary& dict,
	 	const word& opType,
	 	readFromTimeFolder& timeFolder)
	:
		includeMask(dict, opType, timeFolder),
		operator_(dict),
		field_(timeFolder.readPointField_H<T>(this->fieldName()))
	{}

	add_vCtor(
		includeMask,
		IncludeMask,
		dictionary);

	bool isIncluded(int32 n)const override
	{
		return operator_(field_[n]);
	}

};


template<typename T>
class IncludeMask<T,allOp<T>>
:
	public includeMask
{
public:
	TypeNameTemplate2("IncludeMask", T, allOp<int8>);

	IncludeMask(
		const dictionary& dict,
	 	const word& opType,
	 	readFromTimeFolder& timeFolder)
	:
		includeMask(dict, opType, timeFolder)
	{}

	add_vCtor(
		includeMask,
		IncludeMask,
		dictionary);
	
	bool isIncluded(int32 n)const override
	{
		return true;
	}
};


} // pFlow

#endif //__IncludeMask_H__


