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

#ifndef __quadruple_hpp__
#define __quadruple_hpp__

#include "error.hpp"
#include "typeInfo.hpp"
#include "iIstream.hpp"
#include "iOstream.hpp"
#include "token.hpp"
#include "triple.hpp"
#include "uniquePtr.hpp"

namespace pFlow
{

template<typename T>
class quadruple;
class iIstream;

#include "quadrupleFwd.hpp"

// you can see it as a sequence of four elements (w,x,y,z) or an (scalar and
// vector)
template<typename T>
struct quadruple
{
	T         s_;
	triple<T> v_;


	QuadrupleTypeInfoNV(T);

	//// constructors
	INLINE_FUNCTION_HD
	quadruple()
	{
	}

	//// Constructors

	INLINE_FUNCTION_HD
	quadruple(const T& w, const T& x, const T& y, const T& z)
	  : s_(w),
	    v_(x, y, z)
	{
	}

	INLINE_FUNCTION_HD
	quadruple(const T& s, const triple<T> v)
	  : s_(s),
	    v_(v)
	{
	}

	INLINE_FUNCTION_HD
	quadruple(const T& val)
	  : s_(val),
	    v_(val)
	{
	}

	// type conversion trough assignment
	template<typename T2>
	INLINE_FUNCTION_HD quadruple<T>& operator=(const quadruple<T2>& rhs)
	{
		this->v_ = rhs.v_;
		this->s_ = static_cast<T>(rhs.s_);
		return *this;
	}

	// type casting through copy constructor
	template<typename T2>
	INLINE_FUNCTION_HD quadruple(const quadruple<T2>& src)
	  : s_(static_cast<T>(src.s_)),
	    v_(src.v_)
	{
	}

	// copy construct
	INLINE_FUNCTION_HD
	quadruple(const quadruple<T>& src) = default;

	// move construct
	INLINE_FUNCTION_HD
	quadruple(quadruple<T>&& src) = default;

	// copy assignment
	INLINE_FUNCTION_HD
	quadruple<T>& operator=(const quadruple<T>& src) = default;

	// move assignment
	INLINE_FUNCTION_HD
	quadruple<T>& operator=(quadruple<T>&& src) = default;

	// clone
	INLINE_FUNCTION_H
	uniquePtr<quadruple<T>> clone() const
	{
		return makeUnique<quadruple<T>>(*this);
	}

	INLINE_FUNCTION_H
	quadruple<T>* clonePtr() const
	{
		return new quadruple<T>(*this);
	}

	// Access
	INLINE_FUNCTION_HD
	T& w()
	{
		return s_;
	}
	INLINE_FUNCTION_HD
	const T& w() const
	{
		return s_;
	}

	INLINE_FUNCTION_HD
	T& x()
	{
		return v_.x();
	}
	INLINE_FUNCTION_HD
	const T& x() const
	{
		return v_.x();
	}

	INLINE_FUNCTION_HD
	T& y()
	{
		return v_.y();
	}
	INLINE_FUNCTION_HD
	const T& y() const
	{
		return v_.y();
	}

	INLINE_FUNCTION_HD
	T& z()
	{
		return v_.z();
	}
	INLINE_FUNCTION_HD
	const T& z() const
	{
		return v_.z();
	}

	INLINE_FUNCTION_HD
	T& s()
	{
		return s_;
	}
	INLINE_FUNCTION_HD
	const T& s() const
	{
		return s_;
	}

	INLINE_FUNCTION_HD
	triple<T> v()
	{
		return v_;
	}
	INLINE_FUNCTION_HD
	const triple<T> v() const
	{
		return v_;
	}

	// methods
	friend FUNCTION_HD T
	dot<T>(const quadruple<T>& oprnd1, const quadruple<T>& oprnd2);

	INLINE_FUNCTION_HD T    length() const;

	INLINE_FUNCTION_HD void normalize();

	//// operators

	// + operator
	friend FUNCTION_HD      quadruple<T> operator+
	  <T>(const quadruple<T>& oprnd1, const quadruple<T>& oprnd2);

	friend FUNCTION_HD quadruple<T> operator+
	  <T>(const quadruple<T>& oprnd1, const T& oprnd2);

	friend FUNCTION_HD quadruple<T> operator+
	  <T>(const T& oprnd1, const quadruple<T>& oprnd2);

	// - operator
	friend FUNCTION_HD quadruple<T> operator-
	  <T>(const quadruple<T>& oprnd1, const quadruple<T>& oprnd2);

	friend FUNCTION_HD quadruple<T> operator-
	  <T>(const quadruple<T>& oprnd1, const T& oprnd2);

	friend FUNCTION_HD quadruple<T> operator-
	  <T>(const T& oprnd1, const quadruple<T>& oprnd2);

	// * operators
	friend FUNCTION_HD quadruple<T> operator*
	  <T>(const quadruple<T>& oprnd1, const quadruple<T>& oprnd2);

	friend FUNCTION_HD quadruple<T> operator*
	  <T>(const quadruple<T>& oprnd1, const T& oprnd2);

	friend FUNCTION_HD quadruple<T> operator*
	  <T>(const T& oprnd1, const quadruple<T>& oprnd2);

	// / operators
	friend FUNCTION_HD quadruple<T> operator/
	  <T>(const quadruple<T>& oprnd1, const quadruple<T>& oprnd2);

	friend FUNCTION_HD quadruple<T> operator/
	  <T>(const quadruple<T>& oprnd1, const T& oprnd2);

	friend FUNCTION_HD quadruple<T> operator/
	  <T>(const T& oprnd1, const quadruple<T>& oprnd2);

	INLINE_FUNCTION_HD
	void operator+=(const quadruple& oprnd2);

	INLINE_FUNCTION_HD
	void operator-=(const quadruple& oprnd2);

	INLINE_FUNCTION_HD
	void operator*=(const quadruple& oprnd2);

	INLINE_FUNCTION_HD
	void operator/=(const quadruple& oprnd2);

	// unary negate operator
	INLINE_FUNCTION_HD
	quadruple operator-() const;

	// unary plus operator
	INLINE_FUNCTION_HD
	quadruple               operator+() const;

	friend FUNCTION_HD bool operator==
	  <T>(const quadruple<T>& opr1, const quadruple<T>& opr2);

	// << operator
	friend FUNCTION_H iOstream&
	operator<< <T>(iOstream& str, const quadruple<T>& ov);

	// >> operator
	friend FUNCTION_H iIstream& operator>> <T>(iIstream& str, quadruple<T>& iv);

	// same as >> operator, but faster, good for mass read
	friend FUNCTION_H void readIstream<T>(iIstream& str, quadruple<T>& iv);
};

} // pFlow

#include "quadrupleI.hpp"
#include "quadrupleMath.hpp"

#endif
