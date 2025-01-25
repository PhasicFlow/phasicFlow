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


#include "globalDamping.hpp"


pFlow::globalDamping::globalDamping(const systemControl& control)
{}
/*:
    timeControl_(control.settingsDict().subDict("globalDamping"), control.time().dt(), "damping")
{
    const dictionary& dict = control.settingsDict().subDict("globalDamping");

    dampingFactor_ = dict.getValOrSetMin<real>("dampingFactor", static_cast<real>(1.0));

    dampingFactor_ = max( dampingFactor_ , static_cast<real>(0.01));

    performDamping_ = !equal(dampingFactor_, static_cast<real>(1.0));

    if( performDamping_ )
		REPORT(2)<<"Global damping "<<Yellow_Text("is active")<<
		" and damping factor is "<<dampingFactor_<<END_REPORT;
	else
		REPORT(2)<<"Global damping "<<Yellow_Text("is not active")<<"."<<END_REPORT;

}*/


/*void pFlow::globalDamping::applyDamping
(
    const timeInfo& ti, 
    realx3PointField_D& velocity
)
{
	if(!performDamping_) return;
	if(!timeControl_.timeEvent(ti.iter(), ti.t(), ti.dt()) )return;

	auto d_v = velocity.deviceView();
	auto activeRng = velocity.activeRange();
	auto dmpng = dampingFactor_;

	Kokkos::parallel_for(
		"globalDamping::applyDamping",
		deviceRPolicyStatic(activeRng.start(), activeRng.end()),
		LAMBDA_HD(uint32 i){
			d_v[i] *= dmpng;
		});
	Kokkos::fence();
	//REPORT(1)<<"Applied global damping "<<END_REPORT;
}*/

pFlow::real pFlow::globalDamping::dampingFactor(const timeInfo& ti)const
{
  return 1;
	/*if(!performDamping_) return 1.0;
	if(!timeControl_.timeEvent(ti.iter(), ti.t(), ti.dt()) )return 1.0;
	return dampingFactor_;*/
}