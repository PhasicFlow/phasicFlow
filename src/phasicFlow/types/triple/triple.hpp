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
#include "uniquePtr.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"
#include "error.hpp"



namespace pFlow
{


template<typename T> class triple;


#include "tripleFwd.hpp"


// for 3D vectors
// it should be used only for numeric types, real, unit 
template <typename T>
struct triple
{
	// data members
	T x_;
	T y_;
	T z_;

	// initilizes to zero
	INLINE_FUNCTION_HD triple():
	x_(0),
	y_(0),
	z_(0)
	{}

	// Constructors
	INLINE_FUNCTION_HD triple(const T &x, const T &y, const T &z):
	x_(x),
	y_(y),
	z_(z)
	{}

	INLINE_FUNCTION_HD triple(const T &v):
	triple(v, v, v)
	{}

	// type conversion trough assignment  
	template <typename T2>
	INLINE_FUNCTION_HD triple<T> & operator = (const triple<T2> & rhs)
	{
		this->x_ = static_cast<T>(rhs.x_);
		this->y_ = static_cast<T>(rhs.y_);
		this->z_ = static_cast<T>(rhs.z_);
		return *this;
	}

	// type casting through copy constructor 
	template<typename T2>
	INLINE_FUNCTION_HD triple(const triple<T2> &src):
	x_(static_cast<T>(src.x_)),
	y_(static_cast<T>(src.y_)),
	z_(static_cast<T>(src.z_))
	{
	}
	
	// copy construct
	INLINE_FUNCTION_HD
	triple(const triple<T>& src) = default;

	// volatile copy construct
	/*INLINE_FUNCTION_HD
	triple(volatile triple<T>& src):
		x_(src.x_),
		y_(src.y_),
		z_(src.z_)
	{}*/

	/*INLINE_FUNCTION_HD
	triple& operator=(volatile triple<T>& src)	
	{
		x_ = src.x_;
		y_ = src.y_;
		z_ = src.z_;
	}*/

	// move construct
	INLINE_FUNCTION_HD
	triple(triple<T>&& src) = default;

	// copy assignment
	INLINE_FUNCTION_HD
	triple<T>& operator=(const triple<T>& src) = default;

	// move assignment
	INLINE_FUNCTION_HD
	triple<T>& operator=(triple<T>&& src) = default;

	// clone 
	INLINE_FUNCTION 
	uniquePtr<triple<T>> clone() const
	{
		return makeUnique<triple<T>>(*this);
	}

	INLINE_FUNCTION
	triple<T>* clonePtr()const
	{
		return new triple<T>(*this);
	}

	//// member methods

	// access
	INLINE_FUNCTION_HD T & x(){ return x_; }
	INLINE_FUNCTION_HD const T & x()const { return x_; }
	
	INLINE_FUNCTION_HD T & y(){ return y_; }
	INLINE_FUNCTION_HD const T & y()const { return y_; }

	INLINE_FUNCTION_HD T & z(){ return z_; }
	INLINE_FUNCTION_HD const T & z()const { return z_; }

	// methods
	friend FUNCTION_HD T dot <T> (const triple<T> & oprnd1, const triple<T> & oprnd2);
	
	friend FUNCTION_HD triple<T> cross <T>(const triple<T> & v1, const triple<T> & v2);
	
	INLINE_FUNCTION_HD T length() const; 

	INLINE_FUNCTION_HD void normalize(); 



	//// operators 
	
	// + operator
	friend FUNCTION_HD triple<T> operator+ <T> (const triple<T> & oprnd1, const triple<T> & oprnd2);

	friend FUNCTION_HD triple<T> operator+ <T> (const triple<T> & oprnd1, const T & oprnd2);
	
	friend FUNCTION_HD triple<T> operator+ <T> (const T & oprnd1, const triple<T> & oprnd2);	
	
	// - operator 
	friend FUNCTION_HD triple<T> operator - <T> (const triple<T> & oprnd1, const triple<T> & oprnd2);
	
	friend FUNCTION_HD triple<T> operator - <T> (const triple<T> & oprnd1, const T & oprnd2);
	
	friend FUNCTION_HD triple<T> operator - <T> (const T & oprnd1, const triple<T> & oprnd2);
	
	// * operators
	friend FUNCTION_HD triple<T> operator * <T> (const triple<T> & oprnd1, const triple<T> & oprnd2);
	
	friend FUNCTION_HD triple<T> operator * <T> (const triple<T> & oprnd1, const T & oprnd2);
	
	friend FUNCTION_HD triple<T> operator * <T> (const T & oprnd1, const triple<T> & oprnd2);

	// / operators
	friend FUNCTION_HD triple<T> operator / <T> (const triple<T> & oprnd1, const triple<T> & oprnd2);
	
	friend FUNCTION_HD triple<T> operator / <T> (const triple<T> & oprnd1, const T & oprnd2);
	
	friend FUNCTION_HD triple<T> operator / <T> (const T & oprnd1, const triple<T> & oprnd2);


	INLINE_FUNCTION_HD void operator+= (const triple & oprnd2);

	INLINE_FUNCTION_HD void operator-= (const triple & oprnd2);

	INLINE_FUNCTION_HD void operator*= (const triple & oprnd2);

	INLINE_FUNCTION_HD void operator/= (const triple & oprnd2);
	
	
	// unary negate operator 
	INLINE_FUNCTION_HD triple operator- ()const;
	
	// unary plus operator 
	INLINE_FUNCTION_HD triple operator+ ()const;
	

	friend FUNCTION_HD bool operator == <T> (const triple<T> &opr1, const triple<T> &opr2);

	friend FUNCTION_HD bool operator < <T> (const triple<T> &opr1, const triple<T> &opr2);

	friend FUNCTION_HD bool operator > <T> (const triple<T> &opr1, const triple<T> &opr2);

	friend FUNCTION_HD bool operator >= <T> (const triple<T> &opr1, const triple<T> &opr2);

	friend FUNCTION_HD bool operator <= <T> (const triple<T> &opr1, const triple<T> &opr2);
	
	// << operator 	
	friend iOstream& operator<< <T> (iOstream& str, const triple<T> & ov);
	
	// >> operator 
	friend iIstream& operator >> <T> (iIstream & str, triple<T> & iv);

	// same as >> operator, but faster, good for mass read
	friend void readIstream <T>( iIstream& str, triple<T> &iv);

};

template<typename T>
bool INLINE_FUNCTION_HD equal( const triple<T>& opr1, const triple<T>& opr2 );


} // end of pFlow

#include "tripleI.hpp"
#include "tripleMath.hpp"


#endif
