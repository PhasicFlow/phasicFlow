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


#ifndef __triSurfaceFieldToVTK_hpp__
#define __triSurfaceFieldToVTK_hpp__

#include <regex>

#include "pointFieldToVTK.hpp"
#include "triSurface.hpp"
#include "multiTriSurface.hpp"
#include "triSurfaceFields.hpp"


namespace pFlow::TSFtoVTK
{

bool convertTimeFolderTriSurfaceFields( 
	systemControl& control,
	const fileSystem& destPath,
	const word& bName,
	bool separate,
	wordList& surfNames,
	wordList& fileNames);


bool triSurfaceToVTK(Ostream &os,
					const realx3 *points,
					const uint32x3 *vertices,
					const subSurface &subSurf);

bool triSurfaceToVTK(Ostream &os,
					const realx3* points,
					const uint32x3* vertices,
					uint32 numPoints,
					uint32 numTris);

bool convertTimeFolderTriSurfaceFieldsSingle( 
	multiTriSurface& surface,
	const fileSystem& destPath,
	real  time,
	const word& bName,
	wordList& surfNames,
	wordList& fileNames);

bool convertTimeFolderTriSurfaceFieldsSeparate( 
	multiTriSurface& surface,
	const fileSystem& destPath,
	real  time,
	const word& bName,
	wordList& surfNames,
	wordList& fileNames);

inline
bool regexCheck(const word& TYPENAME, const word& fieldType)
{
	std::regex match("triSurfaceField\\<([A-Za-z1-9_]*)\\,([A-Za-z1-9_]*)\\>");
	std::smatch search1;
	std::smatch search2;
	if(!std::regex_match(fieldType, search1, match))return false;
	if(!std::regex_match(TYPENAME, search2, match))return false;
	if(search1.size()!=3)return false;
	if(search1.size()!=search2.size())return false;
	return search1[1] == search2[1];
}

template<typename Type>
inline
bool checkTriFieldType(word objectType)
{
	return regexCheck(triSurfaceField<Type>::TYPENAME(), objectType);
}

bool convertRealx3TypetriSurfaceField(
	Ostream& os,
	const IOfileHeader& header,
	multiTriSurface& tSurface);

bool convertRealx3TypetriSurfaceFieldSeparate(
	const fileSystem& destPath,
	const IOfileHeader& header,
	multiTriSurface& tSurface,
	const word& bName,
	real time);




}

#endif 
