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

#ifndef __rectMeshFieldToVTK_H__
#define __rectMeshFieldToVTK_H__


namespace pFlow
{

template<typename T>
bool convertRectMeshField(iOstream& os, rectMeshField_H<T>& field)
{
	fatalErrorInFunction<< "this type is not supported "<<
	field.typeName()<<endl;
	fatalExit;
	return false;
}


template<>
bool convertRectMeshField(iOstream& os, rectMeshField_H<real>& field)
{
	
	os<<"FIELD FieldData 1 " << field.name() << " 1 "<< field.size() << " float\n";
	for(int32 k=0; k<field.nz(); k++)
	{
		for(int32 j=0; j<field.ny(); j++)
		{
			for(int32 i=0; i<field.nx(); i++)
			{
				os<< field(i,j,k)<<"\n";
			}
		}
	}
	os<<endl;
	return true;
}

template<>
bool convertRectMeshField(iOstream& os, rectMeshField_H<realx3>& field)
{
	
	os<<"FIELD FieldData 1 " << field.name() << " 3 "<< field.size() << " float\n";
	for(int32 k=0; k<field.nz(); k++)
	{
		for(int32 j=0; j<field.ny(); j++)
		{
			for(int32 i=0; i<field.nx(); i++)
			{
				os<< field(i,j,k).x()<<" "<<field(i,j,k).y()<<" "<<field(i,j,k).z()<<"\n";
			}
		}
	}
	os<<endl;
	return true;
}

template<>
bool convertRectMeshField(iOstream& os, rectMeshField_H<int32>& field)
{
	
	os<<"FIELD FieldData 1 " << field.name() << " 1 "<< field.size() << " int\n";
	for(int32 k=0; k<field.nz(); k++)
	{
		for(int32 j=0; j<field.ny(); j++)
		{
			for(int32 i=0; i<field.nx(); i++)
			{
				os<< field(i,j,k)<<"\n";
			}
		}
	}
	os<<endl;
	return true;
}

}


#endif