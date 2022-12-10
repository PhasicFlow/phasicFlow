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
INLINE_FUNCTION_HD T dot
(
	const quadruple<T> & oprnd1,
	const quadruple<T> & oprnd2
);


template<typename T> 
INLINE_FUNCTION_HD quadruple<T> operator + 
(
	const quadruple<T> & oprnd1,
	const quadruple<T> & oprnd2
);

template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator+
(
	const quadruple<T> & oprnd1,
	const T & oprnd2
);


template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator+
(
	const T & oprnd2,
	const quadruple<T> & oprnd1
); 

template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator-
(
	const quadruple<T> & oprnd1,
	const quadruple<T> & oprnd2
);
	
template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator-
(
	const quadruple<T> & oprnd1,
	const T & oprnd2
);
	
template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator-
(
	const T & oprnd1,
	const quadruple<T> & oprnd2
);

template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator*
(
	const quadruple<T> & oprnd1,
	const quadruple<T> & oprnd2
);
	
template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator*
(
	const quadruple<T> & oprnd1,
	const T & oprnd2
);
	
template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator*
(
	const T & oprnd1,
	const quadruple<T> & oprnd2
);

template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator/
(
	const quadruple<T> & oprnd1,
	const quadruple<T> & oprnd2
);
	
template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator/
(
	const quadruple<T> & oprnd1,
	const T & oprnd2
);
	
template<typename T>
INLINE_FUNCTION_HD quadruple<T> operator/
(
	const T & oprnd1,
	const quadruple<T> & oprnd2
);

template<typename T>
INLINE_FUNCTION_HD bool operator ==
(
	const quadruple<T> &opr1,
	const quadruple<T> &opr2
);

template<typename T> 
INLINE_FUNCTION_H iOstream& operator<<
(
	iOstream& str,
	const quadruple<T> & ov
);


template<typename T> 
INLINE_FUNCTION_H iIstream& operator>>
(
	iIstream& str,
	quadruple<T> & iv
);

template<typename T>
INLINE_FUNCTION_H void readIstream 
(
	iIstream& str,
	quadruple<T> & iv
);