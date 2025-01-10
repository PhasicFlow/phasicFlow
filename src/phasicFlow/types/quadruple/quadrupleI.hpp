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

template<typename T>
INLINE_FUNCTION_HD T
pFlow::dot(const quadruple<T>& oprnd1, const quadruple<T>& oprnd2)
{
	return oprnd1.s_ * oprnd2.s_ + dot(oprnd1.v(), oprnd2.v());
}

template<typename T>
INLINE_FUNCTION_HD T
pFlow::quadruple<T>::length() const
{
	return sqrt(dot(*this, *this));
}

template<typename T>
INLINE_FUNCTION_HD void
pFlow::quadruple<T>::normalize()
{
	T l = length();
	if (static_cast<real>(l) > smallValue)
	{
		*this /= l;
	}
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
pFlow::operator+(const quadruple<T>& oprnd1, const quadruple<T>& oprnd2)
{
	return quadruple<T>(oprnd1.s_ + oprnd2.s_, oprnd1.v_ + oprnd2.v_);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
                   pFlow::operator+(const quadruple<T>& oprnd1, const T& oprnd2)
{
	return quadruple<T>(oprnd1.s_ + oprnd2, oprnd1.v_ + oprnd2);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
                   pFlow::operator+(const T& oprnd1, const quadruple<T>& oprnd2)
{
	return quadruple<T>(oprnd1 + oprnd2.s_, oprnd1 + oprnd2.v_);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
pFlow::operator-(const quadruple<T>& oprnd1, const quadruple<T>& oprnd2)
{
	return quadruple<T>(oprnd1.s_ - oprnd2.s_, oprnd1.v_ - oprnd2.v_);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
                   pFlow::operator-(const quadruple<T>& oprnd1, const T& oprnd2)
{
	return quadruple<T>(oprnd1.s_ - oprnd2, oprnd1.v_ - oprnd2);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
                   pFlow::operator-(const T& oprnd1, const quadruple<T>& oprnd2)
{
	return quadruple<T>(oprnd1 - oprnd2.s_, oprnd1 - oprnd2.v_);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
pFlow::operator*(const quadruple<T>& oprnd1, const quadruple<T>& oprnd2)
{
	return quadruple<T>(oprnd1.s_ * oprnd2.s_, oprnd1.v_ * oprnd2.v_);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
                   pFlow::operator*(const quadruple<T>& oprnd1, const T& oprnd2)
{
	return quadruple<T>(oprnd1.s_ * oprnd2, oprnd1.v_ * oprnd2);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
                   pFlow::operator*(const T& oprnd1, const quadruple<T>& oprnd2)
{
	return quadruple<T>(oprnd1 * oprnd2.s_, oprnd1 * oprnd2.v_);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
pFlow::operator/(const quadruple<T>& oprnd1, const quadruple<T>& oprnd2)
{
	return quadruple<T>(oprnd1.s_ / oprnd2.s_, oprnd1.v_ / oprnd2.v_);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
                   pFlow::operator/(const quadruple<T>& oprnd1, const T& oprnd2)
{
	return quadruple<T>(oprnd1.s_ / oprnd2, oprnd1.v_ / oprnd2);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
                   pFlow::operator/(const T& oprnd1, const quadruple<T>& oprnd2)
{
	return quadruple<T>(oprnd1 / oprnd2.s_, oprnd1 / oprnd2.v_);
}

template<typename T>
INLINE_FUNCTION_HD void
pFlow::quadruple<T>::operator+=(const quadruple<T>& oprnd2)
{
	this->s_ += oprnd2.s_;
	this->v_ += oprnd2.v_;
}

template<typename T>
INLINE_FUNCTION_HD void
pFlow::quadruple<T>::operator-=(const quadruple<T>& oprnd2)
{
	this->s_ -= oprnd2.s_;
	this->v_ -= oprnd2.v_;
}

template<typename T>
INLINE_FUNCTION_HD void
pFlow::quadruple<T>::operator*=(const quadruple<T>& oprnd2)
{
	this->s_ *= oprnd2.s_;
	this->v_ *= oprnd2.v_;
}

template<typename T>
INLINE_FUNCTION_HD void
pFlow::quadruple<T>::operator/=(const quadruple<T>& oprnd2)
{
	this->s_ /= oprnd2.s_;
	this->v_ /= oprnd2.v_;
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
                   pFlow::quadruple<T>::operator-() const
{
	return quadruple<T>(-this->s_, -this->v_);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::quadruple<T>
                   pFlow::quadruple<T>::operator+() const
{
	return *this;
}

template<typename T>
INLINE_FUNCTION_HD bool
pFlow::operator==(const quadruple<T>& opr1, const quadruple<T>& opr2)
{
	return equal(opr1.s_, opr2.s_) && equal(opr1.v_, opr2.v_);
};

template<typename T>
INLINE_FUNCTION_H pFlow::iOstream&
                  pFlow::operator<<(iOstream& str, const quadruple<T>& ov)
{
	str << token::BEGIN_LIST << ov.w() << token::SPACE << ov.x() << token::SPACE
	    << ov.y() << token::SPACE << ov.z() << token::END_LIST;

	str.check(FUNCTION_NAME);

	return str;
}

template<typename T>
INLINE_FUNCTION_H pFlow::iIstream&
                  pFlow::operator>>(iIstream& str, quadruple<T>& iv)
{
	str.readBegin("quadruple<T>");

	str >> iv.w();
	str >> iv.x();
	str >> iv.y();
	str >> iv.z();

	str.readEnd("quadruple<T>");

	str.check(FUNCTION_NAME);

	return str;
}

template<typename T>
INLINE_FUNCTION_H void
pFlow::readIstream(iIstream& str, quadruple<T>& iv)
{
	str.readBegin("quadruple<T>");
	T val;

	readIstream(str, val);
	iv.w() = val;

	readIstream(str, val);
	iv.x() = val;

	readIstream(str, val);
	iv.y() = val;

	readIstream(str, val);
	iv.z() = val;

	str.readEnd("quadruple<T>");

	str.check(FUNCTION_NAME);
}
