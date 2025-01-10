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
pFlow::dot(const triple<T>& oprnd1, const triple<T>& oprnd2)
{
	return oprnd1.x_ * oprnd2.x_ + oprnd1.y_ * oprnd2.y_ +
	       oprnd1.z_ * oprnd2.z_;
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::cross(const triple<T>& v1, const triple<T>& v2)
{
	return triple<T>(
	  v1.y_ * v2.z_ - v1.z_ * v2.y_,
	  v1.z_ * v2.x_ - v1.x_ * v2.z_,
	  v1.x_ * v2.y_ - v1.y_ * v2.x_
	);
}

template<typename T>
INLINE_FUNCTION_HD T
pFlow::length(const triple<T>& v1)
{
	return v1.length();
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::normalize(const triple<T>& v1)
{
	return v1 / max(length(v1), verySmallValue);
}

template<typename T>
INLINE_FUNCTION_HD T
pFlow::triple<T>::length() const
{
	return sqrt(dot(*this, *this));
}

template<typename T>
INLINE_FUNCTION_HD void
pFlow::triple<T>::normalize()
{
	*this = *this / max(length(), verySmallValue);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
pFlow::operator+(const triple<T>& oprnd1, const triple<T>& oprnd2)
{
	return triple<T>(
	  oprnd1.x_ + oprnd2.x_, oprnd1.y_ + oprnd2.y_, oprnd1.z_ + oprnd2.z_
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::operator+(const triple<T>& oprnd1, const T& oprnd2)
{
	return triple<T>(
	  oprnd1.x_ + oprnd2, oprnd1.y_ + oprnd2, oprnd1.z_ + oprnd2
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::operator+(const T& oprnd1, const triple<T>& oprnd2)
{
	return triple<T>(
	  oprnd1 + oprnd2.x_, oprnd1 + oprnd2.y_, oprnd1 + oprnd2.z_
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
pFlow::operator-(const triple<T>& oprnd1, const triple<T>& oprnd2)
{
	return triple<T>(
	  oprnd1.x_ - oprnd2.x_, oprnd1.y_ - oprnd2.y_, oprnd1.z_ - oprnd2.z_
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::operator-(const triple<T>& oprnd1, const T& oprnd2)
{
	return triple<T>(
	  oprnd1.x_ - oprnd2, oprnd1.y_ - oprnd2, oprnd1.z_ - oprnd2
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::operator-(const T& oprnd1, const triple<T>& oprnd2)
{
	return triple<T>(
	  oprnd1 - oprnd2.x_, oprnd1 - oprnd2.y_, oprnd1 - oprnd2.z_
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
pFlow::operator*(const triple<T>& oprnd1, const triple<T>& oprnd2)
{
	return triple<T>(
	  oprnd1.x_ * oprnd2.x_, oprnd1.y_ * oprnd2.y_, oprnd1.z_ * oprnd2.z_
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::operator*(const triple<T>& oprnd1, const T& oprnd2)
{
	return triple<T>(
	  oprnd1.x_ * oprnd2, oprnd1.y_ * oprnd2, oprnd1.z_ * oprnd2
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::operator*(const T& oprnd1, const triple<T>& oprnd2)
{
	return triple<T>(
	  oprnd1 * oprnd2.x_, oprnd1 * oprnd2.y_, oprnd1 * oprnd2.z_
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
pFlow::operator/(const triple<T>& oprnd1, const triple<T>& oprnd2)
{
	return triple<T>(
	  oprnd1.x_ / oprnd2.x_, oprnd1.y_ / oprnd2.y_, oprnd1.z_ / oprnd2.z_
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::operator/(const triple<T>& oprnd1, const T& oprnd2)
{
	return triple<T>(
	  oprnd1.x_ / oprnd2, oprnd1.y_ / oprnd2, oprnd1.z_ / oprnd2
	);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::operator/(const T& oprnd1, const triple<T>& oprnd2)
{
	return triple<T>(
	  oprnd1 / oprnd2.x_, oprnd1 / oprnd2.y_, oprnd1 / oprnd2.z_
	);
}

template<typename T>
INLINE_FUNCTION_HD void
pFlow::triple<T>::operator+=(const triple<T>& oprnd2)
{
	this->x_ = this->x_ + oprnd2.x_;
	this->y_ = this->y_ + oprnd2.y_;
	this->z_ = this->z_ + oprnd2.z_;
}

template<typename T>
INLINE_FUNCTION_HD void
pFlow::triple<T>::operator-=(const triple<T>& oprnd2)
{
	this->x_ = this->x_ - oprnd2.x_;
	this->y_ = this->y_ - oprnd2.y_;
	this->z_ = this->z_ - oprnd2.z_;
}

template<typename T>
INLINE_FUNCTION_HD void
pFlow::triple<T>::operator*=(const triple<T>& oprnd2)
{
	this->x_ = this->x_ * oprnd2.x_;
	this->y_ = this->y_ * oprnd2.y_;
	this->z_ = this->z_ * oprnd2.z_;
}

template<typename T>
INLINE_FUNCTION_HD void
pFlow::triple<T>::operator/=(const triple<T>& oprnd2)
{
	this->x_ = this->x_ / oprnd2.x_;
	this->y_ = this->y_ / oprnd2.y_;
	this->z_ = this->z_ / oprnd2.z_;
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::triple<T>::operator-() const
{
	return triple<T>(-this->x_, -this->y_, -this->z_);
}

template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T>
                   pFlow::triple<T>::operator+() const
{
	return *this;
}

template<typename T>
INLINE_FUNCTION_HD bool
pFlow::operator==(const triple<T>& opr1, const triple<T>& opr2)
{
	return equal(opr1, opr2);
};

template<typename T>
INLINE_FUNCTION_HD bool
pFlow::operator<(const triple<T>& opr1, const triple<T>& opr2)
{
	if (opr1.x_ < opr2.x_ && opr1.y_ < opr2.y_ && opr1.z_ < opr2.z_)
	{
		return true;
	}
	else
	{
		return false;
	}
};

template<typename T>
INLINE_FUNCTION_HD bool
pFlow::operator>(const triple<T>& opr1, const triple<T>& opr2)
{
	if (opr1.x_ > opr2.x_ && opr1.y_ > opr2.y_ && opr1.z_ > opr2.z_)
	{
		return true;
	}
	else
	{
		return false;
	}
};

template<typename T>
INLINE_FUNCTION_HD bool
pFlow::operator<=(const triple<T>& opr1, const triple<T>& opr2)
{
	if (opr1.x_ <= opr2.x_ && opr1.y_ <= opr2.y_ && opr1.z_ <= opr2.z_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
INLINE_FUNCTION_HD bool
pFlow::operator>=(const triple<T>& opr1, const triple<T>& opr2)
{
	if (opr1.x_ >= opr2.x_ && opr1.y_ >= opr2.y_ && opr1.z_ >= opr2.z_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
INLINE_FUNCTION pFlow::iOstream&
                pFlow::operator<<(iOstream& str, const triple<T>& ov)
{
	str << token::BEGIN_LIST << ov.x_ << token::SPACE << ov.y_ << token::SPACE
	    << ov.z_ << token::END_LIST;

	str.check(FUNCTION_NAME);

	return str;
}

template<typename T>
INLINE_FUNCTION pFlow::iIstream&
                pFlow::operator>>(iIstream& str, triple<T>& iv)
{
	str.readBegin("triple<T>");

	str >> iv.x_;
	str >> iv.y_;
	str >> iv.z_;

	str.readEnd("triple<T>");

	str.check(FUNCTION_NAME);

	return str;
}

template<typename T>
INLINE_FUNCTION void
pFlow::readIstream(iIstream& str, triple<T>& iv)
{
	str.readBegin("triple<T>");
	T val;

	readIstream(str, val);
	iv.x_ = val;

	readIstream(str, val);
	iv.y_ = val;

	readIstream(str, val);
	iv.z_ = val;

	str.readEnd("triple<T>");

	str.check(FUNCTION_NAME);
}

template<typename T>
bool INLINE_FUNCTION_HD
pFlow::equal(const triple<T>& opr1, const triple<T>& opr2)
{
	return equal(opr1.x(), opr2.x()) && equal(opr1.y(), opr2.y()) &&
	       equal(opr1.z(), opr2.z());
}