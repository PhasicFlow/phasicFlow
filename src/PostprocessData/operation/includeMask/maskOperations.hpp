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

#ifndef __maskOperation_hpp__
#define __maskOperation_hpp__

#include "types.hpp"
#include "dictionary.hpp"

namespace pFlow
{

template<typename T>
struct greaterThanOp
{
	TypeInfoNV("greaterThan");

 	inline
 	bool operator()(const T &compVal, const T &val) const {
   		return val > compVal; }
};

template<typename T>
struct greaterThanEqOp
{
	TypeInfoNV("greaterThanEq");

 	inline
 	bool operator()(const T &compVal, const T &val) const {
   		return val >= compVal; }
};

template<typename T>
struct lessThanOp
{
	TypeInfoNV("lessThan");

	inline
	bool operator()(const T &compVal, const T &val) const {
		return val < compVal; }
};

template<typename T>
struct lessThanEqOp
{
	TypeInfoNV("lessThanEq");

	inline
	bool operator()(const T &compVal, const T &val) const {
		return val <= compVal; }
};

template<typename T>
struct equalOp
{
	TypeInfoNV("equal");

	inline
	bool operator()(const T &compVal, const T &val) const {
		return equal(val , compVal); }
};


template<typename T>
struct betweenOp
{
	TypeInfoNV("between");

	inline
	bool operator()(const T &compVal1, const T &compVal2 ,const T &val) const {
		return val>compVal1 && val<compVal2; }
};


template<typename T>
struct betweenEqOp
{
	TypeInfoNV("betweenEq");

	inline
	bool operator()(const T &compVal1, const T &compVal2 ,const T &val) const {
		return val>=compVal1 && val<=compVal2; }
};

template<typename T>
struct allOp
{
	TypeInfoNV("all");

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

	TypeInfoNV(Operator<T>::TYPENAME());

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
	
	TypeInfoNV(opertorType::TYPENAME());

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

	TypeInfoNV(Operator::TYPENAME());
	compareZero(const dictionary& dict);

	bool operator()(const T& value) const
	{
		return operator_();
	}
};

}

#endif //__maskOperation_hpp__