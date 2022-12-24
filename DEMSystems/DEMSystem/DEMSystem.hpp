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

#ifndef __DEMSystem_hpp__
#define __DEMSystem_hpp__

#include <vector>

#include "types.hpp"
#include "virtualConstructor.hpp"
#include "uniquePtr.hpp"
#include "systemControl.hpp"
#include "readControlDict.hpp"


namespace pFlow::coupling
{


class DEMSystem
{
protected:
	
	readControlDict 				ControlDict_;

	uniquePtr<systemControl>		Control_ = nullptr;

	std::vector<box> 				domains_;


	// methods 
	auto& Control()
	{
		return Control_();
	}


public:

	TypeInfo("DEMSystem");

	DEMSystem(
		word  demSystemName,
		int32 numDomains, 
		const std::vector<box>& domains, 
		int argc, 
		char* argv[]);

	virtual ~DEMSystem();

	DEMSystem(const DEMSystem&)=delete;

	DEMSystem& operator = (const DEMSystem&)=delete;

	create_vCtor(
		DEMSystem,
		word,
		(
			word  demSystemName,
			int32 numDomains, 
			const std::vector<box>& domains, 
			int argc, 
			char* argv[]
		),
		(
			demSystemName,
			numDomains, 
			domains, 
			argc, 
			argv
		));

	realx3 g()const
	{
		return Control_->g();
	}

	auto inline constexpr usingDoulle()const
	{
		return pFlow::usingDouble__;
	}

	virtual 
	int32 numParInDomain(int32 di)const = 0;
	
	virtual
	std::vector<int32> numParInDomain()const = 0;

	virtual 
	bool iterate(int32 n, real timeToWrite, word timeName) = 0;
	
	virtual 
	real maxBounndingSphereSize()const = 0;

	static 
	uniquePtr<DEMSystem>
		create(
			word  demSystemName,
			int32 numDomains, 
			const std::vector<box>& domains, 
			int argc, 
			char* argv[]);



};


} // pFlow

#endif // __DEMSystem_hpp__