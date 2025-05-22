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
#include "span.hpp"
#include "box.hpp"
#include "virtualConstructor.hpp"
#include "uniquePtr.hpp"
#include "systemControl.hpp"
#include "readControlDict.hpp"


namespace pFlow
{


class DEMSystem
{
protected:
	
	readControlDict 				ControlDict_;

	uniquePtr<systemControl>		Control_ = nullptr;

	uniquePtr<Timers> 				timers_;


public:

	TypeInfo("DEMSystem");

	DEMSystem(
		word  demSystemName,
		const std::vector<box>& domains, 
		int argc, 
		char* argv[]);

	virtual ~DEMSystem();

	DEMSystem(const DEMSystem&)=delete;

	/// @brief no assignment 
	DEMSystem& operator = (const DEMSystem&)=delete;

	create_vCtor(
		DEMSystem,
		word,
		(
			word  demSystemName,
			const std::vector<box>& domains, 
			int argc, 
			char* argv[],
			bool requireRVel
		),
		(
			demSystemName,
			domains, 
			argc, 
			argv,
			requireRVel
		));

	realx3 g()const
	{
		return Control_->g();
	}

	// methods 
	auto& Control()
	{
		return Control_();
	}

	const auto& Control()const
	{
		return Control_();
	}

	auto inline constexpr usingDouble()const
	{
		return pFlow::usingDouble__;
	}

	Timers& timers()
	{
		return Control_->timers();
	}

	virtual
	bool updateParticleDistribution(real extentFraction, const std::vector<box> domains) = 0;

	virtual 
	int32 numParInDomain(int32 di)const = 0;
	
	virtual
	std::vector<int32> numParInDomains()const = 0;

	virtual 
	span<const int32> parIndexInDomain(int32 domIndx)const = 0;

	virtual
	span<real> diameter() = 0;
	
	virtual 
	span<uint32> particleId() = 0;

	virtual 
	span<real> courseGrainFactor() = 0;

	virtual
	span<realx3> acceleration()=0;

	virtual
	span<realx3> velocity() = 0;

	virtual
	span<realx3> position() = 0;

	virtual
	span<realx3> rAcceleration()=0;

	virtual
	span<realx3> rVelocity() = 0;

	virtual
	span<realx3> rPosition() = 0;

	virtual
	span<realx3> parFluidForce() = 0;

	virtual
	span<realx3> parFluidTorque() = 0;

	virtual 
	bool sendFluidForceToDEM() = 0;

	virtual
	bool sendFluidTorqueToDEM() = 0;
	
	virtual 
	real maxBounndingSphereSize()const = 0;

	virtual 
	bool beforeIteration() = 0;

	virtual 
	bool iterate(
		real upToTime, 
		real timeToWrite, 
		word timeName) = 0;

	virtual 
	bool iterate(real upToTime) = 0;
	
	
	static 
	uniquePtr<DEMSystem>
		create(
			word  demSystemName,
			const std::vector<box>& domains, 
			int argc, 
			char* argv[],
			bool requireRVel=false);

};


} // pFlow

#endif // __DEMSystem_hpp__
