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


#ifndef __geometric_hpp__
#define __geometric_hpp__ 

#include "vtkFile.hpp"
#include "triSurface.hpp"
#include "multiTriSurface.hpp"
#include "IOobject.hpp"

namespace pFlow
{


template<typename ObjType>
bool geomObjectToVTK(IOfileHeader& header, real time, fileSystem destPath, word bName)
{

	if( ObjType::TYPENAME() != header.objectType() )return false;

	auto ioObjPtr = IOobject::make<ObjType>(header);

	auto& data = ioObjPtr().template getObject<ObjType>();

	vtkFile vtk(destPath, bName, time);

	if(!vtk) return false;

	Report(1)<<"Converting geometry to vtk."<<endReport;

	if(!dataToVTK(vtk, data) )
	{
		fatalErrorInFunction<<
		"  error in writing object "<<ioObjPtr().typeName() << " to folder " << destPath<<endl;
		fatalExit;
	}
	
	return true;
}

template<typename Type>
bool dataToVTK(vtkFile& vtk, const Type& dataEntity)
{
	fatalErrorInFunction<<
	"not implemented function!";
	fatalExit;
	return false;
}

template<>
bool dataToVTK( vtkFile& vtk, const triSurface& surface );

template<>
bool dataToVTK( vtkFile& vtk, const multiTriSurface& surface );


}

#endif //__geometric_hpp__
