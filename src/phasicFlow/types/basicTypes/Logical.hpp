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
#ifndef __Logical_hpp__
#define __Logical_hpp__

#include "builtinTypes.hpp"
#include "bTypesFunctions.hpp"



namespace pFlow
{

//- Forward
class iIstream;
class iOstream;

/**
 *  Holds a bool value and converts strings to bool
 *
 */
class Logical
{
private:

	/// bool value
	bool                     s_ = false;

	/// Set numbe of of Yes or No
	int                      yesNoSet_ = 0;

	/// Set of Yes or Nos
	inline static const word YesNo__[4][2] = { { "Yes", "No" },
		                                       { "on", "off" },
		                                       { "true", "false" },
		                                       { "Ok", "No" } };

	/// Construct from bool and set number
	inline explicit Logical(bool s, int yns)
	  : s_(s),
	    yesNoSet_(yns)
	{
	}

public:

	/// Type info
	inline static word TYPENAME()
	{
		return "Logical";
	}

	inline word typeName()const
	{
		return TYPENAME();
	}

	//// Constructors

	/// Default constructor
	inline Logical() = default;

	/// Construct from bool
	inline explicit Logical(bool s)
	  : s_(s)
	{
	}

	/// Construct from word
	Logical(const word& l);

	/// Construct from char string
	Logical(const char* ch);

	/// Copy
	Logical(const Logical&) = default;

	/// Move
	Logical(Logical&&) = default;

	/// Copy assignment
	Logical&        operator=(const Logical&) = default;

	/// Move assignment
	Logical&        operator=(Logical&&) = default;

	/// Assignment with bool
	inline Logical& operator=(const bool& b)
	{
		s_        = b;
		yesNoSet_ = 0;
		return *this;
	}

	//// Methods

	/// () operator, return bool value
	inline bool operator()() const
	{
		return s_;
	}

	/// Return bool value
	inline explicit operator bool() const
	{
		return s_;
	}

	/// Not operator
	inline Logical operator!() const
	{
		return Logical(!s_, yesNoSet_);
	}

	//// IO operations

	bool read(iIstream& is);

	bool write(iOstream& os) const;

	//// Static members

	bool static evaluteWord(const word& l, bool& b, int& yesNoSet);
};

iIstream&
operator>>(iIstream& is, Logical& L);

iOstream&
operator<<(iOstream& os, const Logical& L);

} // pFlow

#endif // __Logical_hpp__
