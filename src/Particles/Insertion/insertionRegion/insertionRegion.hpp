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

#ifndef __insertionRegion_hpp__
#define __insertionRegion_hpp__

#include "timeFlowControl.hpp"
#include "shapeMixture.hpp"
#include "peakableRegions.hpp"
#include "setFieldList.hpp"

namespace pFlow
{

class dictionary;

/**
 * This class defines all the necessary enteties for defining an insertion
 * region. 
 * 
 * Insertion region information are supplied through a dictionary in a file. 
 * For example:
\verbatim
{
	type 		cylinderRegion; // type of insertion region
	rate 		15000;  // insertion rate (particles/s)
	startTime	0;      // (s)
	endTime		0.5;    // (s)
	interval	0.025;  // (s)  

	cylinderRegionInfo 
	{
		radius 	0.09;       	// radius of cylinder (m)
   		p1 		(0.0 0.0 0.10); // (m,m,m)
   		p2 		(0.0 0.0 0.11); // (m,m,m)
	}
   
	setFields
	{
		velocity    realx3 (0.0 0.0 -0.6); // initial velocity of inserted particles 
	}
   
	mixture
	{
		lightSphere 1; // mixture composition of inserted particles 
	}
} \endverbatim
 *  
 * More information on the above dictionary entries can be found in
 * the table below.
 * 
 *  
 * | Parameter | Type | Description | Optional [default value] |
 * |----| :---: | ---- | ---- |
 * | type | word | type of the insertion region with name ### | No |
 * | rate | real | rate of insertion (particle/s) | No |
 * | startTime | real | start of insertion (s) | No |
 * | endTime | real | end of insertion (s) | No  |
 * | interval | real | time interval between successive insertions (s) | No |
 * | ###Info | dictionary | data for insertion region | No |
 * | setFields | dictionary | set field for inserted particles (s) | Yes [empty dictionray] |
 * | mixture | dictionary | mixture of particles to be inserted (s) | No |
 * 
 */
class insertionRegion
:
	public timeFlowControl
{
protected:

	/// name of the region 
	word  		name_;

	/// type of insertion region
	word		type_;

	/// peakable region of points
	uniquePtr<peakableRegion>  	pRegion_ = nullptr;

	/// mixture of shapes 
	uniquePtr<shapeMixture>  	mixture_ = nullptr;

	/// setFields for insertion region 
	uniquePtr<setFieldList>     setFields_ = nullptr;


	/// read from dictionary
	bool readInsertionRegion(const dictionary& dict);

	/// write to dictionary
	bool writeInsertionRegion(dictionary& dict) const;


public:

	/// Type info
	TypeInfoNV("insertionRegion");

	// - Constructors

		/// Construct from a dictionary
		insertionRegion(const dictionary& dict);

		/// Copy
		insertionRegion(const insertionRegion& src);

		/// Move 
		insertionRegion(insertionRegion&&) = default;

		/// Copy assignment
		insertionRegion& operator=(const insertionRegion&);

		/// Move assignment
		insertionRegion& operator=(insertionRegion&&) = default;

		/// Destructor 
		~insertionRegion() = default;


	// - Methods 

		/// Const ref to setFields
		const auto& setFields()const
		{
			return setFields_();
		}

		/// Const ref to name of the region
		const auto& name()const
		{
			return name_;
		}

	// - IO operation

		/// read from dictionary
		bool read(const dictionary& dict)
		{
			if(!timeFlowControl::read(dict))return false;

			return readInsertionRegion(dict);
		}

		/// write to dictionary
		bool write(dictionary& dict)const
		{
			if(!timeFlowControl::write(dict)) return false;

			return writeInsertionRegion(dict);
		}
};

} //pFlow

#endif //__insertionRegion_hpp__
