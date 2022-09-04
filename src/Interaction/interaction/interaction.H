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

#ifndef __interaction_H__
#define __interaction_H__

#include "demInteraction.H"
#include "eventObserver.H"
#include "interactionBase.H"
#include "contactSearch.H"



namespace pFlow
{

class interaction
:
	public demInteraction,
	public eventObserver,
	public interactionBase
{
public:
	
	using IdType 				= typename interactionBase::IdType;

	using IndexType    			= typename interactionBase::IndexType;

	using ExecutionSpace 	 	= typename interactionBase::ExecutionSpace;

protected:

	/// interaction file dictionary 
	dictionary& 				fileDict_;

	/// contact search object for pp and pw interactions 
	uniquePtr<contactSearch> 	contactSearch_ = nullptr;

public:

	TypeName("interaction");


	//// - constructors 

		interaction(
			systemControl& control,
			const particles& prtcl,
			const geometry& geom );
		

	
		virtual ~interaction() = default;

		create_vCtor(
			interaction,
			systemControl,
			(
				systemControl& control,
				const particles& prtcl,
				const geometry& geom
			),
			(control, prtcl, geom)
			);
	
	auto& contactSearchPtr()
	{
		return contactSearch_;
	}

	auto& contactSearchRef()
	{
		return contactSearch_();
	}

	const auto& fileDict()const
	{
		return fileDict_;
	}

	static 
	uniquePtr<interaction> create(
		systemControl& control,
		const particles& prtcl,
		const geometry& geom);

};


}

#endif //__interaction_H__ 
