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

class demComponent
{
protected:

	word 	componentName_;


	systemControl& control_;


	Timers 	timers_;

public:

	TypeInfo("demComponent");

	demComponent(const word& name, systemControl& control)
	:
		componentName_(name),
		control_(control),
		timers_(name, &control.timers())
	{}

	virtual ~demComponent() = default;


	const auto& control()const
	{
		return control_;
	}

	auto& control()
	{
		return control_;
	}

	inline
	real dt()const
	{
		return control_.time().dt();
	}

	inline
	real currentTime()const
	{
		return control_.time().currentTime();
	}

	auto& timers(){
		return timers_;
	}

	const auto& timers() const{
		return timers_;
	}


	virtual bool beforeIteration() = 0;


	virtual bool iterate() = 0;


	virtual bool afterIteration() = 0;

};

}

#endif