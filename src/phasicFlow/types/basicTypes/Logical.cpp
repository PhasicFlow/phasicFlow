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

#include "Logical.hpp"
#include "error.hpp"
#include "iIstream.hpp"
#include "iOstream.hpp"

pFlow::Logical::Logical(const word& l)
{
	bool s;
	int yesNoSet;
	if (!evaluteWord(l, s, yesNoSet))
	{
		fatalErrorInFunction << "  invalid input for Logical: " << l << endl;
		fatalExit;
	}
	*this = Logical(s, yesNoSet);
}

pFlow::Logical::Logical(const char* ch)
  : Logical(word(ch))
{
}

bool
pFlow::Logical::evaluteWord(const word& l, bool& b, int& yesNoSet)
{
	auto Ul = toUpper(l);

	for (int i = 1; i < 5; ++i)
	{
		if (toUpper(YesNo__[i][0]) == Ul)
		{
			b        = true;
			yesNoSet = i;
			return true;
		}
		else if (toUpper(YesNo__[i][1]) == Ul)
		{
			b        = false;
			yesNoSet = i;
			return true;
		}
	}
	yesNoSet = 0;
	return false;
}

bool
pFlow::Logical::read(iIstream& is)
{
	token t(is);
	word  w;
	if (!t.good())
	{
		ioErrorInFile(is.name(), is.lineNumber())
		  << "Bad token - could not get Logical value";
		is.setBad();
		return false;
	}

	if (t.isString())
	{
		w = t.stringToken();
	}
	else if (t.isWord())
	{
		w = t.wordToken();
	}
	else
	{
		ioErrorInFile(is.name(), is.lineNumber())
		  << "Wrong token type - expected Logical value, found " << t;
		is.setBad();
		return false;
	}
	bool s;
	int yesNoSet;
	if( evaluteWord(w, s, yesNoSet) )
	{
		*this = Logical(s, yesNoSet);
		return true;
	}
	return false;
}

bool
pFlow::Logical::write(iOstream& os) const
{
	if (s_ > 0)
	{
		os << YesNo__[s_][0];
	}
	else
	{
		os << YesNo__[-s_][1];
	}
	return os.check(FUNCTION_NAME);
}

pFlow::iIstream&
pFlow::operator>>(iIstream& is, Logical& L)
{
	if (!L.read(is))
	{
		fatalExit;
	}
	return is;
}

pFlow::iOstream&
pFlow::operator<<(iOstream& os, const Logical& L)
{
	if (!L.write(os))
	{
		fatalExit;
	}
	return os;
}