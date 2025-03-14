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
dot(const triple<T>& oprnd1, const triple<T>& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   cross(const triple<T>& v1, const triple<T>& v2);

template<typename T>
INLINE_FUNCTION_HD T
length(const triple<T>& v1);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   normalize(const triple<T>& v1);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator+(const triple<T>& oprnd1, const triple<T>& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator+(const triple<T>& oprnd1, const T& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator+(const T& oprnd2, const triple<T>& oprnd1);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator-(const triple<T>& oprnd1, const triple<T>& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator-(const triple<T>& oprnd1, const T& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator-(const T& oprnd1, const triple<T>& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator*(const triple<T>& oprnd1, const triple<T>& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator*(const triple<T>& oprnd1, const T& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator*(const T& oprnd1, const triple<T>& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator/(const triple<T>& oprnd1, const triple<T>& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator/(const triple<T>& oprnd1, const T& oprnd2);

template<typename T>
INLINE_FUNCTION_HD triple<T>
                   operator/(const T& oprnd1, const triple<T>& oprnd2);

template<typename T>
INLINE_FUNCTION_HD bool
operator==(const triple<T>& opr1, const triple<T>& opr2);

template<typename T>
INLINE_FUNCTION_HD bool
operator>(const triple<T>& opr1, const triple<T>& opr2);

template<typename T>
INLINE_FUNCTION_HD bool
operator<(const triple<T>& opr1, const triple<T>& opr2);

template<typename T>
INLINE_FUNCTION_HD bool
operator<=(const triple<T>& opr1, const triple<T>& opr2);

template<typename T>
INLINE_FUNCTION_HD bool
operator>=(const triple<T>& opr1, const triple<T>& opr2);

template<typename T>
inline iOstream&
operator<<(iOstream& str, const triple<T>& ov);

template<typename T>
inline iIstream&
operator>>(iIstream& str, triple<T>& iv);

template<typename T>
inline void
readIstream(iIstream& str, triple<T>& iv);
