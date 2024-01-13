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

#include "types.hpp"
#include "typeInfo.hpp"
#include "Timers.hpp"

namespace pFlow
{

class systemControl;
class Time;
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
		systemControl& 	control_;

		Time& 			time_;

		/// All timers (if any) of this component
		Timers 	timers_;

public:

	/// Type info
	TypeInfo("demComponent");

	// - Constructors
	
		/// construct from components
		demComponent(const word& name, systemControl& control);
		
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
		real dt()const;
		
		/// Current simulation time 
		real currentTime()const;
		
		inline
		const auto& time()const
		{
			return time_;
		}

		inline 
		auto& time()
		{
			return time_;
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