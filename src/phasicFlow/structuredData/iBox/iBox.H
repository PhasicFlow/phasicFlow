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

#ifndef __iBox_H__
#define __iBox_H__

#include "types.H"
#include "dictionary.H"
#include "iIstream.H"
#include "iOstream.H"

namespace pFlow
{

template<typename intType>
class iBox
{
protected:
	
	// - min point
	triple<intType> 	min_;
	
	// - max point 
	triple<intType> 	max_;

public:

	// - type info
	TypeNameTemplateNV("iBox", intType);

	//// - Constructors 
		INLINE_FUNCTION_HD 
		iBox();

		INLINE_FUNCTION_HD 
		iBox(const triple<intType> & minP, const triple<intType> & maxP);

		FUNCTION_H
		iBox(const dictionary& dict);

		FUNCTION_H
		iBox(iIstream& is);

		FUNCTION_HD
		iBox(const iBox&) = default;

		FUNCTION_HD
		iBox(iBox&&) = default;

		FUNCTION_HD
		iBox& operator=(const iBox&) = default;

		FUNCTION_HD
		iBox& operator=(iBox&&) = default;

		~iBox()=default;

	//// - Methods 

		INLINE_FUNCTION_HD
		bool isInside(const triple<intType> & point)const;

		INLINE_FUNCTION_HD
		const triple<intType> & minPoint()const
		{
			return min_;
		}

		INLINE_FUNCTION_HD
		const triple<intType> & maxPoint()const
		{
			return max_;
		}

	//// - IO operation 
		FUNCTION_H
		bool read(iIstream & is);

		FUNCTION_H
		bool write(iOstream& os)const;

		FUNCTION_H
		bool read(const dictionary& dict);

		FUNCTION_H
		bool write(dictionary& dict)const;
};

template<typename intType>
FUNCTION_H
iIstream& operator >> (iIstream& is, iBox<intType>& b);

template<typename intType>
FUNCTION_H
iOstream& operator << (iOstream& os, const iBox<intType>& b);



}


#include "iBox.C"


#endif
