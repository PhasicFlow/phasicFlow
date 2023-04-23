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

#ifndef __demComponent_hpp__ 
#define __demComponent_hpp__

#include "systemControl.hpp"



namespace pFlow
{

/**
 * A base class for every main component of DEM system.
 * 
 * This class provides abstraction at a very high level for any solver/utility
 * that forces the derived component (i.e. particles, geometry, and etc) to 
 * advance over time when iterate is called in time a loop.
 * 
 */
class demComponent
{
protected:

	// - Protected data members 

		/// Name of the DEM component 
		word 	componentName_;

		/// Reference to systemControl 
		systemControl& control_;

		/// All timers (if any) of this component
		Timers 	timers_;

public:

	/// Type info
	TypeInfo("demComponent");

	// - Constructors
	
		/// construct from components
		demComponent(const word& name, systemControl& control)
		:
			componentName_(name),
			control_(control),
			timers_(name, &control.timers())
		{}

		/// No copy constructor
		demComponent(const demComponent&) = delete;

		/// No move constructor 
		demComponent(demComponent&&) = delete;

		/// No copy assignment
		demComponent& operator = (const demComponent&) = delete;

		/// No move assignment 
		demComponent& operator =(demComponent&&) = delete;

		/// destructor 
		virtual ~demComponent() = default;

	
	// - Member functions 

		/// Const ref to systemControl
		inline
		const auto& control()const
		{
			return control_;
		}

		/// Ref to systemControl
		inline
		auto& control()
		{
			return control_;
		}

		/// Time step of integration
		inline
		real dt()const
		{
			return control_.time().dt();
		}

		/// Current simulation time 
		inline
		real currentTime()const
		{
			return control_.time().currentTime();
		}

		/// Const ref to timers
		inline
		const auto& timers() const
		{
			return timers_;
		}

		/// Ref to timers
		inline
		auto& timers()
		{
			return timers_;
		}

		/// This is called before the start of time loop
		virtual
		bool beforeTimeLoop()
		{
			notImplementedFunction;
			return false;
		}

		/// This is called in time loop, before iterate
		virtual 
		bool beforeIteration() = 0;

		/// This is called in time loop. Perform the main calculations 
		/// when the component should evolve along time.
		virtual 
		bool iterate() = 0;

		/// This is called in time loop, after iterate.
		virtual 
		bool afterIteration() = 0;

		/// This is called after the time loop
		virtual
		bool afterTimeLoop()
		{
			notImplementedFunction;
			return false;
		}

};

}

#endif