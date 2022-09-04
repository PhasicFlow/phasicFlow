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

#ifndef __insertionRegion_H__
#define __insertionRegion_H__

#include "timeFlowControl.H"
#include "shapeMixture.H"
#include "peakableRegions.H"
#include "setFieldList.H"

namespace pFlow
{

class dictionary;

class insertionRegion
:
	public timeFlowControl
{
protected:

	// - name of the region 
	word  		name_;

	// - type of insertion region
	word		type_;

	// peakable region of points
	uniquePtr<peakableRegion>  	pRegion_ = nullptr;

	// mixture of shapes 
	uniquePtr<shapeMixture>  	mixture_ = nullptr;

	// setFields for insertion region 
	uniquePtr<setFieldList>     setFields_ = nullptr;


	bool readInsertionRegion(const dictionary& dict);

	bool writeInsertionRegion(dictionary& dict) const;


public:

	TypeNameNV("insertionRegion");

	//// - Constructors

		insertionRegion(const dictionary& dict);

		insertionRegion(const insertionRegion& src);

		insertionRegion(insertionRegion&&) = default;

		insertionRegion& operator=(const insertionRegion&);

		insertionRegion& operator=(insertionRegion&&) = default;


		~insertionRegion() = default;


	//// - Methods 
		const auto& setFields()const
		{
			return setFields_();
		}

		const auto& name()const
		{
			return name_;
		}


	//// - IO operation

		bool read(const dictionary& dict)
		{
			if(!timeFlowControl::read(dict))return false;

			return readInsertionRegion(dict);
		}

		bool write(dictionary& dict)const
		{
			if(!timeFlowControl::write(dict)) return false;

			return writeInsertionRegion(dict);
		}


};

} //pFlow

#endif //__insertionRegion_H__
