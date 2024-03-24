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

#ifndef __virtualConstructor_hpp__
#define __virtualConstructor_hpp__

#include <functional>

#include "types.hpp"
#include "error.hpp"
#include "Map.hpp"
#include "uniquePtr.hpp"

#define create_vCtor(baseClass,selectorName,argList,args) 								\
																						\
typedef std::function< uniquePtr<baseClass> argList > selectorName##FunctionType;		\
typedef wordMap<selectorName##FunctionType>   selectorName##vCtorSelectorType;			\
																						\
inline static selectorName##vCtorSelectorType selectorName##vCtorSelector_; 			\
																						\
																						\
template<typename dType>																\
class create##selectorName##Callback 													\
{																						\
public: 																				\
	create##selectorName##Callback ()		 											\
	{																					\
		auto success = 																	\
			selectorName##vCtorSelector_.insertIf										\
			(																			\
					word(dType::TYPENAME()),											\
					[&] argList -> uniquePtr<baseClass>									\
					{ 																	\
						return uniquePtr<baseClass>										\
						( 																\
							new dType args 												\
						);																\
					}																	\
			);																			\
																						\
		if( !success )																	\
		{																				\
			fatalErrorInFunction														\
			    << "Duplicate entry "<< dType::TYPENAME() 								\
			    << " in virtual constructor table of "<< #baseClass						\
			    << " with selector name " << #selectorName <<endl;						\
		}																				\
																						\
	}																					\
																						\
	create##selectorName##Callback														\
	(const create##selectorName##Callback&)= delete;									\
	void operator=																		\
	(const create##selectorName##Callback&)= delete;									\
};



#define add_vCtor(baseClass, derivedClass, selectorName) 	\
															\
inline static typename baseClass::template create##selectorName##Callback<derivedClass> baseClass##derivedClass##selectorName##_;



#endif // __virtualConstructor_hpp__
