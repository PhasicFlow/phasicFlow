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


#include "Lists.hpp"
#include "internalPoints.hpp"

#include "Vectors.hpp"
#include "VectorSingles.hpp"
#include "VectorDuals.hpp"
#include "Fields.hpp"
#include "eventSubscriber.hpp"
#include "indexContainer.hpp"



namespace pFlow
{

//forward
class boundaryBase;


class pointStructure
:
	public eventSubscriber
{
public:
	
	using boundaryListType = ListPtr<boundaryBase>;

protected:

	//// - data members 
		domain 				globalDomain_;

		domain 				thisDomain_;

		internalPoints 		internal_;

		boundaryListType 	boundaries_;

		wordList 			boundaryTypes_;	

public:

		
	// - type info
	TypeInfo("pointStructure");
	

	//// - Constructors 

		// - an empty pointStructure, good for reading from file 
		pointStructure();

		// - construct from components
		pointStructure(const int8Vector& flgVec, const realx3Vector& posVec);

		// - construct from point positions, assume all points are active
		pointStructure(const realx3Vector& posVec);

		// - copy construct
		//
		//   should be changed, may causs undefined behavior 
		//////////////////////////////////////////////////////////////
		pointStructure(const pointStructure&) = default;

		
		// - no move construct 
		pointStructure(pointStructure&&) = delete;

		// - copy assignment 
		//
		//   should be changed, may causs undefined behavior 
		//////////////////////////////////////////////////////////////
		pointStructure& operator=(const pointStructure&) = default;

		// - no move assignment 
		pointStructure& operator=(pointStructure&&) = delete;

		// - destructor 
		virtual ~pointStructure() = default

		// - size of data structure
		FUNCTION_H
		label size()const;
		
		// - maximum capacity of data structure 
		FUNCTION_H
		label capacity()const;
		
		
		// - update data structure by inserting/setting new points 
		//   Notifies all the fields in the registered list of data structure
		//   and exclude the fields that re in the exclusionList
		//   retrun nullptr if it fails 
		FUNCTION_H
		/*virtual uniquePtr<int32IndexContainer> insertPoints(
			const realx3Vector& pos,
			const setFieldList& setField,
			repository& owner,
			const List<eventObserver*>& exclusionList={nullptr}
		);*/

		boundaryBase& boundary(size_t i)
		{
			return boundaries_[i];
		}


		const boundaryBase& boundary(size_t i)const
		{
			return boundaries_[i];
		}


	
	//// - IO operations 
		
		



};


} // pFlow




#endif //__pointStructure_hpp__
