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


template<typename intType>
INLINE_FUNCTION_HD
pFlow::iBox<intType>::iBox()
:
	min_(0),
	max_(1)
{}


template<typename intType>
INLINE_FUNCTION_HD
pFlow::iBox<intType>::iBox(
	const triple<intType>& minP,
	const triple<intType>& maxP)
:
	min_(minP),
	max_(maxP)
{}

template<typename intType>
FUNCTION_H
pFlow::iBox<intType>::iBox
(
	const dictionary & dict
)
:
	min_
	(
		dict.getVal<triple<intType>>("min")
	),
	max_
	(
		dict.getVal<triple<intType>>("max")
	)
{}

template<typename intType>
FUNCTION_H
pFlow::iBox<intType>::iBox
(
	iIstream& is
)
{
	if( !read(is))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"error in reading iBox from file. \n";
		fatalExit;
	}
}

template<typename intType>
INLINE_FUNCTION_HD 
bool pFlow::iBox<intType>::isInside
(
	const triple<intType>& point
)const
{
	return point >= min_ && point <= max_;
}

template<typename intType>
FUNCTION_H
bool pFlow::iBox<intType>::read(iIstream & is)
{
	if(!is.nextData<triple<intType>>("min", min_)) return false;
	if(!is.nextData<triple<intType>>("max", max_)) return false;
	return true;
}

template<typename intType>
FUNCTION_H
bool pFlow::iBox<intType>::write(iOstream& os)const
{
	os.writeWordEntry("min", min_);
	os.writeWordEntry("max", max_);
	return os.check(FUNCTION_NAME);
}

template<typename intType>
FUNCTION_H
bool pFlow::iBox<intType>::read
(
	const dictionary& dict
)
{
	min_ = dict.getVal<triple<intType>>("min");
	max_ = dict.getVal<triple<intType>>("max");
	return true;
}

template<typename intType>
FUNCTION_H
bool pFlow::iBox<intType>::write
(
	dictionary& dict
)const
{
	if(!dict.add("min", min_))
	{
		fatalErrorInFunction<<
		"  error in writing min to dictionary "<<dict.globalName()<<endl;
		return false;
	}

	if(!dict.add("max", max_))
	{
		fatalErrorInFunction<<
		"  error in writing max to dictionary "<<dict.globalName()<<endl;
		return false;
	}
	
	return true;
}

template<typename intType>
FUNCTION_H
pFlow::iIstream& pFlow::operator >> (iIstream& is, iBox<intType>& b)
{
	if(! b.read(is))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"error in reading iBox. \n";
		fatalExit;
	}
	return is;
}

template<typename intType>
FUNCTION_H
pFlow::iOstream& pFlow::operator << (iOstream& os, const iBox<intType>& b)
{
	
	if(! b.write(os))
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
		"error in writing iBox. \n";
		fatalExit;
	}
	return os;	
}