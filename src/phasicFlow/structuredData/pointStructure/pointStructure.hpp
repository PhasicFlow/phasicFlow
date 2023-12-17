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


#ifndef __pointStructure_hpp__
#define __pointStructure_hpp__

#include "internalPoints.hpp"
#include "simulationDomain.hpp"
#include "boundaryList.hpp"
#include "uniquePtr.hpp"


namespace pFlow
{

class simulationDomain;


class pointStructure
:
	public internalPoints
{
protected:

	//// - data members 
	uniquePtr<simulationDomain> simulationDomain_ = nullptr;

	boundaryList 				boundaries_;
		
	
	bool distributePoints(const realx3Vector& points);

public:

		
	// - type info
	TypeInfo("pointStructure");
	

	//// - Constructors 

		// - an empty pointStructure, good for reading from file 
		pointStructure(const dictionary& domainDict);

		// - construct from components
		//pointStructure(const int8Vector& flgVec, const realx3Vector& posVec);

		/// construct from point positions, assume all points are active
		pointStructure(
			const dictionary& domainDict, 
			const realx3Vector& posVec);

		// - copy construct
		//
		//   should be changed, may causs undefined behavior 
		//////////////////////////////////////////////////////////////
		pointStructure(const pointStructure&) = delete;

		// - no move construct 
		pointStructure(pointStructure&&) = delete;

		// - copy assignment 
		//
		//   should be changed, may causs undefined behavior 
		//////////////////////////////////////////////////////////////
		pointStructure& operator=(const pointStructure&) = delete;

		// - no move assignment 
		pointStructure& operator=(pointStructure&&) = delete;

		// - destructor 
		virtual ~pointStructure() = default;
		

	/// Read 
		FUNCTION_H
		bool read(iIstream& is, IOPattern::IOType iotype);
		

		/// Write 
		/*FUNCTION_H
		bool write(iOstream& os, IOPattern::IOType iotype)const;	*/
		
		
		
		



};


} // pFlow




#endif //__pointStructure_hpp__
