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


#ifndef __eventMessage_hpp__
#define __eventMessage_hpp__


namespace pFlow
{

class eventMessage
{
public:
	enum event : unsigned int
	{
		DELETE	 		= 1,
		INSERT  		= 2,
		REARRANGE  		= 4,
		SIZE_CHANGED 	= 8,
		CAP_CHANGED 	= 16,
		RANGE_CHANGED   = 32
	};

protected:

	 unsigned int message_;

public:

	eventMessage()
	:
		message_(0)
	{}

	eventMessage(unsigned int msg)
	:
		message_(msg)
	{}

	inline unsigned int get()const
	{
		return message_;
	}

	inline void set(unsigned int msg)
	{
		message_= msg;
	}

	inline void add(unsigned int msg)
	{
		message_ = message_+ msg;
	}

	inline bool equivalentTo( const event& evt )const
	{
		return (message_ & evt) == evt;
	}

	inline bool isNull()const
	{
		return message_ == 0u;
	}	
	inline bool isDeleted()const
	{
		return equivalentTo(event::DELETE);
	}

	inline bool isInsert()const
	{
		return equivalentTo(event::INSERT);
	}

	inline bool isRearranged()const
	{
		return equivalentTo(event::REARRANGE);
	}

	inline bool isSizeChanged()const
	{
		return equivalentTo(event::SIZE_CHANGED);
	}

	inline bool isCapacityChanged()const
	{
		return equivalentTo(event::CAP_CHANGED);
	}

	inline bool isRangeChanged()const
	{
		return equivalentTo(event::RANGE_CHANGED);
	}

};

}

#endif // __eventMessage_hpp__
