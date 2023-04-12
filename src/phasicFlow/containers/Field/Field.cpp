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

template<template<class, class> class VectorField, class T, class PropType>
bool pFlow::Field<VectorField, T, PropType>::readUniform
( 
	iIstream& is,
	size_t len,
	bool readLength
)
{
	size_t flen = 0;
	if( readLength ) 
	{
		is>>flen;

		if(is.bad() || is.eof() )
		{
			ioErrorInFile( is.name(), is.lineNumber() ) <<
			"expected integer value to specify field length \n";
			return false;
		}
	}
	else
	{
		flen = len;
	}

	// create a vector with one element and read it in
	T bF;
	
	is >> bF;	
	
	VectorType::assign(flen, bF);
	
	// read end statement char ;
	is.readEndStatement("readField");

	return true;
}


template<template<class, class> class VectorField, class T, class PropType>
bool pFlow::Field<VectorField, T, PropType>::readNonUniform
(
	iIstream& is,
	size_t len
)
{
	size_t flen = 0;

	
	is >> flen;
	if(is.bad() || is.eof() )
	{
		ioErrorInFile( is.name(), is.lineNumber() ) <<
		"  expected integer value to specify field length \n";
		return false;
	}
	
	if( len!=0 && flen != len )
	{
		ioErrorInFile( is.name(), is.lineNumber() ) <<
		"  expected "<< len <<" as the field length but found "<< flen <<" \n";
		return false;	
	}

	this->clear();
	if(is.isBinary() && !std::is_same_v<T,word>)
	{
		this->resize(flen);
		is.read(reinterpret_cast<char*>(this->data()), this->size()*sizeof(T));	
		is.readEndStatement("readField");
	}
	else
	{
		VectorType::read(is);
		is.readEndStatement("readField");
		if(  this->size() != flen )
		{
			ioErrorInFile( is.name(), is.lineNumber() ) <<
			"  expected " << flen << " elements, but supplied "<<
			this->size() << " elements in file "<< is.name() <<endl;
			return false; 
		}
	}	

	return true;	
}


template<template<class, class> class VectorField, class T, class PropType>
bool pFlow::Field<VectorField, T, PropType>::readField
(
	iIstream& is,
	const size_t len,
	bool resume,
	bool readLength
)
{
	
	bool tokenFound;
	if( resume )
		tokenFound = is.findTokenResume(fieldKey_);
	else
		tokenFound = is.findToken(fieldKey_);

	if( !tokenFound )
	{
		ioErrorInFile( is.name(), is.lineNumber() ) <<
		" error in searching for filedkey " << fieldKey_<<endl;
		return false;
	}

	word fieldU;

	is >> fieldU;
	
	if(is.bad() || is.eof())
	{
		ioErrorInFile( is.name(), is.lineNumber()) <<
		" error in reading keyword from file.";
		return false;
	}

	if( fieldU == uniform__ )
	{
		
		return readUniform(is, len, readLength);

	}
	else if( fieldU == nonUniform__ )
	{

		return readNonUniform(is, len); 

	}
	else
	{
		ioErrorInFile( is.name(), is.lineNumber() ) <<
		"unknown keyword, expected uniform/nonUniform, but found " <<
		fieldU << endl;
		return false;
	}

	return true;
}


template<template<class, class> class VectorField, class T, class PropType>
bool pFlow::Field<VectorField, T, PropType>::readField
(
	iIstream& is,
	bool resume
)
{
	return readField(is, 0, resume ,true);
}


template<template<class, class> class VectorField, class T, class PropType>
bool pFlow::Field<VectorField, T, PropType>::writeField(iOstream& os)const
{
	os.writeWordKeyword(fieldKey_) << nonUniform__<<endl;
	os<< this->size()<<endl;
	if( os.isBinary() && !std::is_same_v<T,word>)
	{
		os.write(reinterpret_cast<const char*>(this->data()), this->size()*sizeof(T));
	}
	else
	{
		VectorType::write(os);
	}
	os.endEntry();
	return true;
}

