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
#ifndef __helperTstream_hpp__
#define __helperTstream_hpp__


inline bool validTokenForStream(const token tok)
{
	if( tok.good() && !tok.isPunctuation() )return true;
	if( tok == token::SPACE) return false;
	if( tok == token::TAB) return false;
	if( tok == token::NL) return false;
	if( tok == token::NULL_TOKEN )return false;

	return true;
}

inline bool isBeginToken(const token& tok)
{
    if( tok.good() && !tok.isPunctuation() )return false;
    if( tok == token::BEGIN_LIST) 	return true;
    if( tok == token::BEGIN_BLOCK)	return true;
    if( tok == token::BEGIN_SQR)	return true;
    return false;
}

inline bool isEndToken(const token& tok)
{
    if( tok.good() && !tok.isPunctuation() )return false;
    if( tok == token::END_LIST) return true;
    if( tok == token::END_BLOCK)return true;
    if( tok == token::END_SQR)	return true;
    return false;
}

#endif // __helperTstream__
