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

#include "Timer.hpp"
#include "Timers.hpp"
#include "streams.hpp"

pFlow::Timer::Timer(const word name, Timers* parrent)
:
	name_(name),
	parrent_(parrent)
{
	if(parrent_)
		parrent_->addToList(this);
}


pFlow::Timer::~Timer()
{
	if(parrent_)
	{
		parrent_->removeFromList(this);
	}
}

pFlow::int32 pFlow::Timer::level()const
{
	if(parrent_)
		return parrent_->level()+1;
	else
		return 0;
}


bool pFlow::Timer::write(iOstream& os, bool subTree)const
{
	
	if(!timerActive() && !master())return true;
	

	int32 lvl = level();
	for(int32 l=1; l<lvl; l++)
	{	
		os<<"┃   ";
	}

	if(lvl>0)
	{

		if(master())
			os<<"┣━━ ";
		else
			if(lvl==1)
				os<<"┃└─ ";
			else
				os<<" └─ ";
	}
	else
		; //os<<"⊿ ";
	
	if(lvl==0)
		os<<greenColor<<boldChar;

	os<<name_;

	if(timerActive())
		if(master())
			os<<" execution time (s): total ("<<
				totalTime()<<"), av. ("<<
				averageTime()<<").";
		else
			os<<" execution time (s): total ("<<
				cyanText(totalTime())<<"), av. ("<<
				cyanText(averageTime())<<").";

	os<<defaultColor;
	os<<'\n';

	return true;
}