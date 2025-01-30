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
#ifndef __triple_hpp__
#define __triple_hpp__

#include "pFlowMacros.hpp"
#include "numericConstants.hpp"
#include "typeInfo.hpp"
#include "uniquePtr.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"
#include "error.hpp"

namespace pFlow
{

/// - Forward
template<typename T>
class triple;

#include "tripleFwd.hpp"

/**
 * A set of 3 variables that can be used for vector variables.
 * The template parameter should be numeric type only.
 *
 */
template<typename T>
struct triple
{
	/// data members
	T x_;
	T y_;
	T z_;

	/// Type info for triple
	TripleTypeInfoNV(T);
	
	//// Constructors

	/// Initilize to zero
	INLINE_FUNCTION_HD
	triple() = default;
	
	INLINE_FUNCTION_HD
	~triple() = default;

	/// Construct from x, y, z
	INLINE_FUNCTION_HD
	triple(const T& x, const T& y, const T& z)
	  : x_(x),
	    y_(y),
	    z_(z)
	{
	}

	/// Construct from v
	INLINE_FUNCTION_HD
	triple(const T& v)
	  : triple(v, v, v)
	{
	}

	/// Type conversion trough assignment
	template<typename T2>
	INLINE_FUNCTION_HD triple<T>& operator=(const triple<T2>& rhs)
	{
		this->x_ = static_cast<T>(rhs.x_);
		this->y_ = static_cast<T>(rhs.y_);
		this->z_ = static_cast<T>(rhs.z_);
		return *this;
	}

	/// Type casting through copy constructor
	template<typename T2>
	INLINE_FUNCTION_HD triple(const triple<T2>& src)
	  : x_(static_cast<T>(src.x_)),
	    y_(static_cast<T>(src.y_)),
	    z_(static_cast<T>(src.z_))
	{
	}

	/// Copy construct
	INLINE_FUNCTION_HD
	triple(const triple<T>& src) = default;

	/// Move construct
	INLINE_FUNCTION_HD
	triple(triple<T>&& src) = default;

	/// copy assignment
	INLINE_FUNCTION_HD
	triple<T>& operator=(const triple<T>& src) = default;

	/// move assignment
	INLINE_FUNCTION_HD
	triple<T>& operator=(triple<T>&& src) = default;

	/// clone
	inline
	uniquePtr<triple<T>> clone() const
	{
		return makeUnique<triple<T>>(*this);
	}

	inline
	triple<T>* clonePtr() const
	{
		return new triple<T>(*this);
	}

	////// member methods

	/// access component
	INLINE_FUNCTION_HD T& x()
	{
		return x_;
	}

	/// access component
	INLINE_FUNCTION_HD const T& x() const
	{
		return x_;
	}

	/// access component
	INLINE_FUNCTION_HD T& y()
	{
		return y_;
	}

	/// access component
	INLINE_FUNCTION_HD const T& y() const
	{
		return y_;
	}

	/// access component
	INLINE_FUNCTION_HD T& z()
	{
		return z_;
	}

	/// access component
	INLINE_FUNCTION_HD const T& z() const
	{
		return z_;
	}

	/// access component
	INLINE_FUNCTION_HD T& comp1()
	{
		return x_;
	}

	/// access component
	INLINE_FUNCTION_HD const T& comp1() const
	{
		return x_;
	}

	/// access component
	INLINE_FUNCTION_HD T& comp2()
	{
		return y_;
	}

	/// access component
	INLINE_FUNCTION_HD const T& comp2() const
	{
		return y_;
	}

	/// access component
	INLINE_FUNCTION_HD T& comp3()
	{
		return z_;
	}

	/// access component
	INLINE_FUNCTION_HD const T& comp3() const
	{
		return z_;
	}

	//// methods

	/// Dot product of two vectors
	friend FUNCTION_HD T
	                   dot<T>(const triple<T>& oprnd1, const triple<T>& oprnd2);

	/// Cross product of two vectors
	friend FUNCTION_HD triple<T>
	                   cross<T>(const triple<T>& v1, const triple<T>& v2);

	/// Length of the vector
	INLINE_FUNCTION_HD T    length() const;

	/// Normalize the vector
	INLINE_FUNCTION_HD void normalize();

	//// operators

	/// + operator
	friend FUNCTION_HD      triple<T> operator+
	  <T>(const triple<T>& oprnd1, const triple<T>& oprnd2);

	friend FUNCTION_HD triple<T> operator+
	  <T>(const triple<T>& oprnd1, const T& oprnd2);

	friend FUNCTION_HD triple<T> operator+
	  <T>(const T& oprnd1, const triple<T>& oprnd2);

	/// - operator
	friend FUNCTION_HD triple<T> operator-
	  <T>(const triple<T>& oprnd1, const triple<T>& oprnd2);

	friend FUNCTION_HD triple<T> operator-
	  <T>(const triple<T>& oprnd1, const T& oprnd2);

	friend FUNCTION_HD triple<T> operator-
	  <T>(const T& oprnd1, const triple<T>& oprnd2);

	/// * operators
	friend FUNCTION_HD triple<T> operator*
	  <T>(const triple<T>& oprnd1, const triple<T>& oprnd2);

	friend FUNCTION_HD triple<T> operator*
	  <T>(const triple<T>& oprnd1, const T& oprnd2);

	friend FUNCTION_HD triple<T> operator*
	  <T>(const T& oprnd1, const triple<T>& oprnd2);

	/// / operators
	friend FUNCTION_HD triple<T> operator/
	  <T>(const triple<T>& oprnd1, const triple<T>& oprnd2);

	friend FUNCTION_HD triple<T> operator/
	  <T>(const triple<T>& oprnd1, const T& oprnd2);

	friend FUNCTION_HD triple<T> operator/
	  <T>(const T& oprnd1, const triple<T>& oprnd2);

	INLINE_FUNCTION_HD void   operator+=(const triple& oprnd2);

	INLINE_FUNCTION_HD void   operator-=(const triple& oprnd2);

	INLINE_FUNCTION_HD void   operator*=(const triple& oprnd2);

	INLINE_FUNCTION_HD void   operator/=(const triple& oprnd2);

	/// unary negate operator
	INLINE_FUNCTION_HD triple operator-() const;

	/// unary plus operator
	INLINE_FUNCTION_HD triple operator+() const;

	friend FUNCTION_HD bool   operator==
	  <T>(const triple<T>& opr1, const triple<T>& opr2);

	friend FUNCTION_HD bool
	operator< <T>(const triple<T>& opr1, const triple<T>& opr2);

	friend FUNCTION_HD bool operator>
	  <T>(const triple<T>& opr1, const triple<T>& opr2);

	friend FUNCTION_HD bool operator>=
	  <T>(const triple<T>& opr1, const triple<T>& opr2);

	friend FUNCTION_HD bool operator<=
	  <T>(const triple<T>& opr1, const triple<T>& opr2);

	//// IO operators

	/// << operator
	friend iOstream& operator<< <T>(iOstream& str, const triple<T>& ov);

	/// >> operator
	friend iIstream& operator>> <T>(iIstream& str, triple<T>& iv);

	/// same as >> operator, but faster, good for mass read
	friend void readIstream<T>(iIstream& str, triple<T>& iv);
};

template<typename T>
bool INLINE_FUNCTION_HD
equal(const triple<T>& opr1, const triple<T>& opr2);

bool INLINE_FUNCTION_HD
equal(const triple<real>& opr1, const triple<real>& opr2, real tol)
{
	return equal(opr1.x(), opr2.x(), tol) && equal(opr1.y(), opr2.y(), tol) &&
	       equal(opr1.z(), opr2.z(), tol);
}

} /// end of pFlow

#include "tripleI.hpp"
#include "tripleMath.hpp"

#endif
