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
:
    timeControl_(control.settingsDict().subDict("globalDamping"), control.time().dt(), "damping")
{
    const dictionary& dict = control.settingsDict().subDict("globalDamping");

    dampingFactor_ = dict.getValMin<real>("dampingFactor", static_cast<real>(1.0));

    dampingFactor_ = max( dampingFactor_ , static_cast<real>(0.01));

    performDamping_ = !equal(dampingFactor_, static_cast<real>(1.0));

    if( performDamping_ )
    {
      REPORT(2)<<"Global damping "<<Yellow_Text("is active")<<
		  " and damping factor is "<<dampingFactor_<<END_REPORT;
    }
		else
		{
      REPORT(2)<<"Global damping "<<Yellow_Text("is not active")<<"."<<END_REPORT;
    }

}

pFlow::real pFlow::globalDamping::dampingFactor(const timeInfo& ti)const
{
	if(!performDamping_) return 1.0;
	if(!timeControl_.eventTime(ti ))return 1.0;
	return dampingFactor_;
}