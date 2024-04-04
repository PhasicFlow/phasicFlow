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

#ifndef __interaction_hpp__
#define __interaction_hpp__

#include "demComponent.hpp"
#include "property.hpp"
#include "observer.hpp"
#include "systemControl.hpp"
#include "contactSearch.hpp"



namespace pFlow
{

class particles;
class geometry;

class interaction
:
	public property,
	public observer,
	public demComponent
	
{
private:

	/// reference to particles object 
	const particles& 	particles_;

	/// reference to geometry object 
	const geometry& 	geometry_;

	static inline
	const message msg_ = message::ITEM_REARRANGE;

public:

	TypeInfo("interaction");


	//// - constructors 

		interaction(
			systemControl& control,
			const particles& prtcl,
			const geometry& geom );
		
		~interaction() override = default;

		create_vCtor
		(
			interaction,
			systemControl,
			(
				systemControl& control,
				const particles& prtcl,
				const geometry& geom
			),
			(control, prtcl, geom)
		);
	
	inline
	const auto& Particles()const
	{
		return particles_;
	}

	inline
	const auto& Geometry()const
	{
		return geometry_;
	}

	static 
	uniquePtr<interaction> create(
		systemControl& control,
		const particles& prtcl,
		const geometry& geom);

};


}

#endif //__interaction_hpp__ 
