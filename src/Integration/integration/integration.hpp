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

#ifndef __integration_hpp__
#define __integration_hpp__


#include "virtualConstructor.hpp"
#include "pointFields.hpp"
#include "Logical.hpp"


namespace pFlow
{

// - Forward
class repository;
class pointStructure;

/**
 * Base class for integrating the first order ODE (IVP) 
 * 
 * The ODE should be in the following form:
 *\f[ 
	 \frac{dy}{dt} = f(y,t)
  \f]
 * for example the equation of motion is in the following form:
 *\f[ 
	m\frac{d\vec{v}}{dt} = m\vec{g} + \sum \vec{f_c}(\vec{v},t)
  \f]
 *
 * The integration method can be either one-step or predictor-corrector type.
 * 
 */
class integration
{
private:

	// - Protected data members 

		/// The owner repository that all fields are storred in
		repository& owner_;

		/// A reference to pointStructure
		const pointStructure& pStruct_;

		/// The base name for integration 
		const word baseName_;

		bool 	keepHistory_;

protected:

	bool insertValues(
		const anyList& varList, 
		const deviceViewType1D<realx3>& srcVals,
		realx3PointField_D& dstFeild);

public:

	/// Type info
	TypeInfo("integration");


	// - Constructors

		/// Construct from components
		integration(
			const word& baseName,
			pointStructure& pStruct,
			const word& method,
			const realx3Field_D& initialValField,
			bool  keepHistory);

		/// Copy constructor 
		integration(const integration&) = default;

		/// Move constructor
		integration(integration&&) = default;

		/// Copy assignment 
		integration& operator = (const integration&) = default;

		/// Move assignment
		integration& operator = (integration&&) = default;

		/// Destructor 
		virtual ~integration()=default;

		/// Add a virtual constructor 
		create_vCtor
		(
			integration,
			word,
			(
				const word& baseName,
				pointStructure& pStruct,
				const word& method,
				const realx3Field_D& initialValField,
				bool keepHistory
			),
			(baseName, pStruct, method, initialValField, keepHistory)
		);


	// - Methods

		/// Const ref to pointStructure
		inline
		const auto& pStruct()const
		{
			return pStruct_;
		}

		/// Base name
		inline
		const word& baseName()const
		{
			return baseName_;
		}

		/// Ref to the owner repository
		inline
		repository& owner()
		{
			return owner_;
		}

		bool keepHistory()const
		{
			return keepHistory_;
		}

		virtual 
		void updateBoundariesSlaveToMasterIfRequested() = 0;
		/// return integration method 
		virtual 
		word method()const = 0 ;

		/// Prediction step in integration
		virtual 
		bool predict(real dt, realx3PointField_D& y, realx3PointField_D& dy) = 0;

		virtual 
		bool predict(real dt, realx3Field_D& y, realx3PointField_D& dy) = 0;

		/// Correction/main integration step
		virtual 
		bool correct(real dt, realx3PointField_D& y, realx3PointField_D& dy, real damping = 1.0) = 0;

		virtual 
		bool correctPStruct(real dt, pointStructure& pStruct, realx3PointField_D& vel) = 0;

	/// Create the polymorphic object based on inputs
	static
		uniquePtr<integration> create(
			const word& baseName,
			pointStructure& pStruct,
			const word& method,
			const realx3Field_D& initialValField,
			bool  keepHistory);

}; // integration

} // pFlow


#endif //__integration_hpp__
