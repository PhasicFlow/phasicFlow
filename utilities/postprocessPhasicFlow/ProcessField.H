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

#ifndef __ProcessField_H__ 
#define __ProcessField_H__


#include "processField.H"
#include "rectMeshFields.H"
#include "twoPartEntry.H"
#include "fieldOperations.H"
#include "rectMeshFieldToVTK.H"

namespace pFlow
{


template<typename T>
class ProcessField
:
	public processField
{

protected:

	pointField_H<T>& 	field_;


	rectMeshField_H<T>& 	processedField_;

public:

	TypeNameTemplate("ProcessField", T);


	ProcessField(
		const dictionary& dict,
		pointRectCell& pToCell,
		repository& rep)
	:
		processField(dict, pToCell, rep),
		field_(
			this->isUniform()?
			timeFolder().createUniformPointField_H(this->fieldName(), getUniformValue() ):
			timeFolder().readPointField_H<T>(this->fieldName()) 
			),
		processedField_
		(
			processedRepository().emplaceObject<rectMeshField_H<T>>
			(
				objectFile
				(
					processedFieldName(),
					"",
					objectFile::READ_NEVER,
					objectFile::WRITE_ALWAYS
				),
				mesh(),
				processedFieldName(),
				T{}
			)
		)
	{
		
	}

	add_vCtor(
		processField,
		ProcessField,
		dictionary);


	T getUniformValue()const
	{
		const dataEntry& entry = dict().dataEntryRef("field");
		twoPartEntry tpEntry(entry);
		return tpEntry.secondPartVal<T>();
	}

	virtual bool process() override
	{
		
		const includeMask& incMask = includeMask_();
		
		auto numerator = sumMaksOp( field_ , this->pointToCell(), incMask);
		
		rectMeshField_H<real> denomerator( this->mesh(), real{} );

		if(operation() == "sum")
		{
			denomerator = rectMeshField_H<real>(this->mesh(), static_cast<real>(1.0));

		}else if(operation() == "average")
		{

			pointField_H<real> oneFld(field_.pStruct(), static_cast<real>(1.0), static_cast<real>(1.0));

			denomerator = sumOp(oneFld, this->pointToCell());

		}else if(operation() == "averageMask")
		{
			pointField_H<real> oneFld(field_.pStruct(), static_cast<real>(1.0), static_cast<real>(1.0));

			denomerator = sumMaksOp(oneFld, this->pointToCell(), incMask);
		}else
		{
			fatalErrorInFunction<<"operation is not known: "<< operation()<<endl;
			fatalExit;
		}

		
		for(int32 i=0; i<this->mesh().nx(); i++ )
		{
			for(int32 j=0; j<this->mesh().ny(); j++ )
			{
				for(int32 k=0; k<this->mesh().nz(); k++ )
				{
					if( pointToCell().nPointInCell(i,j,k)>= threshold() )
					{
						processedField_(i,j,k) = numerator(i,j,k)/denomerator(i,j,k);
					}
					else
					{
						processedField_(i,j,k) = T{};
					}
				}		
			}	
		}


		return true;
	}


	bool writeToVTK(iOstream& os)const override
	{
		return convertRectMeshField(os, processedField_);
	}
	
};


} // pFlow



#endif //__ProcessField_H__
