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

#ifndef __ProcessField_hpp__ 
#define __ProcessField_hpp__


#include "processField.hpp"
#include "rectMeshFields.hpp"
#include "twoPartEntry.hpp"
#include "fieldOperations.hpp"
#include "rectMeshFieldToVTK.hpp"

namespace pFlow
{


template<typename T>
class ProcessField
:
	public processField
{

protected:

	uniquePtr<pointField_H<T>> 	field_;


	rectMeshField_H<T> 	processedField_;

public:

	TypeInfoTemplate11("ProcessField", T);

	ProcessField(
		const dictionary& dict,
		pointRectCell& pToCell,
		repository& rep)
	:
		processField(dict, pToCell, rep),
		field_(
			this->isUniform()?
			timeFolder().createUniformPointField_H(this->fieldName(), getUniformValue(), false ):
			timeFolder().readPointField_H<T>(this->fieldName()) 
			),
		processedField_
		(
			mesh(),
			processedFieldName(),
			T{}
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
		
		auto numeratorPtr =  sumMaksOp( field_() , this->pointToCell(), incMask);
		uniquePtr<rectMeshField_H<real>> denomeratorPtr;
		
		if(operation() == "sum")
		{
			denomeratorPtr = makeUnique<rectMeshField_H<real>>(this->mesh(), static_cast<real>(1.0));

		}else if(operation() == "average")
		{

			pointField_H<real> oneFld(
				objectFile
				(
					"oneField",
					"", 
					objectFile::READ_NEVER, 
					objectFile::WRITE_NEVER
				),
				const_cast<pointStructure&>(field_().pStruct()), 
				static_cast<real>(1.0), 
				static_cast<real>(1.0)
			);

			denomeratorPtr = sumOp(oneFld, this->pointToCell());

		}else if(operation() == "averageMask")
		{
			//pointField_H<real> oneFld(field_().pStruct(), static_cast<real>(1.0), static_cast<real>(1.0));
			pointField_H<real> oneFld(
				objectFile
				(
					"oneField",
					"", 
					objectFile::READ_NEVER, 
					objectFile::WRITE_NEVER
				),
				const_cast<pointStructure&>(field_().pStruct()), 
				static_cast<real>(1.0), 
				static_cast<real>(1.0)
			);
			
			denomeratorPtr = sumMaksOp(oneFld, this->pointToCell(), incMask);

		}
		else
		{
			fatalErrorInFunction<<"operation is not known: "<< operation()<<endl;
			fatalExit;
		}

		auto& numerator = numeratorPtr();
		auto& denomerator = denomeratorPtr();

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



#endif //__ProcessField_hpp__
